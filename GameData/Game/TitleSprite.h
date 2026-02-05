#pragma once
#include "SpriteAnimation.h"
/// <summary>
/// タイトルのスプライトを表示するクラス
/// </summary>
class TitleSprite : public IGameObject
{
public:
	TitleSprite() {};//コンストラクタ
	~TitleSprite() {};//デストラクタ

	bool Start();//開始処理
	void Render(RenderContext& rc);//描画処理

public://列挙型

	//タイトルで描画するUIの種類
	enum EnDrawingUI
	{
		enDrawingUI_TextUI,//テキストUI
		enDrawingUI_HowToPlayUI,//遊び方UI
		enDrawingUI_Num//UIの種類
	};

private://メンバ関数

	/// <summary>
	/// タイトルテキストUIの初期化
	/// </summary>
	void InitTitleTextUI();

	/// <summary>
	/// スタートテキストUIの初期化
	/// </summary>
	void InitStartUIText();

	/// <summary>
	/// 遊び方UIの初期化
	/// </summary>
	void InitHowToPlayUI();

public://メンバ関数

	/// <summary>
	/// UIを描画する
	/// </summary>
	void EnableDrawingUI(EnDrawingUI drawingUI)
	{
		m_isDrawingUI[drawingUI] = true;
	}

	/// <summary>
	/// UIを描画しない
	/// </summary>
	void DisableDrawingUI(EnDrawingUI drawingUI)
	{
		m_isDrawingUI[drawingUI] = false;
	}

private://メンバ変数
	SpriteRender m_titleTextUI;//タイトルテキストUI
	SpriteRender m_startTextUI;//スタートテキストUI
	SpriteRender m_howToPlayUI;//遊び方テキストUI
	bool m_isDrawingUI[enDrawingUI_Num] = { false };//UIを描画するか?

private://表示するUIのファイルパス用のメンバ変数

	//タイトルテキストUIのファイルパス
	const std::string m_titleTextUIFilePath = "Assets/sprite/text/titleName.dds";

	//スタートテキストUIのファイルパス
	const std::string m_startTextUIFilePath = "Assets/sprite/text/start_abutton.dds";

	//遊び方テキストUIのファイルパス
	const std::string m_howToPlayUIFilePath = "Assets/sprite/howToPlay/howToPlay.dds";
};

