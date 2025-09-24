#pragma once
/// <summary>
/// スイングするターゲットを探索するクラス
/// </summary>
class SwingTarget : public IGameObject
{
public:
	SwingTarget() {};//コンストラクタ
	~SwingTarget() {};//デストラクタ

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="tkmFile">tkmファイル</param>
	void Init(const TkmFile& tkmFile)
	{
		m_aabb.InitFromTkmFile(tkmFile);
	}

	/// <summary>
	/// スイングのターゲットの座標の計算
	/// </summary>
	/// <param name="worldMatrix">ワールド行列</param>
	/// <param name="widthSegments">幅セグメント</param>
	/// <param name="heightSegments">高さセグメント</param>
	/// <param name="lengthSegments">長さセグメント</param>
	/// <param name="heightLowerLimit">高さの制限</param>
	void CalcSwingTargetPositions(
		const Matrix& worldMatrix,
		const UINT widthSegments,
		const UINT heightSegments,
		const UINT lengthSegments,
		const float heightLowerLimit = 0.0f
	);


	/// <summary>
	/// スイングターゲットの座標コンテナの取得
	/// </summary>
	/// <returns>スイングターゲットの座標コンテナ</returns>
	const std::vector<Vector3>& GetSwingTargetPositions() const
	{
		return m_swingTargetPositions;
	}

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

private://メンバ関数

	/// <summary>
	/// スイングのターゲットの座標コンテナの再初期化
	/// </summary>
	/// <param name="widthSegments">幅セグメント</param>
	/// <param name="heightSegments">高さセグメント</param>
	/// <param name="lengthSegments">長さセグメント</param>
	void ReInitSwingTargetPositions(
		const UINT widthSegments,
		const UINT heightSegments,
		const UINT lengthSegments
	);

private://メンバ変数
	AABB m_aabb;//AABB
	std::vector<Vector3> m_swingTargetPositions;//スイングターゲットの座標コンテナ
	Vector3 m_position = Vector3::Zero;//座標
};

