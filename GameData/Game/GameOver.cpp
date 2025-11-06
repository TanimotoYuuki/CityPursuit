#include "stdafx.h"
#include "GameOver.h"
#include "Game.h"
#include "GameTimeLimit.h"
#include "GameOverSprite.h"
#include "GameEndSelect.h"

//デストラクタ
GameOver::~GameOver()
{
	DeleteGO(m_gameOverSprite);
	DeleteGO(m_gameEndSelect);
}

//開始処理
bool GameOver::Start()
{
	m_gameOverSprite = NewGO<GameOverSprite>(0, "gameovertext");
	m_gameEndSelect = NewGO<GameEndSelect>(0, "gameendselect");
	return true;
}

//更新処理
void GameOver::Update()
{
	//ゲームオーバーの状態
	switch (m_gameOverState)
	{
	case enGameOverState_TimeLimit://時間切れ
		TimeLimitUpdate();
		break;
	case enGameOverState_Monochrome://モノクロ化
		MonochromeUpdate();
		break;
	case enGameOverState_GameOverText://ゲームオーバーのテキスト
		GameOverTextUpdate();
		break;
	case enGameOverState_Select://選択
		SelectUpdate();
		break;
	default:
		break;
	}
}

//時間切れの更新処理
void GameOver::TimeLimitUpdate()
{
	//時間切れの演出が終わったら次のステートに移行する
	if (m_game->GetGameTimeLimitPtr()->IsFinishDirection())
	{
		m_game->GetGameTimeLimitPtr()->DisableDrawingUI();
		m_gameOverState = enGameOverState_Monochrome;
	}
}

//モノクロ化の更新処理
void GameOver::MonochromeUpdate()
{
	g_renderingEngine->GetGameEndPostEffect().SetDrawingGameEndPostEffect(GameEndPostEffect::enGameEndPostEffect_Monochrome);

	//モノクロ化が終わったら次のステートに移行する
	if (g_renderingEngine->GetGameEndPostEffect().IsFinishDrawingGameEndPostEffect())
	{
		m_gameOverState = enGameOverState_GameOverText;
	}
}

//ゲームオーバーのテキストの更新処理
void GameOver::GameOverTextUpdate()
{
	m_gameOverSprite->PlaySpriteAnimation();

	//スプライトのアニメーションの再生が終わったら次のステートに移行する
	if (m_gameOverSprite->IsFinishSpriteAnimation())
	{
		m_gameEndSelect->EnableDrawingUI();
		m_gameOverState = enGameOverState_Select;
	}
}

//選択の更新処理
void GameOver::SelectUpdate()
{
	m_gameEndSelect->Execute();
}