#include "stdafx.h"
#include "Game.h"
#include "BackGround.h"

//開始処理
bool Game::Start()
{
	m_backGround = NewGO<BackGround>(0, "background");
	return true;
}

//更新処理
void Game::Update()
{

}