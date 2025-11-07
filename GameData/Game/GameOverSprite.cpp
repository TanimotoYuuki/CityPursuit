#include "stdafx.h"
#include "GameOverSprite.h"

namespace {
	//ゲームオーバーテキストUI
	const float GAME_OVER_TEXT_UI_WIDTH = 2048.0f;//ゲームオーバーテキストUIの横幅

	const float GAME_OVER_TEXT_UI_HEIGHT = 128.0f;//ゲームオーバーテキストUIの縦幅

	const Vector3 GAME_OVER_TEXT_UI_POSITION{ 0.0f,575.0f,0.0f };//ゲームオーバーテキストUIの位置

	const Vector3 GAME_OVER_TEXT_UI_SCALE{ 0.7f,0.7f,0.7f };//ゲームオーバーテキストUIの大きさ

	const Vector4 GAME_OVER_TEXT_UI_MULCOLOR{ 1.0f,1.0f,1.0f,0.0f };//ゲームオーバーテキストUIの乗算カラー

	//アニメーション
	const float ANIMATION_PLAY_SPEED = 1.2f;//リザルトUIのアニメーションの再生速度

	const float OPAQUE_ALPHA = 1.0f;//不透明のときのα値

	const Vector3 AFTER_ANIMATION_POSITION{ 0.0f,375.0f,0.0f };//ゲームオーバーテキストUIの位置
}

//開始処理
bool GameOverSprite::Start()
{
	//ゲームオーバーテキストUIの初期化
	InitGameOverTextUI();

	//透明度を変えるアニメーションのユニークポインタの初期化
	m_alphaSpriteAnimation = std::make_unique<AlphaSpriteAnimation>(
		&m_gameOverTextUI, //アニメーションをさせるスプライト
		1.0f, //ターゲットの割合
		ANIMATION_PLAY_SPEED,//アニメーションの再生速度
		false, //ループするか?
		m_gameOverTextUI.GetMulColor().a, //元の透明度
		OPAQUE_ALPHA//ターゲットの透明度
	);

	//位置を変えるアニメーションのユニークポインタの初期化
	m_positionSpriteAnimation = std::make_unique<PositionSpriteAnimation>(
		&m_gameOverTextUI, //アニメーションをさせるスプライト
		1.0f, //ターゲットの割合
		ANIMATION_PLAY_SPEED,//アニメーションの再生速度
		false, //ループするか?
		m_gameOverTextUI.GetPosition(), //元の位置
		AFTER_ANIMATION_POSITION//ターゲットの位置
	);

	return true;
}

//描画処理
void GameOverSprite::Render(RenderContext& renderContext)
{
	//ゲームオーバーテキストUIの描画
	m_gameOverTextUI.Draw(renderContext);
}

//ゲームオーバーテキストUIの初期化
void GameOverSprite::InitGameOverTextUI()
{
	//ゲームオーバーテキストUIの初期化
	m_gameOverTextUI.Init(m_gameOverTextUIFilePath.c_str(), GAME_OVER_TEXT_UI_WIDTH, GAME_OVER_TEXT_UI_HEIGHT);
	//ゲームオーバーテキストUIの座標の設定
	m_gameOverTextUI.SetPosition(GAME_OVER_TEXT_UI_POSITION);
	//ゲームオーバーテキストUIの大きさの設定
	m_gameOverTextUI.SetScale(GAME_OVER_TEXT_UI_SCALE);
	//ゲームオーバーテキストUIの乗算カラーの設定
	m_gameOverTextUI.SetMulColor(GAME_OVER_TEXT_UI_MULCOLOR);
	//ゲームオーバーテキストUIの更新処理
	m_gameOverTextUI.Update();
}

//スプライトのアニメーションの再生
void GameOverSprite::PlaySpriteAnimation()
{
	m_alphaSpriteAnimation->Update();
	m_positionSpriteAnimation->Update();
}