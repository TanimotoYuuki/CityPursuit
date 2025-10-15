#pragma once
/// <summary>
/// プレイヤーが敵をキャッチするクラス
/// </summary>
class Player;
class PlayerSwingAction;
class SwingModel;
class Enemy;
class QteEvent;
class PlayerCatchEnemy : public IGameObject
{
public:
	PlayerCatchEnemy() {};
	~PlayerCatchEnemy() {};

	bool Start();

public://列挙型

	//敵をキャッチする状態
	enum EnCatchEnemyState
	{
		enStartWireToEnemy,	//敵にワイヤーを伸ばし始める
		enWireingToEnemy,	//敵にワイヤーを伸ばし中
		enGoOnEnemy,		//敵の上に移動中
		enOnEnemy,			//敵の上にいる
	};

private://メンバ関数

	/// <summary>
	/// ターゲットを探す処理
	/// </summary>
	void FindTarget();

	/// <summary>
	/// 敵に向かってワイヤーを伸ばし始める処理
	/// </summary>
	void StartWireToEnemy();

	/// <summary>
	/// 敵の方を向かせる
	/// </summary>
	void LookAtEnemy();

	/// <summary>
	/// 敵に向かって糸を伸ばす処理
	/// </summary>
	void WireingToEnemy();

	/// <summary>
	/// 敵の上に行く処理
	/// </summary>
	void GoOnEnemy();

	/// <summary>
	/// 敵の上にいる処理
	/// </summary>
	void OnEnemy();

	/// <summary>
	/// ステートを変更する
	/// </summary>
	/// <param name="newState">新しいステート</param>
	void ChangeState(const EnCatchEnemyState newState);

public://メンバ関数

	/// <summary>
	/// 敵をキャッチする実行処理
	/// </summary>
	void Execute();

	/// <summary>
	/// リセット処理
	/// </summary>
	void Reset();

	/// <summary>
	/// 敵をキャッチしているか
	/// </summary>
	/// <returns>trueならキャッチしている</returns>
	bool IsCatchingEnemy()
	{
		return m_isCatchEnemy;
	}

private://メンバ変数
	EnCatchEnemyState m_catchEnemyState = enStartWireToEnemy;//敵をキャッチする状態
	Vector3 m_distance = Vector3::Zero;//距離
	Vector3 m_startGoOnEnemyPos = Vector3::Zero;//敵の上に行くときの最初の座標
	float m_goOnEnemyTimer = 0.0f;//敵の上に行くまでのタイマー
	bool m_isCatchEnemy = false;//敵をキャッチしているか？
	Player* m_player = nullptr;//プレイヤー用のインスタンス
	PlayerSwingAction* m_playerSwingAction = nullptr;//プレイヤースイングアクション用のインスタンス
	SwingModel* m_swingModel = nullptr;//スイングモデル用のインスタンス
	QteEvent* m_qteEvent = nullptr;//QTEイベント用のインスタンス
	Enemy* m_enemy = nullptr;//敵用のインスタンス
};

