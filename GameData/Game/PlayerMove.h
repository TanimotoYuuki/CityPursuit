#pragma once
/// <summary>
/// プレイヤーの移動クラス
/// </summary>
class PlayerMove : public IGameObject
{
public:
	PlayerMove() {};//コンストラクタ
	~PlayerMove() {};//デストラクタ

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">プレイヤーの位置</param>
	void Init(const Vector3& position)
	{
		//キャラクターコントローラの設定
		m_charaCon.Init(15.0f, 75.0f, position);
	}

	/// <summary>
	/// 移動処理の実行
	/// </summary>
	void Execute(Vector3& position);

	/// <summary>
	/// 移動速度の取得
	/// </summary>
	/// <returns>移動速度</returns>
	Vector3& GetMoveSpeed()
	{
		return m_moveSpeed;
	}

private://メンバ変数
	Vector3 m_moveSpeed = Vector3::Zero;//移動速度
	CharacterController m_charaCon;//キャラクターコントローラ
};

