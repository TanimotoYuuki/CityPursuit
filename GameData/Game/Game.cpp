#include "stdafx.h"
#include "Game.h"
#include "BackGround.h"
#include "Player.h"
#include "Enemy.h"
#include "GameTimeLimit.h"
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
	m_player = NewGO<Player>(0, "player");
	m_player->SetGamePtr(this);
	m_gameTimeLimit = NewGO<GameTimeLimit>(0, "gametimelimit");
	m_gameTimeLimit->SetTimeLimit(GAME_TIME_LIMIT);
	m_gameTimeLimit->EnableDrawingUI();
	FadeManager::GetInstance()->SetFadeState(FadeManager::enFadeState_FadeIn);
	return true;
}

//更新処理
void Game::Update()
{
	m_gameTimeLimit->Execute();//制限時間の実行処理
	auto enemy = FindGO<Enemy>("enemy");
	if (enemy == nullptr && m_player->GetCharacterController().IsOnGround())
	//if(g_pad[0]->IsTrigger(enButtonX))//デバッグ用
	{
		SceneManager::GetInstance()->CreateScene(SceneManager::enSceneID_GameClear);
		m_gameTimeLimit->DisableDrawingUI();
		m_isGameEnd = true;
	}

	if(m_gameTimeLimit->IsTimeUp())
	//if (g_pad[0]->IsTrigger(enButtonX))//デバッグ用
	{
		SceneManager::GetInstance()->CreateScene(SceneManager::enSceneID_GameOver);
		m_isGameEnd = true;
	}
	DebugLog::GetInstance()->Update();
}

void Game::Render(RenderContext& rc)
{
	DebugLog::GetInstance()->Render(rc);
}