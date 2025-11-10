#include "stdafx.h"
#include "Game.h"
#include "BackGround.h"
#include "Player.h"
#include "Enemy.h"
#include "GameTimeLimit.h"
#include "GameMission.h"
#include "GameStartSprite.h"
#include "SceneManager.h"
#include "FadeManager.h"
#include "DebugLog.h"

namespace {
	const int GAME_TIME_LIMIT = 300;//ゲームの制限時間
}

//開始処理
bool Game::Start()
{
	//m_debugLog = NewGO<DebugLog>(0, "debuglog");
	m_backGround = NewGO<BackGround>(0, "background");
	m_backGround->SetGamePtr(this);
	m_player = NewGO<Player>(0, "player");
	m_player->SetGamePtr(this);
	m_gameTimeLimit = NewGO<GameTimeLimit>(0, "gametimelimit");
	m_gameTimeLimit->SetTimeLimit(GAME_TIME_LIMIT);
	m_gameTimeLimit->DisableDrawingUI();
	m_gameMission = NewGO<GameMission>(0, "gamemission");
	m_gameMission->DisableDrawingUI();
	m_gameStartSprite = NewGO<GameStartSprite>(0, "gamestartsprite");
	m_gameStartSprite->DisableDrawingUI();
	FadeManager::GetInstance()->SetFadeState(FadeManager::enFadeState_FadeIn);
	return true;
}

//更新処理
void Game::Update()
{
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

	if (m_gameMission->GetCurrentCaptureEnemyNum() == m_gameMission->GetQuotaCaptureEnemyNum() && 
		m_player->GetCharacterController().IsOnGround())
	//if(g_pad[0]->IsTrigger(enButtonX))//デバッグ用
	{
		SceneManager::GetInstance()->CreateScene(SceneManager::enSceneID_GameClear);
		m_gameTimeLimit->DisableDrawingUI();
		m_gameMission->DisableDrawingUI();
		m_isGameEnd = true;
	}


	if(m_gameTimeLimit->IsTimeUp())
	//if (g_pad[0]->IsTrigger(enButtonX))//デバッグ用
	{
		SceneManager::GetInstance()->CreateScene(SceneManager::enSceneID_GameOver);
		m_gameMission->DisableDrawingUI();
		m_isGameEnd = true;
	}
	DebugLog::GetInstance()->Update();
}

void Game::Render(RenderContext& rc)
{
	DebugLog::GetInstance()->Render(rc);
}