#pragma once
#include "SpriteAnimation.h"
/// <summary>
/// ゲームスタートのスプライトを表示するクラス
/// </summary>
class GameStartSprite : public IGameObject
{
public:
	GameStartSprite() {};//コンストラクタ
	~GameStartSprite() {};//デストラクタ

	bool Start();//開始処理
	void Render(RenderContext& rc);//描画処理

public://メンバ変数

	/// <summary>
	/// ゲームスタートテキストUIの初期化
	/// </summary>
	void InitGameStartTextUI();

public://メンバ関数

	/// <summary>
	/// スプライトのアニメーションの再生
	/// </summary>
	void PlaySpriteAnimation();

	/// <summary>
	/// 大きさ(スケールダウン)を変えるアニメーションの再生が終わったか?
	/// </summary>
	/// <param name="enSpriteAlphaState">trueなら大きさ(スケールダウン)を変えるアニメーションの再生が終わっている</param>
	bool IsFinishScaleDownSpriteAnimation()
	{
		return m_scaleDownSpriteAnimation->IsCompleted();
	}

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
	SpriteRender m_gameStartTextUI;//ゲームスタートテキストUI
	bool m_isDrawingUI = false;//UIを描画するか?
	std::unique_ptr<AlphaSpriteAnimation> m_alphaSpriteAnimation;//透明度を変えるアニメーションのユニークポインタ
	std::unique_ptr<ScaleSpriteAnimation> m_scaleDownSpriteAnimation;//大きさ(スケールダウン)を変えるアニメーションのユニークポインタ
	std::unique_ptr<ScaleSpriteAnimation> m_scaleUpSpriteAnimation;//大きさを(スケールアップ)変えるアニメーションのユニークポインタ

private://表示するUIのファイルパス用のメンバ変数

	//ゲームスタートテキストUIのファイルパス
	const std::string m_gameStartTextUIFilePath = "Assets/sprite/text/missionStart.dds";
};

