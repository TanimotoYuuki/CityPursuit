#pragma once
/// <summary>
/// プレイヤースイングアクションクラス
/// </summary>
class SwingActionManager;
class SwingModel;
class SwingTarget;
class Player;
class PlayerSwingAction : public IGameObject
{
public: 
	PlayerSwingAction() {};//コンストラクタ
	~PlayerSwingAction();//デストラクタ

	bool Start();//開始処理

	/// <summary>
	/// ワイヤーアクションの実行処理
	/// </summary>
	void Execute();

public://列挙型

	//スイングの状態
	enum EnSwingState
	{
		enSwingState_WireStretching,//ワイヤーを伸ばしている状態
		enSwingState_Swinging,//スイングしている状態
		enSwingState_AirAfterSwing,//スイング後の空中状態
		enSwingState_SwingEnd,//スイング終了状態
		enSwingState_FindSwingTarget,//スイングターゲット検索状態
		enSwingState_Num//スイングの状態数
	};

private://メンバ関数

	/// <summary>
	/// スイングアクションの前に行う処理
	/// </summary>
	void PreSwingAction();

	/// <summary>
	/// スイングアクションの更新処理
	/// </summary>
	void SwingActionUpdate();

	/// <summary>
	/// ワイヤーを伸ばしている処理
	/// </summary>
	void WireStretching();

	/// <summary>
	/// スイングしている処理
	/// </summary>
	void Swinging();

	/// <summary>
	/// スイング後の空中処理
	/// </summary>
	void AirAfterSwing();

	/// <summary>
	/// スイング終了処理
	/// </summary>
	void SwingEnd();

	/// <summary>
	/// スイングターゲットを探す
	/// </summary>
	void FindSwingTarget();

	/// <summary>
	/// ワイヤーが伸びるイベント処理
	/// </summary>
	void IsWireStretchingEvent();

	/// <summary>
	/// スイングしているイベント処理
	/// </summary>
	void IsSwingingEvent();

	/// <summary>
	/// スイング後の空中処理
	/// </summary>
	void IsAirAfterSwingEvent();

	/// <summary>
	/// スイングによるプレイヤーの移動
	/// </summary>
	void SwingPlayerMove();

public://メンバ関数

	/// <summary>
	/// スイングの状態の変更
	/// </summary>
	/// <param name="enSwingState">スイングの状態</param>
	void ChangeState(const EnSwingState enSwingState);

	/// <summary>
	/// スイングの状態の取得
	/// </summary>
	/// <returns>スイングの状態</returns>
	EnSwingState& GetSwingState() 
	{
		return m_swingState;
	}

private://メンバ変数

	EnSwingState m_swingState = enSwingState_FindSwingTarget;//スイングの状態
	Vector3 m_swingForwardDir = Vector3::Front;//スイングの前方向
	float m_swingSpeed = 0.0f;//スイングスピード
	float m_startDecelerateSwingSpeed = -100.0f;//スイング減速開始速度
	float m_velocityAfterSwing = 0.0f;//スイング後の速度
	float m_accelerationAfterSwing = 0.0f;//スイング後の加速
	bool m_afterSwing = false;//スイングを行ったか？
	bool m_swingRollFlag = false;//スイングロールを行うか？
	bool m_isSwingAction = false;//スイングアクションしているか？
	float m_g = 0.0f;
	Vector3 m_inputMoveDirXZ = Vector3::Zero;//入力によって生じたXZ平面での移動方向
	SwingActionManager* m_swingActionManager = nullptr;//スイングアクションマネージャー用のインスタンス
	SwingModel* m_swingModel = nullptr;//スイングモデル用のインスタンス
	SwingTarget* m_swingTarget;//スイングターゲット用のインスタンス
	Player* m_player = nullptr;//プレイヤー用のインスタンス
};

