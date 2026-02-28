#include "stdafx.h"
#include "GameOver.h"
#include "Game.h"
#include "GameTimeLimit.h"
#include "GameOverSprite.h"
#include "GameEndSelect.h"
#include "SceneManager.h"
#include "FadeManager.h"
#include "Loading.h"

namespace {
	//BGM
	const float BGM_VOLUME_DOWN = 0.04f;//BGMの音量を下げる

	const float BGM_VOLUME_NONE = 0.0f;//BGMの音量なし
}

//デストラクタ
GameOver::~GameOver()
{
	DeleteGO(m_gameOverSprite);
	DeleteGO(m_gameEndSelect);
	DeleteGO(GameSoundEngine::GetInstance()->GetSoundInstance(GameSoundList_BGM_GameOver));
}

//開始処理
bool GameOver::Start()
{
	m_gameOverSprite = NewGO<GameOverSprite>(0, "gameovertext");
	m_gameEndSelect = NewGO<GameEndSelect>(0, "gameendselect");
	DeleteGO(GameSoundEngine::GetInstance()->GetSoundInstance(GameSoundList_BGM_InGame));
	GameSoundEngine::GetInstance()->PlaySE(GameSoundList_SE_TimeUp, 1.0f);
	return true;
}

//更新処理
void GameOver::Update()
{
	//ゲームオーバーの状態
	switch (m_gameOverState)
	{
	case enGameOverState_TimeLimit://時間切れ
		TimeLimitUpdate();
		break;
	case enGameOverState_Monochrome://モノクロ化
		MonochromeUpdate();
		break;
	case enGameOverState_GameOverText://ゲームオーバーのテキスト
		GameOverTextUpdate();
		break;
	case enGameOverState_Select://選択
		SelectUpdate();
		break;
	case enGameOverState_TransitionScene://シーン遷移
		TransitionSceneUpdate();
		break;
	default:
		break;
	}
}

//時間切れの更新処理
void GameOver::TimeLimitUpdate()
{
	//時間切れの演出が終わったら次のステートに移行する
	if (m_game->GetGameTimeLimitPtr()->IsFinishDirection())
	{
		m_game->GetGameTimeLimitPtr()->DisableDrawingUI();
		m_gameOverState = enGameOverState_Monochrome;
	}
}

//モノクロ化の更新処理
void GameOver::MonochromeUpdate()
{
	g_renderingEngine->GetGameEndPostEffect().SetDrawingGameEndPostEffect(GameEndPostEffect::enGameEndPostEffect_Monochrome);
	GameSoundEngine::GetInstance()->PlayBGM(GameSoundList_BGM_GameOver, 1.0f);

	//モノクロ化が終わったら次のステートに移行する
	if (g_renderingEngine->GetGameEndPostEffect().IsFinishDrawingGameEndPostEffect())
	{
		m_gameOverState = enGameOverState_GameOverText;
	}
}

//ゲームオーバーのテキストの更新処理
void GameOver::GameOverTextUpdate()
{
	m_gameOverSprite->PlaySpriteAnimation();

	//スプライトのアニメーションの再生が終わったら次のステートに移行する
	if (m_gameOverSprite->IsFinishSpriteAnimation())
	{
		m_gameEndSelect->EnableDrawingUI();
		m_gameOverState = enGameOverState_Select;
	}
}

//選択の更新処理
void GameOver::SelectUpdate()
{
	m_gameEndSelect->Execute();

	//選択したときの演出が終わっていたら次のステートに移行する
	if (m_gameEndSelect->IsFinishSelectDecisionDirection())
	{
		m_gameOverState = enGameOverState_TransitionScene;
	}
}

//シーンの遷移の更新処理
void GameOver::TransitionSceneUpdate()
{
	float bgmVolume = GameSoundEngine::GetInstance()->GetSoundInstance(GameSoundList_BGM_GameOver)->GetVolume();

	if (bgmVolume > BGM_VOLUME_NONE)
	{
		//ゲームクリアまたはゲームオーバーBGMの音量を下げる
		bgmVolume -= BGM_VOLUME_DOWN;
		GameSoundEngine::GetInstance()->SetVolume(GameSoundList_BGM_GameOver, bgmVolume);
	}
	else
	{
		//ゲームクリアまたはゲームオーバーBGMの音量を0に固定する
		bgmVolume = BGM_VOLUME_NONE;
		GameSoundEngine::GetInstance()->SetVolume(GameSoundList_BGM_GameOver, bgmVolume);
	}

	//現在の選択
	FadeManager::GetInstance()->SetFadeState(FadeManager::enFadeState_FadeOut);
	switch (m_gameEndSelect->GetCurrentSelect())
	{
	case GameEndSelect::enGameEndSelect_Retry:
		if (FadeManager::GetInstance()->IsFinishFade())
		{
			Loading::GetInstance()->StartLoading();
			//3秒経過したらインゲームシーンへ遷移する
			if (g_gameTime->StopWatch(3.0f))
			{
				SceneManager::GetInstance()->CreateScene(SceneManager::enSceneID_InGame);//インゲームシーンの生成
				DeleteGO(m_game);
				DeleteGO(this);
			}
		}
		break;
	case GameEndSelect::enGameEndSelect_ReturnTitle:
		if (FadeManager::GetInstance()->IsFinishFade())
		{
			SceneManager::GetInstance()->CreateScene(SceneManager::enSceneID_Title);//タイトルシーンの生成
			DeleteGO(m_game);
			DeleteGO(this);
		}
		break;
	default:
		break;
	}
}