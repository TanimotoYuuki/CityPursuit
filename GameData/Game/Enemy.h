#pragma once
/// <summary>
/// 敵クラス
/// </summary>
class EnemyAI;
class Player;
class Enemy : public IGameObject
{
public:
	Enemy() {};//コンストラクタ
	~Enemy();//デストラクタ

	bool Start();//開始処理
	void Update();//更新処理
	void Render(RenderContext& rc);//描画処理

	/// <summary>
	/// 敵の位置の設定
	/// </summary>
	/// <param name="position">敵の位置</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	/// <summary>
	/// 敵の位置の取得
	/// </summary>
	/// <returns>敵の位置</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// <summary>
	/// 敵の回転の設定
	/// </summary>
	/// <param name="rotation">敵の回転</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	/// <summary>
	/// 敵の回転の取得
	/// </summary>
	/// <returns>敵の回転</returns>
	const Quaternion GetRotation() const
	{
		return m_rotation;
	}

	/// <summary>
	/// 敵の大きさの設定
	/// </summary>
	/// <param name="scale">敵の大きさ</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

	/// <summary>
	/// 敵の大きさの取得
	/// </summary>
	/// <returns>敵の大きさ</returns>
	const Vector3& GetScale() const
	{
		return m_scale;
	}

public://メンバ変数
	ModelRender m_enemyModel;//敵モデル
	Player* m_player = nullptr;//プレイヤーのインスタンス
	Vector3 m_position = Vector3::Zero;//敵の位置
	Quaternion m_rotation = Quaternion::Identity;//敵の回転
	Vector3 m_scale = Vector3::Zero;//敵の大きさ
	EnemyAI* m_enemyAI = nullptr;//敵AI用のインスタンス
};

