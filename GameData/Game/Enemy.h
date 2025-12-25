#pragma once
/// <summary>
/// 敵クラス
/// </summary>
class EnemyAI;
class EnemyEffect;
class Game;
class Enemy : public IGameObject
{
public:
	Enemy() {};//コンストラクタ
	~Enemy();//デストラクタ

	bool Start();//開始処理
	void Update();//更新処理
	void Render(RenderContext& rc);//描画処理

public://メンバ関数

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

	/// <summary>
	/// プレイヤーが乗っているかの設定
	/// </summary>
	/// <param name="isOnEnemy">trueならプレイヤーが乗っている</param>
	void SetOnPlayer(bool isOnPlayer)
	{
		m_isOnPlayer = isOnPlayer;
	}

	/// <summary>
	/// プレイヤーが乗っているか?
	/// </summary>
	/// <returns>trueならプレイヤーが乗っている</returns>
	bool IsOnPlayer() const
	{
		return m_isOnPlayer;
	}

	/// <summary>
	/// ゲーム全体を管理するクラスのポインタの設定
	/// </summary>
	/// <param name="game">ゲーム全体を管理するクラスのポインタ</param>
	void SetGamePtr(Game* game)
	{
		m_game = game;
	}

	/// <summary>
	/// 敵AIクラスのポインタの取得
	/// </summary>
	/// <returns></returns>
	EnemyAI* GetEnemyAIPtr() const
	{
		return m_enemyAI;
	}

	/// <summary>
	/// 敵のエフェクトクラスの取得
	/// </summary>
	/// <returns></returns>
	EnemyEffect* GetEnemyEffectPtr() const
	{
		return m_enemyEffect;
	}

	/// <summary>
	/// ゲーム全体を管理するクラスのポインタの取得
	/// </summary>
	/// <returns>ゲーム全体を管理するクラスのポインタ</returns>
	Game* GetGamePtr() const
	{
		return m_game;
	}

private://メンバ変数
	ModelRender m_enemyModel;//敵モデル
	Vector3 m_position = Vector3::Zero;//敵の位置
	Quaternion m_rotation = Quaternion::Identity;//敵の回転
	Vector3 m_scale = Vector3::Zero;//敵の大きさ
	bool m_isOnPlayer = false;//プレイヤーに乗っているか
	EnemyAI* m_enemyAI = nullptr;//敵AI用のインスタンス
	EnemyEffect* m_enemyEffect = nullptr;//敵のエフェクト用のインスタンス
	Game* m_game = nullptr;//ゲーム全体を管理する用のインスタン
};

