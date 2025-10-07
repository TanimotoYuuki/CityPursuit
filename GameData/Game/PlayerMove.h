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
	const Vector3& GetMoveSpeed() const
	{
		return m_moveSpeed;
	}

private://メンバ変数
	Vector3 m_inputLStickMemory = Vector3::Zero;//左スティックの入力量を格納する変数
	Vector3 m_inputLStick = Vector3::Zero;//左スティックの入力量
	Vector3 m_cameraForward = Vector3::Zero;//カメラの前方向
	Vector3 m_cameraRight = Vector3::Zero;//カメラの横方向
	Vector3 m_moveSpeed = Vector3::Zero;//移動速度
	PlayerJump m_playerJump;//プレイヤージャンプクラス
	PlayerSwingAction* m_playerSwingAction = nullptr;//プレイヤースイングアクション用のインスタンス
	DebugLog* m_debugLog = nullptr;//デバッグログ用のインスタンス
};

