#include "stdafx.h"
#include "Game.h"
#include "BackGround.h"
#include "Player.h"
#include "Enemy.h"
#include "SceneManager.h"
#include "DebugLog.h"

//開始処理
bool Game::Start()
{
	//m_debugLog = NewGO<DebugLog>(0, "debuglog");
	m_backGround = NewGO<BackGround>(0, "background");
	m_player = NewGO<Player>(0, "player");
	m_player->SetGamePtr(this);
	return true;
}

//更新処理
void Game::Update()
{
	auto enemy = FindGO<Enemy>("enemy");
	if (enemy == nullptr && m_player->GetCharacterController().IsOnGround())
	{
		SceneManager::GetInstance()->CreateScene(SceneManager::enSceneID_GameClear);
		m_isGameClear = true;
	}
	DebugLog::GetInstance()->Update();
}

void Game::Render(RenderContext& rc)
{
	DebugLog::GetInstance()->Render(rc);
}