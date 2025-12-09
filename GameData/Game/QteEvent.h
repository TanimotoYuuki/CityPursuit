#pragma once
#include "SpriteAnimation.h"
/// <summary>
/// QTEイベントクラス
/// </summary>
class QteEventInput;
class Game;
class Player;
class Enemy;
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

	//QTEイベントの結果
	enum EnQteEventResult
	{
		enQteEventResult_Success,//成功
		enQteEventResult_Failed,//失敗
		enQteEventResult_Num//QTE結果UI数
	};

	//QTEイベントの結果UIの演出
	enum EnQteEventResultUIDirection
	{
		enQteEventResultUIDirection_Start,//開始処理
		enQteEventResultUIDirection_End,//終了処理
		enQteEventResultUIDirection_Num,//演出数
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
	/// 制限時間UI関連の初期化
	/// </summary>
	void InitTimeLimitUI();

	/// <summary>
	/// QTEイベントの結果UIの初期化
	/// </summary>
	void InitQteEventResultUI(EnQteEventResult enQteEventReulst);

	/// <summary>
	/// ゲームパッドの入力前後のUIの更新処理
	/// </summary>
	void GamePadInputUIUpdate();

	/// <summary>
	/// 制限時間の更新処理
	/// </summary>
	void TimeLimitUpdate();

	/// <summary>
	/// 入力に失敗したときの処理
	/// </summary>
	void InputFailed();

	/// <summary>
	/// 制限時間UIの色を変える処理
	/// </summary>
	/// <param name="timelimit">制限時間</param>
	void ChangeTimeLimitUIColor(float timeLimit);

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
	/// <param name="enQteEventResult">QTEイベントの結果</param>
	/// <param name="enGamePadInputList">ゲームパッド入力リスト</param>
	/// <param name="easingBeforeUI">イージング前の設定用のゲームパッドUI</param>
	/// <param name="easingAfterUI">イージング後の設定用のゲームパッドUI</param>
	void SetGamePadInputUIEasing(EnQteEventResult enQteEventResult, EnGamePadInputList enGamePadInputList, SpriteRender& easingBeforeUI, SpriteRender& easingAfterUI);

	/// <summary>
	/// ゲームパッドの入力前後のUIのイージング更新処理
	/// </summary>
	/// <param name="enQteEventResult">QTEイベントの結果</param>
	/// <param name="enGamePadInputList">ゲームパッド入力リスト</param>
	/// <param name="inputAferUI">ゲームパッドUI(入力後)</param>
	void GamePadInputUIEasingUpdate(EnQteEventResult enQteEventResult, EnGamePadInputList enGamePadInputList, SpriteRender& inputAfterUI);

public://メンバ関数

	/// <summary>
	/// ゲーム全体を管理するクラスのポインタの設定
	/// </summary>
	/// <param name="game">ゲーム全体を管理するクラスのポインタ</param>
	void SetGamePtr(Game* game)
	{
		m_game = game;
	}

	/// <summary>
	/// ターゲットの敵を設定する
	/// </summary>
	/// <param name="target">ターゲットの敵</param>
	void SetTargetEnemy(Enemy* target)
	{
		m_targetEnemy = target;
	}

	/// <summary>
	/// プレイヤークラスのポインタの設定
	/// </summary>
	/// <param name="playerCatchEnemy">プレイヤークラスのポインタ</param>
	void SetPlayerPtr(Player* player)
	{
		m_player = player;
	}

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

	/// <summary>
	/// 制限時間を動かす
	/// </summary>
	void StartTimeLimit()
	{
		m_isStopTimeLimit = false;
	}

	/// <summary>
	/// 制限時間を止める
	/// </summary>
	void StopTimeLimit()
	{
		m_isStopTimeLimit = true;
	}

	/// <summary>
	/// リザルト結果に対しての演出の終了
	/// </summary>
	void FinishResultDirection()
	{
		m_isResultDirection = false;
	}

	/// <summary>
	/// 時間が止まっているか?
	/// </summary>
	/// <returns>trueなら止まっている</returns>
	bool IsStopTimeLimit()
	{
		return m_isStopTimeLimit;
	}

	/// <summary>
	/// QTEイベントの結果の取得
	/// </summary>
	/// <param name="enQteEventResult">QTEイベントの結果</param>
	/// <returns>どれかがtrueならQTEイベントの結果</returns>
	bool IsQteEventResult(EnQteEventResult enQteEventResult)
	{
		return m_isQteEventResult[enQteEventResult];
	}

	/// <summary>
	/// リセット処理
	/// </summary>
	void Reset();

private://メンバ変数
	SpriteRender m_gamePadInputBeforeUI[enGamePadInputList_Num];//ゲームパッドのボタンや方向キーを入力前のUI
	SpriteRender m_gamePadInputAfterUI[enQteEventResult_Num][enGamePadInputList_Num];//ゲームパッドのボタンや方向キーを入力後のUI
	SpriteRender m_qteEventResultUI[enQteEventResult_Num];//QTEイベントの結果UI
	SpriteRender m_timeLimitUI;//制限時間UI
	SpriteRender m_timeLimitBerUI;//制限時間バーUI
	Vector3 m_gamePadInputUIBeforeEasingPosition[enQteEventResult_Num][enGamePadInputList_Num];//ゲームパッドUIイージング前用の位置
	Vector3 m_gamePadInputUIAfterEasingPosition[enQteEventResult_Num][enGamePadInputList_Num];//ゲームパッドUIイージング後用の位置
	Vector3 m_gamePadInputUIEasingPosition[enQteEventResult_Num][enGamePadInputList_Num];//ゲームパッドUIイージング中用の位置
	Vector4 m_timeLimitUIColor = Vector4::White;//制限時間UIの色
	EnCommandList m_nowInputCommand = enCommandList_None;//現在入力しているコマンド
	EnCommandList m_nextInputCommand = enCommandList_None;//次入力するコマンド
	EnQteEventResult m_qteEventResult = enQteEventResult_Num;//QTEイベントの結果
	EnQteEventResultUIDirection m_qteEventResultUIDirection = enQteEventResultUIDirection_Start;//QTEイベントの結果UIの演出
	int m_succesInputCommandCount = 0;//コマンド入力が成功した回数
	float m_gamePadInputUIEasingTime[enQteEventResult_Num][enGamePadInputList_Num] = { 0.0f };//ゲームパッドUIのイージング用の割合
	float m_timeLimitMax = 0.0f;//制限時間(上限)
	float m_timeLimit = 15.0f;//制限時間
	bool m_isGamePadUIEasingStart[enQteEventResult_Num][enGamePadInputList_Num] = { false };//ゲームパッドUIがイージングしているか?
	bool m_isGamePadUIEasingEnd[enQteEventResult_Num][enGamePadInputList_Num] = { false };//ゲームパッドUIがイージングし終わっているか?
	bool m_isInputCommandSuccess[enGamePadInputList_Num] = { false };//コマンド入力が成功しているか?
	bool m_isStopTimeLimit = false;//制限時間を止めるかどうか？
	bool m_isQteEventResult[enQteEventResult_Num] = { false,false };//QTEイベントの結果
	bool m_isAddSuccessInputCommandCount = false;//コマンド入力成功回数を増やしたかどうか?
	bool m_isResultDirection = false;//リザルト結果に対しての演出しているか?
	std::vector<EnGamePadInputList> m_inputCommandList;//コマンド入力リスト
	QteEventInput* m_qteEventInput = nullptr;//QTEイベントでプレイヤー側が入力する用のインスタンス
	Player* m_player = nullptr;//プレイヤー用のインスタンス
	Enemy* m_targetEnemy = nullptr;//ターゲットにしているエネミー用のインスタンス
	Game* m_game = nullptr;//ゲーム全体を管理する用のインスタンス
	std::unique_ptr<PositionSpriteAnimation> m_positionSpriteAnimation[enQteEventResult_Num][enQteEventResultUIDirection_Num];//座標を変えるアニメーション用のユニークポインタ
	std::unique_ptr<AlphaSpriteAnimation> m_alphaSpriteAnimation[enQteEventResult_Num][enQteEventResultUIDirection_Num];//透明度を変えるアニメーション用のユニークポインタ
	std::unique_ptr<RotationSpriteAnimation> m_rotationSpriteAnimation;//回転を変えるアニメーション用のユニークポインタ

private://ファイルパス関連のメンバ変数
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

	//ゲームパッドのボタンや方向キーを入力前のUIのファイルパス
	const std::string m_gamepadInputFailedUIFilePath[enGamePadInputList_Num] = {
		"Assets/sprite/gamepad/inputFailed/arrow/upArrow.dds",//上方向
		"Assets/sprite/gamepad/inputFailed/arrow/downArrow.dds",//下方向
		"Assets/sprite/gamepad/inputFailed/arrow/leftArrow.dds",//左方向
		"Assets/sprite/gamepad/inputFailed/arrow/leftUpArrow.dds",//左上方向
		"Assets/sprite/gamepad/inputFailed/arrow/leftDownArrow.dds",//左下方向
		"Assets/sprite/gamepad/inputFailed/arrow/rightArrow.dds",//右方向
		"Assets/sprite/gamepad/inputFailed/arrow/rightUpArrow.dds",//右上方向
		"Assets/sprite/gamepad/inputFailed/arrow/rightdownArrow.dds",//右下方向
		"Assets/sprite/gamepad/inputFailed/button/aButton.dds",//Aボタン
		"Assets/sprite/gamepad/inputFailed/button/bButton.dds",//Bボタン
		"Assets/sprite/gamepad/inputFailed/button/xButton.dds",//Xボタン
		"Assets/sprite/gamepad/inputFailed/button/yButton.dds"//Yボタン
	};

	//QTEイベントの結果UIのファイルパス
	const std::string m_qteEventResultUIFilePath[enQteEventResult_Num] = {
		"Assets/sprite/text/success.dds",//成功
		"Assets/sprite/text/failed.dds"//失敗
	};
};

