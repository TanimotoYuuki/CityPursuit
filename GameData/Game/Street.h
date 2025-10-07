#pragma once
/// <summary>
/// 道クラス
/// </summary>
class Street : public IGameObject
{
public:
	Street() {};//コンストラクタ
	~Street() {};//デストラクタ

	bool Start();//開始処理
	void Update();//更新処理
	void Render(RenderContext& rc);//描画処理

	/// <summary>
	/// ビルの位置の設定
	/// </summary>
	/// <param name="position">位置</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	/// <summary>
	/// ビルの回転の設定
	/// </summary>
	/// <param name="rotation">回転</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	/// <summary>
	/// ビルの大きさの設定
	/// </summary>
	/// <param name="scale">大きさ</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

private://メンバ変数
	ModelRender m_streetModel;//道モデル
	Vector3 m_position = Vector3::Zero;//道の位置
	Quaternion m_rotation = Quaternion::Identity;//道の回転
	Vector3 m_scale = Vector3::One;//道の大きさ
	PhysicsStaticObject m_physicsStaticObject;
};

