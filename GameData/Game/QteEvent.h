#pragma once
/// <summary>
/// QTEイベントクラス
/// </summary>
class QteEventInput;
class QteEvent : public IGameObject
{
public:
	QteEvent() {};//コンストラクタ
	~QteEvent();//デストラクタ

	bool Start();//開始処理
	void Update();;//更新処理
	void Render(RenderContext& rc);//描画処理


public://列挙型
	//ゲームパッド入力リスト
	enum EnGamePadInputList
	{
		enGamePadInputList_UpArrow,//上方向
		enGamePadInputList_DownArrow,//下方向
		enGamePadInputList_LeftArrow,//左方向
		enGamePadInputList_LeftUpArrow,//左上方向
		enGamePadInputList_LeftDownArrow,//左下方向
		enGamePadInputList_RightArrow,//右方向
		enGamePadInputList_RightUpArrow,//右上方向
		enGamePadInputList_RightDownArrow,//右下方向
		enGamePadInputList_AButton,//Aボタン
		enGamePadInputList_BButton,//Bボタン
		enGamePadInputList_XButton,//Xボタン
		enGamePadInputList_YButton,//Yボタン
		enGamePadInputList_Num//ゲームパッド入力数
	};

	//コマンドリスト
	enum EnCommandList
	{
		enCommandList_Hadouken,//波動拳
		enCommandList_SyakunetsuHadouken,//灼熱波動拳
		enCommandList_Tatsumaki,//竜巻旋風脚
		enCommandList_Shoryuken,//昇龍拳
		enCommandList_SomersaultKick,//サマーソルトキック
		enCommandList_SonicBoom,//ソニックブーム
		enCommandList_YogaFlame,//ヨガフレイム
		enCommandList_ScrewPiledriver,//スクリューパイルドライバー
		enCommandList_Num,//コマンド数
		enCommandList_None//コマンドなし
	};

private://メンバ関数

	/// <summary>
	/// 初期設定
	/// </summary>
	void Init();

	/// <summary>
	/// ゲームパッドのボタンや方向キーを入力前のUIの初期設定
	/// </summary>
	/// <param name="enGamePadInputList">ゲームパッド入力リスト</param>
	void InitGamePadInputBeforeUI(EnGamePadInputList enGamePadInputList);
	
	/// <summary>
	/// ゲームパッドのボタンや方向キーを入力後のUIの初期設定
	/// </summary>
	/// <param name="enGamePadInputList">ゲームパッド入力リスト</param>
	void InitGamePadInputAfterUI(EnGamePadInputList enGamePadInputList);

	/// <summary>
	/// ゲームパッドUIのリセット処理
	/// </summary>
	void GamePadPushUIReset();

	/// <summary>
	/// 入力するコマンドの設定
	/// </summary>
	void SetInputCommand();

	/// <summary>
	/// ゲームパッドの入力前後のUIの位置設定
	/// </summary>
	/// <param name="commandNum">コマンドの順番</param>
	/// <param name="enGamePadInputList">ゲームパッド入力リスト</param>
	void SetGamePadInputUIPosition(int commandNum, EnGamePadInputList enGamePadInputList);

	/// <summary>
	/// ゲームパッドの入力前後のUIのイージング設定
	/// </summary>
	/// <param name="enGamePadInputList">ゲームパッド入力リスト</param>
	/// <param name="easingBeforeUI">イージング前の設定用のゲームパッドUI</param>
	/// <param name="easingAfterUI">イージング後の設定用のゲームパッドUI</param>
	void SetGamePadInputUIEasing(EnGamePadInputList enGamePadInputList, SpriteRender& easingBeforeUI, SpriteRender& easingAfterUI);

	/// <summary>
	/// ゲームパッドの入力前後のUIのイージング更新処理
	/// </summary>
	/// <param name="enGamePadInputList">ゲームパッド入力リスト</param>
	/// <param name="inputAferUI">ゲームパッドUI(入力後)</param>
	void GamePadInputUIEasingUpdate(EnGamePadInputList enGamePadInputList, SpriteRender& inputAfterUI);

public://メンバ関数

	/// <summary>
	/// コマンド入力リストの取得
	/// </summary>
	std::vector<EnGamePadInputList> GetInputCommand()
	{
		return m_inputCommandList;
	}

	/// <summary>
	/// コマンド入力成功
	/// </summary>
	/// <param name="command">コマンド</param>
	void SuccessInputCommand(int command)
	{
		m_isInputCommandSuccess[command] = true;
	}

private://メンバ変数
	SpriteRender m_gamePadInputBeforeUI[enGamePadInputList_Num];//ゲームパッドのボタンや方向キーを入力前のUI
	SpriteRender m_gamePadInputAfterUI[enGamePadInputList_Num];//ゲームパッドのボタンや方向キーを入力後のUI
	Vector3 m_gamePadInputUIBeforeEasingPosition[enGamePadInputList_Num];//ゲームパッドUIイージング前用の位置
	Vector3 m_gamePadInputUIAfterEasingPosition[enGamePadInputList_Num];//ゲームパッドUIイージング後用の位置
	Vector3 m_gamePadInputUIEasingPosition[enGamePadInputList_Num];//ゲームパッドUIイージング中用の位置
	EnCommandList m_nowInputCommand = enCommandList_None;//現在入力しているコマンド
	EnCommandList m_nextInputCommand = enCommandList_None;//次入力するコマンド
	float m_gamePadInputUIEasingTime[enGamePadInputList_Num] = { 0.0f };//ゲームパッドUIのイージング用の割合
	bool m_isGamePadUIEasingStart[enGamePadInputList_Num] = { false };//ゲームパッドUIがイージングしているか?
	bool m_isGamePadUIEasingEnd[enGamePadInputList_Num] = { false };//ゲームパッドUIがイージングし終わっているか?
	bool m_isInputCommandSuccess[enGamePadInputList_Num] = { false };//コマンド入力が成功しているか?
	std::vector<EnGamePadInputList> m_inputCommandList;//コマンド入力リスト
	QteEventInput* m_qteEventInput = nullptr;//QTEイベントでプレイヤー側が入力する用のインスタンス

private://データ関連のメンバ変数
	//ゲームパッドのボタンや方向キーを入力前のUIのファイルパス
	const std::string m_gamepadInputBeforeUIFilePath[enGamePadInputList_Num] = {
		"Assets/sprite/gamepad/inputBefore/arrow/upArrow.dds",//上方向
		"Assets/sprite/gamepad/inputBefore/arrow/downArrow.dds",//下方向
		"Assets/sprite/gamepad/inputBefore/arrow/leftArrow.dds",//左方向
		"Assets/sprite/gamepad/inputBefore/arrow/leftUpArrow.dds",//左上方向
		"Assets/sprite/gamepad/inputBefore/arrow/leftDownArrow.dds",//左下方向
		"Assets/sprite/gamepad/inputBefore/arrow/rightArrow.dds",//右方向
		"Assets/sprite/gamepad/inputBefore/arrow/rightUpArrow.dds",//右上方向
		"Assets/sprite/gamepad/inputBefore/arrow/rightDownArrow.dds",//右下方向
		"Assets/sprite/gamepad/inputBefore/button/aButton.dds",//Aボタン
		"Assets/sprite/gamepad/inputBefore/button/bButton.dds",//Bボタン
		"Assets/sprite/gamepad/inputBefore/button/xButton.dds",//Xボタン
		"Assets/sprite/gamepad/inputBefore/button/yButton.dds"//Yボタン
	};

	//ゲームパッドのボタンや方向キーを入力前のUIのファイルパス
	const std::string m_gamepadInputAfterUIFilePath[enGamePadInputList_Num] = {
		"Assets/sprite/gamepad/inputAfter/arrow/upArrow.dds",//上方向
		"Assets/sprite/gamepad/inputAfter/arrow/downArrow.dds",//下方向
		"Assets/sprite/gamepad/inputAfter/arrow/leftArrow.dds",//左方向
		"Assets/sprite/gamepad/inputAfter/arrow/leftUpArrow.dds",//左上方向
		"Assets/sprite/gamepad/inputAfter/arrow/leftDownArrow.dds",//左下方向
		"Assets/sprite/gamepad/inputAfter/arrow/rightArrow.dds",//右方向
		"Assets/sprite/gamepad/inputAfter/arrow/rightUpArrow.dds",//右上方向
		"Assets/sprite/gamepad/inputAfter/arrow/rightdownArrow.dds",//右下方向
		"Assets/sprite/gamepad/inputAfter/button/aButton.dds",//Aボタン
		"Assets/sprite/gamepad/inputAfter/button/bButton.dds",//Bボタン
		"Assets/sprite/gamepad/inputAfter/button/xButton.dds",//Xボタン
		"Assets/sprite/gamepad/inputAfter/button/yButton.dds"//Yボタン
	};
};

