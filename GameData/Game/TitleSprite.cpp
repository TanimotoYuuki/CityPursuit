#include "stdafx.h"
#include "TitleSprite.h"

namespace {
	//ゲームオーバーテキストUI
	const float TITLE_TEXT_UI_WIDTH = 1024.0f;//タイトルテキストUIの横幅

	const float TITLE_TEXT_UI_HEIGHT = 256.0f;//タイトルテキストUIの縦幅

	const Vector3 TITLE_TEXT_UI_POSITION{ 0.0f,125.0f,0.0f };//タイトルテキストUIの位置

	const Vector3 TITLE_TEXT_UI_SCALE{ 1.1f,1.1f,1.1f };//タイトルテキストUIの大きさ
}

//開始処理
bool TitleSprite::Start()
{
	//タイトルテキストUIの初期化
	InitTitleTextUI();
	return true;
}

//描画処理
void TitleSprite::Render(RenderContext& renderContext)
{
	//UIを描画しないなら処理しない
	if (!m_isDrawingUI)
	{
		return;
	}

	//タイトルテキストUIの描画
	m_titleTextUI.Draw(renderContext);
}

//タイトルテキストUIの初期化
void TitleSprite::InitTitleTextUI()
{
	//タイトルテキストUIの初期化
	m_titleTextUI.Init(m_gameOverTextUIFilePath.c_str(), TITLE_TEXT_UI_WIDTH, TITLE_TEXT_UI_HEIGHT);
	//タイトルテキストUIの座標の設定
	m_titleTextUI.SetPosition(TITLE_TEXT_UI_POSITION);
	//タイトルテキストUIの大きさの設定
	m_titleTextUI.SetScale(TITLE_TEXT_UI_SCALE);
	//タイトルテキストUIの更新処理
	m_titleTextUI.Update();
}