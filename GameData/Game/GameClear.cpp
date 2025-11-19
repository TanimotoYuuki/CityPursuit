#include "stdafx.h"
#include "GameClear.h"
#include "GameClearCamera.h"
#include "Game.h"
#include "GameClearSprite.h"
#include "GameTimeLimit.h"
#include "GameResult.h"
#include "GameEndSelect.h"

//デストラクタ
GameClear::~GameClear()
{
	DeleteGO(m_gameClearCamera);
	DeleteGO(m_gameClearSprite);
	DeleteGO(m_gameResult);
	DeleteGO(m_gameEndSelect);
}

//開始処理
bool GameClear::Start()
{
	m_gameClearCamera = NewGO<GameClearCamera>(0, "gameclearcamera");
	m_gameClearCamera->SetGameClearPtr(this);

	m_gameClearSprite = NewGO<GameClearSprite>(0, "gameclearsprite");

	m_gameResult = NewGO<GameResult>(0, "gameresult");
	m_gameResult->SetQteEventSuccessCount(m_game->GetQteEventSuccessCount());
	m_gameResult->SetQteEventFailedCount(m_game->GetQteEventFailedCount());
	m_gameResult->SetGameClearTimeLimit(m_game->GetGameTimeLimitPtr()->GetTimeLimit());

	m_gameEndSelect = NewGO<GameEndSelect>(0,"gameendselect");

	return true;
}

//更新処理
void GameClear::Update()
{
	//ゲームクリアの状態
	switch (m_gameClearState)
	{
	case GameClear::enGameClearState_GameClearDirection://ゲームクリア演出
		GameClearDirectionUpdate();
		break;
	case GameClear::enGameClearState_Result://リザルト
		ResultUpdate();
		break;
	case GameClear::enGameClearState_Select://選択
		SelectUpdate();
		break;
	default:
		break;
	}
}

//ゲームクリア演出の更新処理
void GameClear::GameClearDirectionUpdate()
{
	//ゲームクリア時のカメラの実行処理
	m_gameClearCamera->Execute();

	//ゲームクリアカメラの回転が終了したらゲームクリアのスプライトを表示させる
	if (m_gameClearCamera->IsFinishRotationCamera())
	{
		m_gameClearSprite->PlayAlphaSpriteAnimation(GameClearSprite::enSpriteAlphaState_Opaque);
	}

	//ゲームクリアカメラの移動が終了したら次のステートに移行する
	if (m_gameClearCamera->IsFinishMoveCamera())
	{
		m_gameClearState = enGameClearState_Result;
	}
}

//ゲームリザルトの更新処理
void GameClear::ResultUpdate()
{
	m_gameResult->Execute();

	//透明度(不透明から透明)を変えるアニメーションを再生していたらゲームクリアのスプライトを非表示させる
	if (m_gameResult->IsPlayNoneAlphaAnimation())
	{
		m_gameClearSprite->PlayAlphaSpriteAnimation(GameClearSprite::enSpriteAlphaState_Transparent);
		g_renderingEngine->GetGameEndPostEffect().SetDrawingGameEndPostEffect(GameEndPostEffect::enGameEndPostEffect_Boke);
	}

	//透明度(不透明から透明)を変えるアニメーションを再生終了していたら次のステートに移行する
	if (m_gameResult->IsFinishNoneAlphaAnimation())
	{
		m_gameEndSelect->EnableDrawingUI();
		m_gameClearState = enGameClearState_Select;
	}
}

//選択の更新処理
void GameClear::SelectUpdate()
{
	m_gameEndSelect->Execute();

	//シーンを遷移するならタイトルの削除処理する
	if (m_gameEndSelect->IsTransitionScene())
	{
		DeleteGO(m_game);
		DeleteGO(this);
	}
}