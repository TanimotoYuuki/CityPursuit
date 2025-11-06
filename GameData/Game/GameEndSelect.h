#pragma once
/// <summary>
/// ゲーム終了したときに選択するクラス
/// </summary>
class GameEndSelect : public IGameObject
{
public:
	GameEndSelect() {};//コンストラクタ
	~GameEndSelect() {};//デストラクタ

	bool Start();//開始処理
	void Render(RenderContext& rc);//描画処理

public://列挙型

	//ゲーム終了したのときの選択
	enum EnGameEndSelect
	{
		enGameEndSelect_Retry,//リトライ
		enGameEndSelect_ReturnTitle,//タイトルへ戻る
		enGameEndSelect_Num//選択数
	};

private://メンバ関数

	/// <summary>
	/// 選択テキストUIの初期化
	/// </summary>
	/// <param name="enGameClearSelect">ゲームクリアのときの選択</param>
	void InitSelectTextUI(EnGameEndSelect enGameEndSelect);
	
	/// <summary>
	/// 現在何を選択しているかを表すUIの初期化
	/// </summary>
	void InitCurrentSelectUI();

	/// <summary>
	/// 現在何を選択しているかを表すUIの更新処理
	/// </summary>
	/// <param name="enGameClearSelect">ゲームクリアのときの選択</param>
	void CurrentSelectUIUpdate(EnGameEndSelect enGameEndSelect);

	/// <summary>
	/// 入力の更新処理
	/// </summary>
	void InputUpdate();

	/// <summary>
	/// 現在何を選択しているかを表すUIのアニメーションの更新処理
	/// </summary>
	/// <param name="speed">再生速度</param>
	void CurrentSelectUIAnimationUpdate(float speed = 1.0f);

	/// <summary>
	/// シーンの遷移処理
	/// </summary>
	void TransitionScene();

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
	/// 現在何を選択しているかを表すための変数の取得
	/// </summary>
	/// <returns></returns>
	EnGameEndSelect GetCurrentSelect()
	{
		return (EnGameEndSelect)m_currentSelect;
	}

private://メンバ変数
	SpriteRender m_selectTextUI[enGameEndSelect_Num];//選択テキストUI
	SpriteRender m_currentSelectUI;//現在何を選択しているか表すUI
	Vector3 m_selectTextUIPosition[enGameEndSelect_Num];//選択テキストUIの座標
	Vector3 m_currentSelectUIPosition[enGameEndSelect_Num];//現在何を選択しているか表すUI
	int m_currentSelect = enGameEndSelect_Retry;//現在何を選択しているかを表すための変数
	int m_noDrawingCurrentSelectUICount = 0;//現在何を選択しているかを表すUIを描画していない回数
	float m_time = 0.0f;//時間
	bool m_isDrawingCurrentSelectUI = true;//現在何を選択しているかを表すUIを描画するか?
	bool m_isSelect = false;//選択できたか?
	bool m_isDrawingUI = false;//UIを描画するか?

private://表示するUIのファイルパス用のメンバ変数

	//選択テキストUIのファイルパス
	const std::string m_selectTextUIFilePath[enGameEndSelect_Num] = {
		"Assets/sprite/text/retry.dds",
		"Assets/sprite/text/returnTitle.dds"
	};

	//現在何を選択しているかを表すUIのファイルパス
	const std::string m_currentSelectUIFilePath = "Assets/sprite/select/select.dds";
};

