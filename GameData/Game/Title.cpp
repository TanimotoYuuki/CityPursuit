#include "stdafx.h"
#include "system/system.h"
#include "Title.h"
#include "SceneManager.h"
#include "FadeManager.h"
#include "Loading.h"
#include "TitleBackGround.h"
#include "Player.h"
#include "PlayerMove.h"
#include "PlayerJump.h"
#include "TitleCamera.h"
#include "TitleSprite.h"
#include "TitleSelect.h"

namespace {
	const float DELAY_TIME = 1.5f;//待機時間

	const Vector3 PLAYER_TITLE_INIT_POSISION{ 500.0f,4500.0f,-16250.0f };//プレイヤーのタイトル画面での初期位置

	const Vector3 PLAYER_TITLE_SCALE{ 8.0f,8.0f,8.0f };//プレイヤーのタイトル画面での大きさ

	const float IN_GAME_TRANSITION_PLAYER_POSITION_Y = 6000.0f;//インゲームに遷移するときのプレイヤーの高さ

	const float BGM_VOLUME_DOWN = 0.04f;//BGMの音量を下げる

	const float BGM_VOLUME_NONE = 0.0f;//BGMの音量なし
}

//デストラクタ
Title::~Title()
{
	DeleteGO(m_titleBackGround->GetTitleBuildingPtr());
	DeleteGO(m_titleCamera);
	DeleteGO(m_titleSprite);
	DeleteGO(m_titleSelect);
}

//開始処理
bool Title::Start()
{
	m_titleBackGround = FindGO<TitleBackGround>("titlebackground");
	if (m_titleBackGround == nullptr)
	{
		m_titleBackGround = NewGO<TitleBackGround>(0, "titlebackground");
	}
	else
	{
		m_titleBackGround->CreateTitleBuilding();
	}

	m_player = FindGO<Player>("player");
	if (m_player == nullptr)
	{
		m_player = NewGO<Player>(0, "player");
	}
	m_player->SetTitlePtr(this);
	m_player->SetPosition(PLAYER_TITLE_INIT_POSISION);
	m_player->SetRotation(Quaternion::Identity);
	m_player->SetScale(PLAYER_TITLE_SCALE);

	m_titleCamera = NewGO<TitleCamera>(0, "titlecamera");

	m_titleSprite = NewGO<TitleSprite>(0, "titlesprite");
	m_titleSprite->EnableDrawingUI(TitleSprite::enDrawingUI_TextUI);

	m_titleSelect = NewGO<TitleSelect>(0, "titleselect");

	FadeManager::GetInstance()->SetFadeState(FadeManager::enFadeState_FadeIn);
	g_renderingEngine->GetGameEndPostEffect().SetDrawingGameEndPostEffect(GameEndPostEffect::enGameEndPostEffect_None);
	return true;
}

//更新処理
void Title::Update()
{
	//タイトルの状態
	switch (m_titleState)
	{
	case enTitleState_Fade://フェード
		FadeUpdate();
		break;
	case enTitleState_TitleText://タイトルのテキスト
		TitleTextUpdate();
		break;
	case enTitleState_Select://選択
		SelectUpdate();
		break;
	case enTitleState_GameStartDirection://ゲームスタート演出
		GameStartDirectionUpdate();
		break;
	case enTitleState_TransitionScene://シーン遷移
		TransitionSceneUpdate();
	default:
		break;
	}

	//タイトルBGMの再生
	GameSoundEngine::GetInstance()->PlayBGM(GameSoundList_BGM_Title, 1.0f);
}

//フェードの更新処理
void Title::FadeUpdate()
{
	//フェード処理が終わったら次のステートに移行する
	if (FadeManager::GetInstance()->IsFinishFade())
	{
		m_titleState = enTitleState_TitleText;
	}
}

//タイトルのテキストの更新処理
void Title::TitleTextUpdate()
{
	//Aボタンを押したら次のステートに移行する
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_titleSprite->DisableDrawingUI(TitleSprite::enDrawingUI_TextUI);
		m_titleSelect->EnableDrawingUI();
		GameSoundEngine::GetInstance()->PlaySE(GameSoundList_SE_Decision, 1.2f);
		m_titleState = enTitleState_Select;
	}
}

//選択の更新処理
void Title::SelectUpdate()
{
	m_titleSelect->Execute();

	//選択できていないとき処理する
	if (!m_titleSelect->IsSelect())
	{
		//Bボタンを押したら前のステートに移行する
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			m_titleSprite->EnableDrawingUI(TitleSprite::enDrawingUI_TextUI);
			m_titleSelect->DisableDrawingUI();
			m_titleSelect->Reset();
			GameSoundEngine::GetInstance()->PlaySE(GameSoundList_SE_Cancel, 1.5f);
			m_titleState = enTitleState_TitleText;
		}
	}

	//選択したときの演出が終わっていたら次のステートに移行する
	if (m_titleSelect->IsFinishSelectDecisionDirection())
	{
		//ゲームスタートを選択したら演出ステートに移行する
		if (m_titleSelect->GetCurrentSelect() == TitleSelect::enTitleSelect_GameStart)
		{
			m_titleSelect->DisableDrawingUI();
			m_player->GetPlayerMove()->GetPlayerJump().JumpStart();
			m_titleState = enTitleState_GameStartDirection;
		}
		//遊び方を選択したらシーン遷移ステートに移行する
		else if (m_titleSelect->GetCurrentSelect() == TitleSelect::enTitleSelect_HowToPlay)
		{
			m_titleSprite->EnableDrawingUI(TitleSprite::enDrawingUI_HowToPlayUI);
			m_titleSelect->DisableDrawingUI();
			m_titleState = enTitleState_TransitionScene;
		}
		//ゲーム終了を選択したらシーン遷移ステートに移行する
		else
		{
			m_titleState = enTitleState_TransitionScene;
		}
	}
}

//ゲームスタート演出の更新処理
void Title::GameStartDirectionUpdate()
{
	if (!m_isGameStartDirectionPlayerJump)
	{
		if (g_gameTime->StopWatch(0.2f))
		{
			m_isGameStartDirectionPlayerJump = true;
		}
		return;
	}

	Vector3 position = m_player->GetPosition();
	position.y += 60.0f;
	m_player->SetPosition(position);

	//プレイヤーが特定の高さまで到達したら次のステートに移行する
	if (m_player->GetPosition().y >= IN_GAME_TRANSITION_PLAYER_POSITION_Y)
	{
		m_titleState = enTitleState_TransitionScene;
	}
}

//シーンの遷移の更新処理
void Title::TransitionSceneUpdate()
{
	float bgmVolume = GameSoundEngine::GetInstance()->GetSoundInstance(GameSoundList_BGM_Title)->GetVolume();

	//現在の選択
	switch (m_titleSelect->GetCurrentSelect())
	{
	case TitleSelect::enTitleSelect_GameStart:
		if (bgmVolume > BGM_VOLUME_NONE)
		{
			//タイトル画面BGMの音量を下げる
			bgmVolume -= BGM_VOLUME_DOWN;
			GameSoundEngine::GetInstance()->SetVolume(GameSoundList_BGM_Title, bgmVolume);
		}
		else
		{
			//タイトル画面BGMの音量を0に固定する
			bgmVolume = BGM_VOLUME_NONE;
			GameSoundEngine::GetInstance()->SetVolume(GameSoundList_BGM_Title, bgmVolume);
		}

		FadeManager::GetInstance()->SetFadeState(FadeManager::enFadeState_FadeOut);
		if (FadeManager::GetInstance()->IsFinishFade())
		{
			Loading::GetInstance()->StartLoading();
			//3秒経過したらインゲームシーンへ遷移する
			if (g_gameTime->StopWatch(3.0f))
			{
				SceneManager::GetInstance()->CreateScene(SceneManager::enSceneID_InGame);//インゲームシーンの生成
				DeleteGO(GameSoundEngine::GetInstance()->GetSoundInstance(GameSoundList_BGM_Title));
				DeleteGO(this);
			}
		}
		break;
	case TitleSelect::enTitleSelect_HowToPlay:
		if(g_pad[0]->IsTrigger(enButtonA))
		{
			m_titleSprite->DisableDrawingUI(TitleSprite::enDrawingUI_HowToPlayUI);
			m_titleSelect->EnableDrawingUI();
			m_titleSelect->Reset();
			GameSoundEngine::GetInstance()->PlaySE(GameSoundList_SE_Decision, 1.2f);
			m_titleState = enTitleState_Select;
		}
		break;
	case TitleSelect::enTitleSelect_GameFinish:
		//ゲーム終了
		g_gameLoop.m_isLoop = false;
		break;
	default:
		break;
	}
}