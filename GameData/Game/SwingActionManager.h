#pragma once
class Player;
class SwingTarget;
class Buildings;
class SwingActionManager : public IGameObject
{
public:
	SwingActionManager() {};//コンストラクタ
	~SwingActionManager() {};//デストラクタ

	bool Start();//開始処理

	/// <summary>
	/// 実行処理
	/// </summary>
	void Execute();

	/// <summary>
	/// スイングアクションができる有効範囲の設定
	/// </summary>
	/// <param name="radius">有効範囲</param>
	void SetSwingTargetScopeRadius(float radius)
	{
		m_swingTargetScopeRadius = radius;
	}

	/// <summary>
	/// スイングアクションができる最短距離の取得
	/// </summary>
	/// <returns>スイングアクションができる最短距離</returns>
	const Vector3& GetSwingActionShortestDistance() const
	{
		return m_swingActionShortestDistance;
	}

	/// <summary>
	/// 有効範囲内のビルを見つかったかどうか？
	/// </summary>
	/// <returns>trueならビルが見つかった</returns>
	bool IsSwingTargetScopeRadiusFound() const
	{
		return m_isSwingTargetScopeRadiusFound;
	}

private://メンバ変数
	float m_swingTargetScopeRadius = 7000.0f;//スイングターゲットができる有効範囲
	Vector3 m_swingActionShortestDistance = Vector3::Zero;//スイングアクションができる最短距離
	bool m_isSwingTargetScopeRadiusFound = false;//有効範囲内のビルが見つかったかどうか
	Player* m_player = nullptr;//プレイヤー用のインスタンス
};

