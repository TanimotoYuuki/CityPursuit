#pragma once
/// <summary>
/// プレイヤーカメラクラス
/// </summary>
class Player;
class PlayerCamera : public IGameObject
{
public:
	PlayerCamera() {};//コンストラクタ
	~PlayerCamera() {};//デストラクタ

private: 

	/// <summary>
	/// 敵の上に乗っている用のカメラ
	/// </summary>
	void OnEnemyCamera(Player* playerData, const Vector3& position);

public://メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// カメラ追従処理の実行
	/// </summary>
	void Execute(Player* playerData, const Vector3& position);

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

	/// <summary>
	/// リセット処理
	/// </summary>
	void Reset()
	{
		m_toCameraPos.Set(0.0f, 10.0f, -300.0f);
	}

	/// <summary>
	/// バネカメラの減衰率をイージングする
	/// </summary>
	/// <param name="rate">割合</param>
	void LerpDampingRate(const float rate)
	{
		m_springCamera.SetDampingRate(Math::Lerp<float>(rate, 0.85f, 1.2f));
	}

	/// <summary>
	/// 注視点の上方向のオフセットをイージングする
	/// </summary>
	/// <param name="rate">割合</param>
	void LerpTargetOffsetUp(const float rate)
	{
		m_targetOffsetUp = Math::Lerp<float>(rate, 120.0f, 170.0f);
	}

	/// <summary>
	/// 注視点のXZ方向のオフセットをイージングする
	/// </summary>
	/// <param name="rate">割合</param>
	void LerpTargetOffsetForward(const float rate)
	{
		m_targetOffsetXZ = Math::Lerp<float>(rate, 0.0f, 150.0f);
	}

	/// <summary>
	/// バネカメラの取得
	/// </summary>
	/// <returns>バネカメラ</returns>
	SpringCamera& GetSpringCamera()
	{
		return m_springCamera;
	}

	/// <summary>
	/// 注視点から視点までのベクトルの取得
	/// </summary>
	/// <returns>注視点から視点までのベクトル</returns>
	const Vector3& GetToCameraPos() const
	{
		return m_toCameraPos;
	}

private://メンバ変数
	SpringCamera m_springCamera;//バネカメラ
	float m_targetOffsetUp = 0.0f;//注視点の上方向オフセット
	float m_targetOffsetXZ = 0.0f;//注視点のXZ方向オフセット
	Vector3 m_toCameraPos = Vector3::Zero;//注視点から視点までのベクトル
	Vector2 m_inputRStick = Vector2::Zero;//右スティックの入力量
	bool m_isOnEnemyCamera = false;//敵の上に乗っているときのカメラか？
	bool m_canMoveCamera = true;//カメラを動かせるかどうか？
};

