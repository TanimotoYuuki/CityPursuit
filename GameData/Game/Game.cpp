#include "stdafx.h"
#include "Game.h"
#include "BackGround.h"
#include "Player.h"

//開始処理
bool Game::Start()
{
	m_backGround = NewGO<BackGround>(0, "background");
	m_player = NewGO<Player>(0, "player");
	return true;
}

//更新処理
void Game::Update()
{

}