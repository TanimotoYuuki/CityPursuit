#include "stdafx.h"
#include "GameClear.h"
#include "GameClearCamera.h"

GameClear::~GameClear()
{
	DeleteGO(m_gameClearCamera);
}

bool GameClear::Start()
{
	m_gameClearCamera = NewGO<GameClearCamera>(0, "gameclearCamera");
	m_gameClearCamera->SetGameClearPtr(this);
	return true;
}

void GameClear::Update()
{
	//ゲームクリア時のカメラの実行処理
	m_gameClearCamera->Execute();

	//ゲームクリアカメラの回転が終了したらゲームクリアフラグを立てる
	if (m_gameClearCamera->IsFinishRotationCamera())
	{
		m_isGameClear = true;
	}
}