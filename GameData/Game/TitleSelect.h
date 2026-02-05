#pragma once
#include "SpriteAnimation.h"
/// <summary>
/// タイトルのときに選択するクラス
/// </summary>
class TitleSelect : public IGameObject
{
public:
	TitleSelect() {};//コンストラクタ
	~TitleSelect() {};//デストラクタ

	bool Start();//開始処理
	void Render(RenderContext& rc);//描画処理

public://列挙型

	//タイトルのときの選択
	enum EnTitleSelect
	{
		enTitleSelect_GameStart,//ゲームスタート
		enTitleSelect_HowToPlay,//遊び方
		enTitleSelect_GameFinish,//ゲーム終了
		enTitleSelect_Num//選択数
	};

private://メンバ関数

	/// <summary>
	/// 選択テキストUIの初期化
	/// </summary>
	/// <param name="enGameClearSelect">タイトルのときの選択</param>
	void InitSelectTextUI(EnTitleSelect enGameEndSelect);

	/// <summary>
	/// 現在何を選択しているかを表すUIの初期化
	/// </summary>
	void InitCurrentSelectUI();

	/// <summary>
	/// 現在何を選択しているかを表すUIの更新処理
	/// </summary>
	/// <param name="enGameClearSelect">タイトルのときの選択</param>
	void CurrentSelectUIUpdate(EnTitleSelect enGameEndSelect);

	/// <summary>
	/// 入力の更新処理
	/// </summary>
	void InputUpdate();

	/// <summary>
	/// 現在何を選択しているかを表すUIのアニメーションの更新処理
	/// </summary>
	void CurrentSelectUIAnimationUpdate();

	/// <summary>
	/// 大きさを変えるアニメーションのリセット処理
	/// </summary>
	void ResetScaleAnimation() 
	{
		m_currentSelectUIScaleDownAnimation->Reset();
		m_currentSelectUIScaleUpAnimation->Reset();
	}

	/// <summary>
	/// 色を変えるアニメーションのリセット処理
	/// </summary>
	void ResetColorAnimation()
	{
		m_currentSelectUIPushDownAnimation->Reset();
		m_currentSelectUIPushUpAnimation->Reset();
	}

public://メンバ関数

	/// <summary>
	/// 実行処理
	/// </summary>
	void Execute();

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

	/// <summary>
	/// リセット処理
	/// </summary>
	void Reset();

	/// <summary>
	/// 選択できたか?
	/// </summary>
	/// <returns>trueなら選択できている</returns>
	bool IsSelect()
	{
		return m_isSelect;
	}

	/// <summary>
	/// 選択したときの演出が終わったか?
	/// </summary>
	/// <returns>trueなら選択したときの演出が終わっている</returns>
	bool IsFinishSelectDecisionDirection() const
	{
		return m_isFinishSelectDecisionDirection;
	}

	/// <summary>
	/// 現在何を選択しているかを表すための変数の取得
	/// </summary>
	/// <returns></returns>
	EnTitleSelect GetCurrentSelect()
	{
		return (EnTitleSelect)m_currentSelect;
	}

private://メンバ変数
	SpriteRender m_selectTextUI[enTitleSelect_Num];//選択テキストUI
	SpriteRender m_currentSelectUI;//現在何を選択しているか表すUI
	Vector3 m_selectTextUIPosition[enTitleSelect_Num];//選択テキストUIの座標
	Vector3 m_currentSelectUIPosition[enTitleSelect_Num];//現在何を選択しているか表すUI
	Vector3 m_defaultCurrentSelectUIPosition[enTitleSelect_Num];//現在何を選択しているか表すUIの初期座標
	Vector3 m_currentSelectUIScale = Vector3::Zero;//現在何を選択しているか表すUIの大きさ
	int m_currentSelect = enTitleSelect_GameStart;//現在何を選択しているかを表すための変数
	int m_previousSelect = -1;//前に何を選択していたかを表すための変数
	float m_time = 0.0f;//時間
	bool m_isSelect = false;//選択できたか?
	bool m_isDrawingUI = false;//UIを描画するか?
	bool m_isFinishSelectDecisionDirection = false;//選択したときの演出が終わったか?
	std::unique_ptr<ScaleSpriteAnimation> m_currentSelectUIScaleDownAnimation;//何を選択しているか表すUIの大きさを縮小するアニメーション
	std::unique_ptr<ScaleSpriteAnimation> m_currentSelectUIScaleUpAnimation;//何を選択しているか表すUIの大きさを拡大するアニメーション
	std::unique_ptr<ColorSpriteAnimation> m_currentSelectUIPushDownAnimation;//何を選択しているか表すUIで押されたときのアニメーション
	std::unique_ptr<ColorSpriteAnimation> m_currentSelectUIPushUpAnimation;//何を選択しているか表すUIで離したときのアニメーション

private://表示するUIのファイルパス用のメンバ変数

	//選択テキストUIのファイルパス
	const std::string m_selectTextUIFilePath[enTitleSelect_Num] = {
		"Assets/sprite/text/gameStart.dds",
		"Assets/sprite/text/howToPlay.dds",
		"Assets/sprite/text/gameFinish.dds",
	};

	//現在何を選択しているかを表すUIのファイルパス
	const std::string m_currentSelectUIFilePath = "Assets/sprite/select/select.dds";
};

