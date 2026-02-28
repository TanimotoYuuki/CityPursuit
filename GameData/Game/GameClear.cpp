#include "stdafx.h"
#include "GameClear.h"
#include "GameClearCamera.h"
#include "Game.h"
#include "GameClearSprite.h"
#include "GameTimeLimit.h"
#include "GameResult.h"
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
GameClear::~GameClear()
{
	DeleteGO(m_gameClearCamera);
	DeleteGO(m_gameClearSprite);
	DeleteGO(m_gameResult);
	DeleteGO(m_gameEndSelect);
	DeleteGO(GameSoundEngine::GetInstance()->GetSoundInstance(GameSoundList_BGM_GameClear));
}

//開始処理
bool GameClear::Start()
{
	m_gameClearCamera = NewGO<GameClearCamera>(0, "gameclearcamera");
	m_gameClearCamera->SetGameClearPtr(this);

	m_gameClearSprite = NewGO<GameClearSprite>(0, "gameclearsprite");

	m_gameResult = NewGO<GameResult>(0, "gameresult");
	m_gameResult->SetQteEventSuccessCount(m_game->GetQteEventSuccessCount());
	m_gameResult->SetQteEventFailedCount(m_game->GetQteEventFailedCount());
	m_gameResult->SetGameClearTimeLimit(m_game->GetGameTimeLimitPtr()->GetTimeLimit());

	m_gameEndSelect = NewGO<GameEndSelect>(0,"gameendselect");
	DeleteGO(GameSoundEngine::GetInstance()->GetSoundInstance(GameSoundList_BGM_InGame));
	GameSoundEngine::GetInstance()->PlayBGM(GameSoundList_BGM_GameClear, 1.0f);
	return true;
}

//更新処理
void GameClear::Update()
{
	//ゲームクリアの状態
	switch (m_gameClearState)
	{
	case GameClear::enGameClearState_GameClearDirection://ゲームクリア演出
		GameClearDirectionUpdate();
		break;
	case GameClear::enGameClearState_Result://リザルト
		ResultUpdate();
		break;
	case GameClear::enGameClearState_Select://選択
		SelectUpdate();
		break;
	case enGameClearState_TransitionScene://シーン遷移
		TransitionSceneUpdate();
		break;
	default:
		break;
	}
}

//ゲームクリア演出の更新処理
void GameClear::GameClearDirectionUpdate()
{
	//ゲームクリア時のカメラの実行処理
	m_gameClearCamera->Execute();

	//ゲームクリアカメラの回転が終了したらゲームクリアのスプライトを表示させる
	if (m_gameClearCamera->IsFinishRotationCamera())
	{
		m_gameClearSprite->PlayAlphaSpriteAnimation(GameClearSprite::enSpriteAlphaState_Opaque);
	}

	//ゲームクリアカメラの移動が終了したら次のステートに移行する
	if (m_gameClearCamera->IsFinishMoveCamera())
	{
		m_gameClearState = enGameClearState_Result;
	}
}

//ゲームリザルトの更新処理
void GameClear::ResultUpdate()
{
	m_gameResult->Execute();

	//透明度(不透明から透明)を変えるアニメーションを再生していたらゲームクリアのスプライトを非表示させる
	if (m_gameResult->IsPlayNoneAlphaAnimation())
	{
		m_gameClearSprite->PlayAlphaSpriteAnimation(GameClearSprite::enSpriteAlphaState_Transparent);
		g_renderingEngine->GetGameEndPostEffect().SetDrawingGameEndPostEffect(GameEndPostEffect::enGameEndPostEffect_Boke);
	}

	//透明度(不透明から透明)を変えるアニメーションを再生終了していたら次のステートに移行する
	if (m_gameResult->IsFinishNoneAlphaAnimation())
	{
		m_gameEndSelect->EnableDrawingUI();
		m_gameClearState = enGameClearState_Select;
	}
}

//選択の更新処理
void GameClear::SelectUpdate()
{
	m_gameEndSelect->Execute();

	//選択したときの演出が終わっていたら次のステートに移行する
	if (m_gameEndSelect->IsFinishSelectDecisionDirection())
	{
		m_gameClearState = enGameClearState_TransitionScene;
	}
}

//シーンの遷移の更新処理
void GameClear::TransitionSceneUpdate()
{
	float bgmVolume = GameSoundEngine::GetInstance()->GetSoundInstance(GameSoundList_BGM_GameClear)->GetVolume();

	if (bgmVolume > BGM_VOLUME_NONE)
	{
		//ゲームクリアまたはゲームオーバーBGMの音量を下げる
		bgmVolume -= BGM_VOLUME_DOWN;
		GameSoundEngine::GetInstance()->SetVolume(GameSoundList_BGM_GameClear, bgmVolume);
	}
	else
	{
		//ゲームクリアまたはゲームオーバーBGMの音量を0に固定する
		bgmVolume = BGM_VOLUME_NONE;
		GameSoundEngine::GetInstance()->SetVolume(GameSoundList_BGM_GameClear, bgmVolume);
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