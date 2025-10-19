#pragma once
/// <summary>
/// QTEイベントでプレイヤー側が入力するクラス
/// </summary>
class QteEvent;
class QteEventInput : public IGameObject
{
public:
	QteEventInput() {};//コンストラクタ
	~QteEventInput() {};//デストラクタ

	bool Start();//開始処理

private://メンバ関数

	/// <summary>
	/// コマンド入力
	/// </summary>
	void InputCommand();

public://メンバ関数

	/// <summary>
	/// 実行処理
	/// </summary>
	void Execute();

	/// <summary>
	/// リセット処理
	/// </summary>
	void Reset()
	{
		m_nowInputCommandOrder = 0;//現在入力するコマンドの順番
		m_inputCommand = -1;//入力したコマンド
	}

	/// <summary>
	/// 現在入力しているコマンドの順番の取得
	/// </summary>
	/// <returns>現在入力しているコマンドの順番</returns>
	int GetNowInputCommandOrder()
	{
		return m_nowInputCommandOrder;
	}

	/// <summary>
	/// 入力したコマンドの取得
	/// </summary>
	/// <returns>入力したコマンド</returns>
	int GetInputCommand()
	{
		return m_inputCommand;
	}

	/// <summary>
	/// 最後のコマンド入力したか?
	/// </summary>
	/// <returns>trueなら最後のコマンド入力している</returns>
	bool IsInputLastCommand()
	{
		return m_isInputLastCommand;
	}

	/// <summary>
	/// 入力失敗しているか判断している変数のリセット処理
	/// </summary>
	void ResetInputFailed()
	{
		m_isInputFailed = false;
	}

	/// <summary>
	/// 最後のコマンド入力しているか判断している変数のリセット処理
	/// </summary>
	void ResetInputLastCommand()
	{
		m_isInputLastCommand = false;
	}

	/// <summary>
	/// 入力失敗したか？
	/// </summary>
	/// <returns>trueなら失敗している</returns>
	bool IsInputFailed()
	{
		return m_isInputFailed;
	}

private://メンバ変数
	int m_nowInputCommandOrder = 0;//現在入力するコマンドの順番
	int m_inputCommand = -1;//入力したコマンド
	bool m_isInputFailed = false;//入力失敗したか？
	bool m_isInputLastCommand = false;//最後のコマンド入力したか?
	QteEvent* m_qteEvent = nullptr;//QTEイベント用のインスタンス
};

