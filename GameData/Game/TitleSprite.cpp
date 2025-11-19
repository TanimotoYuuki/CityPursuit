#include "stdafx.h"
#include "TitleSprite.h"

namespace {
	//タイトルテキストUI
	const float TITLE_TEXT_UI_WIDTH = 1024.0f;//タイトルテキストUIの横幅

	const float TITLE_TEXT_UI_HEIGHT = 256.0f;//タイトルテキストUIの縦幅

	const Vector3 TITLE_TEXT_UI_POSITION{ 0.0f,125.0f,0.0f };//タイトルテキストUIの位置

	const Vector3 TITLE_TEXT_UI_SCALE{ 1.1f,1.1f,1.1f };//タイトルテキストUIの大きさ

	//スタートテキストUI
	const float START_TEXT_UI_WIDTH = 1024.0f;//スタートテキストUIの横幅

	const float START_TEXT_UI_HEIGHT = 128.0f;//スタートテキストUIの縦幅

	const Vector3 START_TEXT_UI_POSITION{ 0.0f,-125.0f,0.0f };//スタートテキストUIの位置

	const Vector3 START_TEXT_UI_SCALE{ 0.8f, 0.8f, 0.8f };//スタートテキストUIの大きさ
}

//開始処理
bool TitleSprite::Start()
{
	//タイトルテキストUIの初期化
	InitTitleTextUI();

	//スタートテキストUIの初期化
	InitStartUIText();

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

	//スタートテキストUIの描画
	m_startTextUI.Draw(renderContext);
}

//タイトルテキストUIの初期化
void TitleSprite::InitTitleTextUI()
{
	//タイトルテキストUIの初期化
	m_titleTextUI.Init(m_titleTextUIFilePath.c_str(), TITLE_TEXT_UI_WIDTH, TITLE_TEXT_UI_HEIGHT);
	//タイトルテキストUIの座標の設定
	m_titleTextUI.SetPosition(TITLE_TEXT_UI_POSITION);
	//タイトルテキストUIの大きさの設定
	m_titleTextUI.SetScale(TITLE_TEXT_UI_SCALE);
	//タイトルテキストUIの更新処理
	m_titleTextUI.Update();
}

//スタートテキストUIの初期化
void TitleSprite::InitStartUIText()
{
	//スタートテキストUIの初期化
	m_startTextUI.Init(m_startTextUIFilePath.c_str(), START_TEXT_UI_WIDTH, START_TEXT_UI_HEIGHT);
	//スタートテキストUIの座標の設定
	m_startTextUI.SetPosition(START_TEXT_UI_POSITION);
	//スタートテキストUIの大きさの設定
	m_startTextUI.SetScale(START_TEXT_UI_SCALE);
	//スタートテキストUIの更新処理
	m_startTextUI.Update();
}