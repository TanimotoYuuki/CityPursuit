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

private://メンバ関数

	/// <summary>
	/// タイトルテキストUIの初期化
	/// </summary>
	void InitTitleTextUI();

public://メンバ関数

	/// <summary>
	/// UIを描画する
	/// </summary>
	void EnableDrawingUI()
	{
		m_isDrawingUI = true;
	}

	/// <summary>
	/// UIを描画しない
	/// </summary>
	void DisableDrawingUI()
	{
		m_isDrawingUI = false;
	}

private://メンバ変数
	SpriteRender m_titleTextUI;//タイトルテキストUI
	bool m_isDrawingUI = false;//UIを描画するか?

private://表示するUIのファイルパス用のメンバ変数

	//ゲームクリアテキストUIのファイルパス
	const std::string m_gameOverTextUIFilePath = "Assets/sprite/text/titleName.dds";
};

