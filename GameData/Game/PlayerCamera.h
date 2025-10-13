#pragma once
/// <summary>
/// プレイヤーカメラクラス
/// </summary>
class PlayerCamera : public IGameObject
{
public:
	PlayerCamera() {};//コンストラクタ
	~PlayerCamera() {};//デストラクタ

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// カメラ追従処理の実行
	/// </summary>
	void Execute(Vector3& position);

	/// <summary>
	/// 敵の上に乗っているカメラか？を設定
	/// </summary>
	/// <param name="isOnEnemyCamera">trueなら敵の上に乗っているカメラにする</param>
	void SetIsOnEnemyCamera(const bool isOnEnemyCamera)
	{
		m_isOnEnemyCamera = isOnEnemyCamera;
	}

	/// <summary>
	/// カメラを動かせるかどうか？を設定
	/// </summary>
	/// <param name="canMoveCamera">trueならカメラを動かせる</param>
	void SetCanMoveCamera(const bool canMoveCamera)
	{
		m_canMoveCamera = canMoveCamera;
	}

private://メンバ変数
	Vector3 m_toCameraPos = Vector3::Zero;//注視点から視点までのベクトル
	Vector2 m_inputRStick = Vector2::Zero;//右スティックの入力量
	bool m_isOnEnemyCamera = false;//敵の上に乗っているときのカメラか？
	bool m_canMoveCamera = true;//カメラを動かせるかどうか？
};

