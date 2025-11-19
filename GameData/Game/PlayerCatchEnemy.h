#pragma once
/// <summary>
/// プレイヤーが敵をキャッチするクラス
/// </summary>
class Player;
class PlayerSwingAction;
class SwingModel;
class Enemy;
class QteEvent;
class Game;
class PlayerCatchEnemy : public IGameObject
{
public:
	PlayerCatchEnemy() {};
	~PlayerCatchEnemy();

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
	/// <param name="enemy">敵</param>
	void StartWireToEnemy(Enemy* enemy);

	/// <summary>
	/// 敵の方を向かせる
	/// </summary>
	/// <param name="enemy">敵</param>
	void LookAtEnemy(Enemy* enemy);

	/// <summary>
	/// 敵に向かって糸を伸ばす処理
	/// </summary>
	/// <param name="enemy">敵</param>
	void WireingToEnemy(Enemy* enemy);

	/// <summary>
	/// 敵の上に行く処理
	/// </summary>
	/// <param name="enemy">敵</param>
	void GoOnEnemy(Enemy* enemy);

	/// <summary>
	/// 敵の上にいる処理
	/// </summary>
	/// <param name="enemy">敵</param>
	void OnEnemy(Enemy* enemy);

	/// <summary>
	/// ステートを変更する
	/// </summary>
	/// <param name="newState">新しいステート</param>
	void ChangeState(Enemy* enemy, const EnCatchEnemyState newState);

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
	/// QTEイベント開始
	/// </summary>
	void StartQteEvent()
	{
		m_isQteEvent = true;
	}

	/// <summary>
	/// QTEイベント終了
	/// </summary>
	void FinishQteEvent()
	{
		m_isQteEvent = false;
	}

	/// <summary>
	/// 敵をキャッチしているか
	/// </summary>
	/// <returns>trueならキャッチしている</returns>
	bool IsCatchingEnemy()
	{
		return m_isCatchEnemy;
	}

	/// <summary>
	/// QTEイベント中か?
	/// </summary>
	/// <returns>trueならQTEイベント中</returns>
	bool IsQteEvent()
	{
		return m_isQteEvent;
	}

	/// <summary>
	/// キャッチした敵のインスタンスの取得
	/// </summary>
	/// <returns>キャッチした敵</returns>
	Enemy* GetCatchEnemy()
	{
		return m_catchEnemy;
	}

private://メンバ変数
	EnCatchEnemyState m_catchEnemyState = enStartWireToEnemy;//敵をキャッチする状態
	Vector3 m_startGoOnEnemyPos = Vector3::Zero;//敵の上に行くときの最初の座標
	float m_goOnEnemyTimer = 0.0f;//敵の上に行くまでのタイマー
	bool m_isInputCatchEnemy = false;//敵をキャッチする入力しているか?
	bool m_isCatchEnemy = false;//敵をキャッチしているか？
	bool m_isQteEvent = false;//QTEイベント中か?
	Player* m_player = nullptr;//プレイヤー用のインスタンス
	PlayerSwingAction* m_playerSwingAction = nullptr;//プレイヤースイングアクション用のインスタンス
	SwingModel* m_swingModel = nullptr;//スイングモデル用のインスタンス
	QteEvent* m_qteEvent = nullptr;//QTEイベント用のインスタンス
	Enemy* m_catchEnemy = nullptr;//キャッチした敵用のインスタンス
	Game* m_game = nullptr;//ゲーム全体を管理する用のインスタンス
};

