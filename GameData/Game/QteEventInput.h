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
		m_nowInputCommand = 0;//現在入力するコマンド
		m_inputCommand = -1;//入力したコマンド
	}

private://メンバ変数
	int m_nowInputCommand = 0;//現在入力するコマンド
	int m_inputCommand = -1;//入力したコマンド
	QteEvent* m_qteEvent = nullptr;//QTEイベント用のインスタンス
};

