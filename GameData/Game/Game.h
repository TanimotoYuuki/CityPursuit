#pragma once
/// <summary>
/// ゲーム全体を管理するクラス
/// </summary>
class GameBackGround;
class Player;
class GameTimeLimit;
class GameMission;
class MiniMap;
class GameStartSprite;
class FirstEnemyDirection;
class InvisibleWall;
class DebugLog;
class Game : public IGameObject
{
public:
	Game() {};//コンストラクタ
	~Game();//デストラクタ

	bool Start();//開始処理
	void Update();//更新処理
	void Render(RenderContext& rc);

private://メンバ関数

	/// <summary>
	/// ロードの更新処理
	/// </summary>
	void LoadingUpdate();

	/// <summary>
	/// インゲーム用のデルタタイムの更新処理
	/// </summary>
	void InGameDeltaTimeUpdate();

	/// <summary>
	/// 視錐台を用いた当たり判定の更新処理
	/// </summary>
	void ViewFrustumCollisionUpdate();

public://メンバ関数

	/// <summary>
	/// ゲーム終了したか?
	/// </summary>
	/// <returns>trueならゲーム終了している</returns>
	bool IsGameEnd()
	{
		return m_isGameEnd;
	}

	/// <summary>
	/// QTEイベントで成功した回数のカウントアップ
	/// </summary>
	void QteEventSuccessCountUp()
	{
		m_qteEventSuccessCount++;
	}

	/// <summary>
	/// QTEイベントで失敗した回数のカウントアップ
	/// </summary>
	void QteEventFailedCountUp()
	{
		m_qteEventFailedCount++;
	}

	/// <summary>
	/// QTEイベントで成功した回数の取得
	/// </summary>
	/// <returns>QTEイベントで成功した回数</returns>
	int GetQteEventSuccessCount()
	{
		return m_qteEventSuccessCount;
	}

	/// <summary>
	/// QTEイベントで失敗した回数の取得
	/// </summary>
	/// <returns>QTEイベントで失敗した回数</returns>
	int GetQteEventFailedCount()
	{
		return m_qteEventFailedCount;
	}

	/// <summary>
	/// インゲーム用のデルタタイムの取得
	/// </summary>
	/// <returns>インゲーム用のデルタタイム</returns>
	float GetInGameDeltaTime()
	{
		return m_inGameDeltaTime;
	}

	/// <summary>
	/// ゲームスタート演出が終了しているか?
	/// </summary>
	/// <returns>trueならゲームスタート演出が終了している</returns>
	bool IsFinishGameStartDirection() const
	{
		return m_isFinishGameStartDirection;
	}

	/// <summary>
	/// 敵を初めて発見したときの演出しているか?
	/// </summary>
	/// <returns>trueなら敵を初めて発見したときの演出している</returns>
	bool IsFirstEnemyDirection() const
	{
		return m_firstEnemyDirection != nullptr ? true : false;
	}

	/// <summary>
	/// プレイヤークラスのポインタの取得
	/// </summary>
	/// <returns>プレイヤークラスのポインタ</returns>
	Player* GetPlayerPtr()
	{
		return m_player;
	}

	/// <summary>
	/// ゲームの制限時間を管理するクラスのポインタの取得
	/// </summary>
	/// <returns>ゲームの制限時間を管理するクラスのポインタ</returns>
	GameTimeLimit* GetGameTimeLimitPtr()
	{
		return m_gameTimeLimit;
	}

	/// <summary>
	/// ゲームミッションのスプライトを表示するクラスのポインタの取得
	/// </summary>
	/// <returns></returns>
	GameMission* GetGameMissionPtr()
	{
		return m_gameMission;
	}

	/// <summary>
	/// ミニマップクラスのポインタの取得
	/// </summary>
	/// <returns>ミニマップクラスのポインタ</returns>
	MiniMap* GetMiniMapPtr()
	{
		return m_miniMap;
	}

private://メンバ変数
	int m_qteEventSuccessCount = 0;//QTEイベントで成功した回数
	int m_qteEventFailedCount = 0;//QTEイベントで失敗した回数
	float m_inGameDeltaTime = 0.0f;//インゲーム用のデルタタイム
	bool m_isGameEnd = false;//ゲーム終了したか?
	bool m_isFinsishFadeIn = false;//フェードインが終わっているか?
	bool m_isGameStartDirection = false;//ゲームスタート演出しているか?
	bool m_isStartGameStartDirection = false;//ゲームスタート演出が開始しているか?
	bool m_isFinishGameStartDirection = false;//ゲームスタート演出が終了しているか?
	bool m_isLoadComplete = false;//ロードが完了したか?
	bool m_isFirstEnemySee = false;//初めて敵を発見したか?
	ViewFrustum m_viewFrustum;//視錐台
	GameBackGround* m_gameBackGround = nullptr;//ゲーム背景用のインスタンス
	Player* m_player = nullptr;//プレイヤー用のインスタンス
	GameTimeLimit* m_gameTimeLimit = nullptr;//ゲームの制限時間を管理する用のインスタンス
	GameMission* m_gameMission = nullptr;//ゲームミッションのスプライトを表示する用のインスタンス
	GameStartSprite* m_gameStartSprite = nullptr;//ゲームスタートのスプライトを表示する用のインスタンス
	FirstEnemyDirection* m_firstEnemyDirection = nullptr;//敵を初めて発見したときの演出用のインスタンス
	MiniMap* m_miniMap = nullptr;//ミニマップ用のインスタンス
	InvisibleWall* m_invisibleWall = nullptr;//見えない壁用のインスタンス
	DebugLog* m_debugLog = nullptr;//デバッグログ用のインスタンス
};

