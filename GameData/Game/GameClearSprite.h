#pragma once
#include "SpriteAnimation.h"
/// <summary>
/// ゲームクリアのスプライトを表示するクラス
/// </summary>
class GameClearSprite : public IGameObject
{
public:
	GameClearSprite() {};//コンストラクタ
	~GameClearSprite() {};//デストラクタ

	bool Start();//開始処理
	void Render(RenderContext& rc);//描画処理

public://列挙型

	//スプライトの透明度の状態
	enum EnSpriteAlphaState
	{
		enSpriteAlphaState_Opaque,//不透明
		enSpriteAlphaState_Transparent,//透明
		enSpriteAlphaState_Num//状態数
	};

private://メンバ関数

	/// <summary>
	/// ゲームクリアテキストUIの初期化
	/// </summary>
	void InitGameClearTextUI();

public://メンバ関数

	/// <summary>
	/// 透明度を変えるアニメーションの再生
	/// </summary>
	/// <param name="enSpriteAlphaState">スプライトの透明度の状態</param>
	void PlayAlphaSpriteAnimation(EnSpriteAlphaState enSpriteAlphaState);

	/// <summary>
	/// 透明度を変えるアニメーションの再生が終わったか?
	/// </summary>
	/// <param name="enSpriteAlphaState">trueなら透明度を変えるアニメーションの再生が終わっている</param>
	bool IsFinishAlphaSpriteAnimation(EnSpriteAlphaState enSpriteAlphaState)
	{
		return m_alphaSpriteAnimation[enSpriteAlphaState]->IsCompleted();
	}

private://メンバ変数
	SpriteRender m_gameClearTextUI;//ゲームクリアテキストUI
	std::unique_ptr<AlphaSpriteAnimation> m_alphaSpriteAnimation[enSpriteAlphaState_Num];//透明度を変えるアニメーションのユニークポインタ

private://表示するUIのファイルパス用のメンバ変数

	//ゲームクリアテキストUIのファイルパス
	const std::string m_gameClearTextUIFilePath = "Assets/sprite/text/missionComplete.dds";
};

