#pragma once
#include "PlayerJump.h"
/// <summary>
/// プレイヤーの移動クラス
/// </summary>
class PlayerSwingAction;
class DebugLog;
class PlayerMove : public IGameObject
{
public:
	PlayerMove() {};//コンストラクタ
	~PlayerMove();//デストラクタ

	bool Start();//開始処理

	/// <summary>
	/// 移動処理の実行
	/// </summary>
	void Execute(Vector3& position, CharacterController& characterController);

	/// <summary>
	/// 重力を使用するか？を設定する
	/// </summary>
	/// <param name="useGravity">trueなら重力を使用する</param>
	void SetUseGravity(const bool useGravity)
	{
		m_useGravity = useGravity;
	}

	/// <summary>
	/// スイングアクション用の重力を使用するか？を設定する
	/// </summary>
	/// <param name="useGravity">trueならスイングアクション用の重力を使用する</param>
	void SetUseSwingActionGravity(const bool useGravity)
	{
		m_ussSwingActionGravity = useGravity;
	}

	/// <summary>
	/// 移動ができるか？を設定する
	/// </summary>
	/// <param name="useGravity">trueなら移動できる</param>
	void SetCanMove(const bool canMove)
	{
		m_canMove = canMove;
	}

	/// <summary>
	/// ジャンプができるか？を設定する
	/// </summary>
	/// <param name="useGravity">trueならジャンプできる</param>
	void SetCanJump(const bool canJump)
	{
		m_canMove = canJump;
	}

	/// <summary>
	/// 移動速度の設定
	/// </summary>
	/// <param name="moveSpeed">移動速度</param>
	void SetMoveSpeed(Vector3& moveSpeed)
	{
		m_moveSpeed = moveSpeed;
	}

	/// <summary>
	/// 移動速度の加算
	/// </summary>
	/// <param name="moveSpeed">移動速度</param>
	void AddMoveSpeed(const Vector3& moveSpeed)
	{
		m_moveSpeed += moveSpeed;
	}

	/// <summary>
	/// 移動速度に制限を書ける
	/// </summary>
	/// <param name="limitSpeed">制限速度</param>
	void LimitMoveSpeed(const float limitSpeed);

	/// <summary>
	/// 移動速度の取得
	/// </summary>
	/// <returns>移動速度</returns>
	float GetXZSpeed() const
	{
		Vector3 moveVecXZ = m_moveSpeed;
		moveVecXZ.y = 0.0f;
		return moveVecXZ.Length();
	}

	/// <summary>
	/// 移動速度のX成分のリセット
	/// </summary>
	void ResetMoveSpeedX()
	{
		m_moveSpeed.x = 0.0f;
	}

	/// <summary>
	/// 移動速度のY成分のリセット
	/// </summary>
	void ResetMoveSpeedY()
	{
		m_moveSpeed.y = 0.0f;
	}

	/// <summary>
	/// 移動速度のZ成分のリセット
	/// </summary>
	void ResetMoveSpeedZ()
	{
		m_moveSpeed.z = 0.0f;
	}

	/// <summary>
	/// 入力情報のリセット
	/// </summary>
	void ResetInput()
	{
		m_inputLStick = Vector3::Zero;
		m_inputLStickMemory = Vector3::Zero;
	}

	/// <summary>
	/// 移動状態のリセット
	/// </summary>
	void ResetMoveState()
	{
		m_isWalk = false;
		m_isRun = false;
	}

	/// <summary>
	/// 左スティックの入力量の取得
	/// </summary>
	/// <returns>Lスティックの入力量</returns>
	Vector3& GetInputLStick()
	{
		return m_inputLStick;
	}

	/// <summary>
	/// 移動速度の取得
	/// </summary>
	/// <returns>移動速度</returns>
	Vector3& GetMoveSpeed()
	{
		return m_moveSpeed;
	}

	/// <summary>
	/// 歩いているか?
	/// </summary>
	/// <returns>trueなら歩いている</returns>
	bool IsWalk() const
	{
		return m_isWalk;
	}

	/// <summary>
	/// 走っているか?
	/// </summary>
	/// <returns>trueなら走っている</returns>
	bool IsRun() const
	{
		return m_isRun;
	}

	/// <summary>
	/// プレイヤージャンプの取得
	/// </summary>
	/// <returns>プレイヤージャンプ</returns>
	PlayerJump& GetPlayerJump()
	{
		return m_playerJump;
	}

	/// <summary>
	/// プレイヤースイングアクションの取得
	/// </summary>
	/// <returns>プレイヤースイングアクション</returns>
	PlayerSwingAction* GetPlayerSwingAction()
	{
		return m_playerSwingAction;
	}

	/// <summary>
	/// プレイヤーの進行方向の取得
	/// </summary>
	/// <returns>プレイヤーの進行方向</returns>
	const Vector3& GetMoveDirection() const
	{
		return m_moveDirection;
	}

private://メンバ変数
	Vector3 m_inputLStickMemory = Vector3::Zero;//左スティックの入力量を格納する変数
	Vector3 m_inputLStick = Vector3::Zero;//左スティックの入力量
	Vector3 m_cameraForward = Vector3::Zero;//カメラの前方向
	Vector3 m_cameraRight = Vector3::Zero;//カメラの横方向
	Vector3 m_moveSpeed = Vector3::Zero;//移動速度
	Vector3 m_moveDirection = Vector3{ 0.0f,0.0f,1.0f };//進行方向
	PlayerJump m_playerJump;//プレイヤージャンプクラス
	bool m_canMove = true;//移動できるか？
	bool m_camJump = true;//ジャンプできるか?
	bool m_useGravity = true;//重力を使用するか？
	bool m_ussSwingActionGravity = false;//スイングアクション用の重力を使用するか?
	bool m_isWalk = false;//歩いているか?
	bool m_isRun = false;//走っているか?
	PlayerSwingAction* m_playerSwingAction = nullptr;//プレイヤースイングアクション用のインスタンス
	DebugLog* m_debugLog = nullptr;//デバッグログ用のインスタンス
};

