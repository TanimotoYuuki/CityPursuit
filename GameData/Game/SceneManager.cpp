#include "stdafx.h"
#include "SceneManager.h"
#include "Game.h"
#include "GameClear.h"
#include "Player.h"
#include "PlayerAnimation.h"

SceneManager* SceneManager::m_instance = nullptr;//初期化

//開始処理
bool SceneManager::Start()
{
	//最初のシーンを設定
	SceneManager::GetInstance()->CreateScene(enSceneID_InGame);
	return true;
}

//更新処理
void SceneManager::Update()
{
	//生成するシーンが現在のシーンと同じなら処理しない
	if (m_createScene == m_currentScene)
	{
		return;
	}

	switch (m_createScene)
	{
	case enSceneID_InGame://インゲームシーン
		m_game = NewGO<Game>(0, "game");
		m_currentScene = enSceneID_InGame;
		break;
	case enSceneID_GameClear://ゲームクリアシーン
		m_gameClear = NewGO<GameClear>(0, "gameclear");
		m_gameClear->SetPlayerPtr(m_game->GetPlayerPtr());
		m_game->GetPlayerPtr()->GetPlayerAnimation()->SetGameClearPtr(m_gameClear);
		m_currentScene = enSceneID_GameClear;
		break;
	default:
		break;
	}

	m_createScene = enSceneID_None;//シーンの生成完了
}