#pragma once
/// <summary>
/// 敵を初めて発見したときの演出クラス
/// </summary>
class Player;
class Enemy;
class FirstEnemyDirection : public IGameObject
{
public:
	FirstEnemyDirection() {};//コンストラクタ
	~FirstEnemyDirection() {};//デストラクタ

	bool Start();//開始処理
	void Update();//更新処理
	void Render(RenderContext& rc);//描画処理

public://列挙型

	//敵を初めて発見したときの演出の状態
	enum EnFirstEnemyDirectionState
	{
		enFirstEnemyDirectionState_CameraStart,//カメラ(演出開始時)
		enFirstEnemyDirectionState_DrawUI,//UI描画
		enFirstEnemyDirectionState_CameraEnd,//カメラ(演出終了時)
		enFirstEnemyDirectionState_Num//演出数
	};

private://メンバ関数

	/// <summary>
	/// 敵テキストUIの初期化
	/// </summary>
	void InitEnemyTextUI();

	/// <summary>
	/// 矢印UIの初期化
	/// </summary>
	void InitArrowUI();

	/// <summary>
	/// バネカメラの初期化
	/// </summary>
	void InitSpringCamera();

	/// <summary>
	/// カメラ演出の更新処理(演出開始時)
	/// </summary>
	/// <param name="playerData">プレイヤーのデータ</param>
	/// <param name="enemyData">敵のデータ</param>
	void CameraStartDirectionUpdate(Player* playerData, Enemy* enemyData);

	/// <summary>
	/// UI描画の演出の更新処理
	/// </summary>
	void DrawUIDrectionUpdate();

	/// <summary>
	/// カメラ演出の更新処理(演出終了時)
	/// </summary>
	/// <param name="playerData">プレイヤーのデータ</param>
	/// <param name="enemyData">敵のデータ</param>
	void CameraEndDirectionUpdate(Player* playerData, Enemy* enemyData);

public://メンバ関数

	/// <summary>
	/// 敵を初めて発見したときの演出の状態の取得
	/// </summary>
	/// <returns>敵を初めて発見したときの演出の状態</returns>
	EnFirstEnemyDirectionState GetFirstEnemyDirectionState() const
	{
		return m_firstEnemyDirectionState;
	}

	/// <summary>
	/// プレイヤークラスのポインタの設定
	/// </summary>
	/// <param name="player">プレイヤークラスのポインタ</param>
	void SetPlayerPtr(Player* player)
	{
		m_player = player;
	}

	/// <summary>
	/// 敵クラスのポインタの設定
	/// </summary>
	/// <param name="enemy">敵クラスのポインタ</param>
	void SetEnemyPtr(Enemy* enemy)
	{
		m_enemy = enemy;
	}

private://メンバ関数
	SpriteRender m_enemyTextUI;//敵テキストUI
	SpriteRender m_arrowUI;//矢印UI
	SpringCamera m_directionCamera;//演出用のバネカメラ
	EnFirstEnemyDirectionState m_firstEnemyDirectionState = enFirstEnemyDirectionState_CameraStart;//敵を初めて発見したときの演出の状態
	float m_cameraLarpRate = 0.0f;//カメラの補完率
	float m_arrowUIMoveTime = 0.0f;//矢印UIの動作経過時間
	Vector3 m_cameraTarget = Vector3::Zero;//カメラの注視点
	Vector3 m_cameraPosition = Vector3::Zero;//カメラの視点
	Vector3 m_larpCameraTarget = Vector3::Zero;//補完用のカメラの注視点
	Vector3 m_larpCameraPosition = Vector3::Zero;//補完用のカメラの視点
	Player* m_player = nullptr;//プレイヤー用のインスタンス
	Enemy* m_enemy = nullptr;//敵用のインスタンス
};

