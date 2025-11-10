#include "stdafx.h"
#include "GameStartSprite.h"

namespace {
	//ゲームスタートテキストUI
	const float GAME_START_TEXT_UI_WIDTH = 2048.0f;//ゲームスタートテキストUIの横幅

	const float GAME_START_TEXT_UI_HEIGHT = 128.0f;//ゲームスタートテキストUIの縦幅

	const Vector3 GAME_START_TEXT_UI_POSITION{ 0.0f,125.0f,0.0f };//ゲームスタートテキストUIの位置

	const Vector3 GAME_START_TEXT_UI_SCALE{ 0.7f,0.7f,0.7f };//ゲームスタートテキストUIの大きさ

	const Vector4 GAME_START_TEXT_UI_MULCOLOR{ 1.0f,1.0f,1.0f,1.0f };//ゲームスタートテキストUIの乗算カラー

	//アニメーション
	const float ANIMATION_PLAY_SPEED = 1.2f;//リザルトUIのアニメーションの再生速度

	const float TRANSPARENT_ALPHA = 0.0f;//透明のときのα値

	const Vector2 AFTER_SCALE_DOWN_ANIMATION_SCALE{ 0.4f,0.4f };//スケールダウンアニメーションの再生後のゲームスタートテキストUIの大きさ

	const Vector2 AFTER_SCALE_UP_ANIMATION_SCALE{ 1.2f,1.2f };//スケールアップアニメーションの再生後のゲームスタートテキストUIの大きさ
}

//開始処理
bool GameStartSprite::Start()
{
	//ゲームスタートテキストUIの初期化
	InitGameStartTextUI();

	//透明度を変えるアニメーションのユニークポインタの初期化
	m_alphaSpriteAnimation = std::make_unique<AlphaSpriteAnimation>(
		&m_gameStartTextUI, //アニメーションをさせるスプライト
		1.0f, //ターゲットの割合
		ANIMATION_PLAY_SPEED,//アニメーションの再生速度
		false, //ループするか?
		m_gameStartTextUI.GetMulColor().a, //元の透明度
		TRANSPARENT_ALPHA//ターゲットの透明度
	);

	Vector2 gameStartTextUIScale = Vector2{ 
		m_gameStartTextUI.GetScale().x ,
		m_gameStartTextUI.GetScale().y 
	};

	//大きさ(スケールダウン)を変えるアニメーションのユニークポインタの初期化
	m_scaleDownSpriteAnimation = std::make_unique<ScaleSpriteAnimation>(
		&m_gameStartTextUI, //アニメーションをさせるスプライト
		1.0f, //ターゲットの割合
		ANIMATION_PLAY_SPEED,//アニメーションの再生速度
		false, //ループするか?
		gameStartTextUIScale, //元の大きさ
		AFTER_SCALE_DOWN_ANIMATION_SCALE//ターゲットの大きさ
	);

	//大きさ(スケールアップ)を変えるアニメーションのユニークポインタの初期化
	m_scaleUpSpriteAnimation = std::make_unique<ScaleSpriteAnimation>(
		&m_gameStartTextUI, //アニメーションをさせるスプライト
		1.0f, //ターゲットの割合
		ANIMATION_PLAY_SPEED,//アニメーションの再生速度
		false, //ループするか?
		AFTER_SCALE_DOWN_ANIMATION_SCALE, //元の大きさ
		AFTER_SCALE_UP_ANIMATION_SCALE//ターゲットの大きさ
	);

	return true;
}

//描画処理
void GameStartSprite::Render(RenderContext& renderContext)
{
	//UIを描画しないなら処理しない
	if (!m_isDrawingUI)
	{
		return;
	}

	//ゲームスタートテキストUIの描画
	m_gameStartTextUI.Draw(renderContext);
}

//ゲームスタートテキストUIの初期化
void GameStartSprite::InitGameStartTextUI()
{
	//ゲームスタートテキストUIの初期化
	m_gameStartTextUI.Init(m_gameStartTextUIFilePath.c_str(), GAME_START_TEXT_UI_WIDTH, GAME_START_TEXT_UI_HEIGHT);
	//ゲームスタートテキストUIの座標の設定
	m_gameStartTextUI.SetPosition(GAME_START_TEXT_UI_POSITION);
	//ゲームスタートテキストUIの大きさの設定
	m_gameStartTextUI.SetScale(GAME_START_TEXT_UI_SCALE);
	//ゲームスタートテキストUIの乗算カラーの設定
	m_gameStartTextUI.SetMulColor(GAME_START_TEXT_UI_MULCOLOR);
	//ゲームスタートテキストUIの更新処理
	m_gameStartTextUI.Update();
}

//スプライトのアニメーションの再生
void GameStartSprite::PlaySpriteAnimation()
{
	if (!m_scaleDownSpriteAnimation->IsCompleted())
	{
		m_scaleDownSpriteAnimation->Update();
	}
	else
	{
		m_alphaSpriteAnimation->Update();
		m_scaleUpSpriteAnimation->Update();
	}

	if (m_scaleUpSpriteAnimation->IsCompleted())
	{
		DeleteGO(this);
	}
}