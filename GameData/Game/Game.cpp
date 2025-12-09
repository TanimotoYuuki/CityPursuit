#include "stdafx.h"
#include "Game.h"
#include "GameBackGround.h"
#include "Player.h"
#include "Enemy.h"
#include "GameTimeLimit.h"
#include "GameMission.h"
#include "MiniMap.h"
#include "GameStartSprite.h"
#include "SceneManager.h"
#include "FadeManager.h"
#include "Loading.h"
#include "DebugLog.h"

namespace {
	const int GAME_TIME_LIMIT = 300;//ゲームの制限時間

	const Vector3 PLAYER_IN_GAME_INIT_POSITION{ 0.0f,0.0f,0.0f };//プレイヤーのインゲームでの初期位置

	const Vector3 PLAYER_IN_GAME_SCALE{ 2.0f,2.0f,2.0f };//プレイヤーのインゲームでの大きさ
}

//デストラクタ
Game::~Game()
{
	DeleteGO(m_gameBackGround);
	DeleteGO(m_gameTimeLimit);
	DeleteGO(m_gameMission);
	DeleteGO(m_miniMap);
	m_player->SetGamePtr(nullptr);
}

//開始処理
bool Game::Start()
{
	m_gameBackGround = NewGO<GameBackGround>(0, "gamebackground");
	m_gameBackGround->SetGamePtr(this);
	m_player = FindGO<Player>("player");
	m_player->SetDirectPosition(PLAYER_IN_GAME_INIT_POSITION);
	m_player->SetScale(PLAYER_IN_GAME_SCALE);
	m_player->SetGamePtr(this);
	m_player->GetPlayerCamera().Reset(m_player);
	m_gameTimeLimit = NewGO<GameTimeLimit>(0, "gametimelimit");
	m_gameTimeLimit->SetTimeLimit(GAME_TIME_LIMIT);
	m_gameTimeLimit->DisableDrawingUI();
	m_gameMission = NewGO<GameMission>(0, "gamemission");
	m_gameMission->DisableDrawingUI();
	m_miniMap = NewGO<MiniMap>(0, "minimap");
	m_miniMap->SetPlayerPtr(m_player);
	m_miniMap->DisableDrawingUI();
	m_gameStartSprite = NewGO<GameStartSprite>(0, "gamestartsprite");
	m_gameStartSprite->DisableDrawingUI();
	g_renderingEngine->GetGameEndPostEffect().SetDrawingGameEndPostEffect(GameEndPostEffect::enGameEndPostEffect_None);
	return true;
}

//更新処理
void Game::Update()
{
	//ロード完了していなければ
	if (!m_isLoadComplete)
	{
		//1.5秒経過したらフェードインに移行
		if (g_gameTime->StopWatch(1.5f))
		{
			FadeManager::GetInstance()->SetFadeState(FadeManager::enFadeState_FadeIn);
			Loading::GetInstance()->FinishLoading();
			m_isLoadComplete = true;
		}
		return;
	}

	m_miniMap->Execute();//ミニマップの実行処理

	if (m_isFinishGameStartDirection != true)
	{
		if (m_isGameStartDirection)
		{
			if (FadeManager::GetInstance()->IsFinishFade())
			{
				m_isGameStartDirection = true;
			}
		}
		else
		{
			if (g_gameTime->StopWatch(1.5f))
			{
				m_isFinishGameStartDirection = true;
				m_gameTimeLimit->EnableDrawingUI();
				m_gameMission->EnableDrawingUI();
				m_miniMap->EnableDrawingUI();
				m_gameStartSprite->EnableDrawingUI();
			}
		}
		return;
	}

	auto startSprite = FindGO<GameStartSprite>("gamestartsprite");
	if (startSprite != nullptr)
	{
		m_gameStartSprite->PlaySpriteAnimation();

		if (!m_gameStartSprite->IsFinishScaleDownSpriteAnimation())
		{
			return;
		}
	}
	m_gameTimeLimit->Execute();//制限時間の実行処理
	m_gameMission->Execute();//ゲームミッションの実行処理

	//if (m_gameMission->GetCurrentCaptureEnemyNum() == m_gameMission->GetQuotaCaptureEnemyNum() && 
	//	m_player->GetCharacterController().IsOnGround())
	if(g_pad[0]->IsTrigger(enButtonX))//デバッグ用
	{
		SceneManager::GetInstance()->CreateScene(SceneManager::enSceneID_GameClear);
		m_gameTimeLimit->DisableDrawingUI();
		m_gameMission->DisableDrawingUI();
		m_miniMap->DisableDrawingUI();
		m_isGameEnd = true;
	}


	if(m_gameTimeLimit->IsTimeUp())
	//if (g_pad[0]->IsTrigger(enButtonX))//デバッグ用
	{
		SceneManager::GetInstance()->CreateScene(SceneManager::enSceneID_GameOver);
		m_gameMission->DisableDrawingUI();
		m_miniMap->DisableDrawingUI();
		m_isGameEnd = true;
	}
	DebugLog::GetInstance()->Update();
}

void Game::Render(RenderContext& rc)
{
	DebugLog::GetInstance()->Render(rc);
}