#include "stdafx.h"
#include "GameClearSprite.h"

namespace {
	//ゲームクリアテキストUI
	const float GAME_CLEAR_TEXT_UI_WIDTH = 2048.0f;//ゲームクリアテキストUIの横幅

	const float GAME_CLEAR_TEXT_UI_HEIGHT = 128.0f;//ゲームクリアテキストUIの縦幅

	const Vector3 GAME_CLEAR_TEXT_UI_POSITION{ 0.0f,375.0f,0.0f };//ゲームクリアテキストUIの位置

	const Vector3 GAME_CLEAR_TEXT_UI_SCALE{ 0.7f,0.7f,0.7f };//ゲームクリアテキストUIの大きさ

	const Vector4 GAME_CLEAR_TEXT_UI_MULCOLOR{ 1.0f,1.0f,1.0f,0.0f };//ゲームクリアテキストUIの乗算カラー

	//アニメーション
	const float ANIMATION_PLAY_SPEED = 2.5f;//リザルトUIのアニメーションの再生速度

	const float OPAQUE_ALPHA = 1.0f;//不透明のときのα値

	const float TRANSPARENT_ALPHA = 0.0f;//透明のときのα値
}

bool GameClearSprite::Start()
{
	//ゲームクリアテキストUIの初期化
	InitGameClearTextUI();

	//透明度を変えるアニメーションのユニークポインタの初期化
	m_alphaSpriteAnimation[enSpriteAlphaState_Opaque] = std::make_unique<AlphaSpriteAnimation>(
		&m_gameClearTextUI, //アニメーションをさせるスプライト
		1.0f, //ターゲットの割合
		ANIMATION_PLAY_SPEED,//アニメーションの再生速度
		false, //ループするか?
		TRANSPARENT_ALPHA, //元の透明度
		OPAQUE_ALPHA//ターゲットの透明度
	);

	//透明度を変えるアニメーションのユニークポインタの初期化
	m_alphaSpriteAnimation[enSpriteAlphaState_Transparent] = std::make_unique<AlphaSpriteAnimation>(
		&m_gameClearTextUI, //アニメーションをさせるスプライト
		1.0f, //ターゲットの割合
		ANIMATION_PLAY_SPEED,//アニメーションの再生速度
		false, //ループするか?
		OPAQUE_ALPHA, //元の透明度
		TRANSPARENT_ALPHA//ターゲットの透明度
	);

	return true;
}

void GameClearSprite::Render(RenderContext& renderContext)
{
	//ゲームクリアテキストUIの描画
	m_gameClearTextUI.Draw(renderContext);
}

//ゲームクリアテキストUIの初期化
void GameClearSprite::InitGameClearTextUI()
{
	//ゲームクリアテキストUIの初期化
	m_gameClearTextUI.Init(m_gameClearTextUIFilePath.c_str(), GAME_CLEAR_TEXT_UI_WIDTH, GAME_CLEAR_TEXT_UI_HEIGHT);
	//ゲームクリアテキストUIの座標の設定
	m_gameClearTextUI.SetPosition(GAME_CLEAR_TEXT_UI_POSITION);
	//ゲームクリアテキストUIの大きさの設定
	m_gameClearTextUI.SetScale(GAME_CLEAR_TEXT_UI_SCALE);
	//ゲームクリアテキストUIの乗算カラーの設定
	m_gameClearTextUI.SetMulColor(GAME_CLEAR_TEXT_UI_MULCOLOR);
	//ゲームクリアテキストUIの更新処理
	m_gameClearTextUI.Update();
}

//透明度を変えるアニメーションの再生
void GameClearSprite::PlayAlphaSpriteAnimation(EnSpriteAlphaState enSpriteAlphaState)
{
	switch (enSpriteAlphaState)
	{
	case EnSpriteAlphaState::enSpriteAlphaState_Opaque:
		m_alphaSpriteAnimation[enSpriteAlphaState_Opaque]->Update();
		break;
	case EnSpriteAlphaState::enSpriteAlphaState_Transparent:
		m_alphaSpriteAnimation[enSpriteAlphaState_Transparent]->Update();
		break;
	default:
		break;
	}
}