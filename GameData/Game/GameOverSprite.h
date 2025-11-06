#pragma once
#include "SpriteAnimation.h"
/// <summary>
/// ゲームオーバーのスプライトを表示するクラス
/// </summary>
class GameOverSprite : public IGameObject
{
public:
	GameOverSprite() {};//コンストラクタ
	~GameOverSprite() {};//デストラクタ

	bool Start();//開始処理
	void Render(RenderContext& rc);//描画処理

private://メンバ関数

	/// <summary>
	/// ゲームオーバーテキストUIの初期化
	/// </summary>
	void InitGameOverTextUI();

public://メンバ関数

	/// <summary>
	/// スプライトのアニメーションの再生
	/// </summary>
	/// <param name="enSpriteAlphaState">スプライトの透明度の状態</param>
	void PlaySpriteAnimation();

	/// <summary>
	/// スプライトのアニメーションの再生が終わったか?
	/// </summary>
	/// <param name="enSpriteAlphaState">trueならスプライトのアニメーションの再生が終わっている</param>
	bool IsFinishSpriteAnimation()
	{
		return m_positionSpriteAnimation->IsCompleted();
	}

private://メンバ変数
	SpriteRender m_gameOverTextUI;//ゲームクリアテキストUI
	std::unique_ptr<AlphaSpriteAnimation> m_alphaSpriteAnimation;//透明度を変えるアニメーションのユニークポインタ
	std::unique_ptr<PositionSpriteAnimation> m_positionSpriteAnimation;//位置を変えるアニメーションのユニークポインタ

private://表示するUIのファイルパス用のメンバ変数

	//ゲームクリアテキストUIのファイルパス
	const std::string m_gameOverTextUIFilePath = "Assets/sprite/text/missionFailed.dds";
};

