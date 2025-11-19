#pragma once
/// <summary>
/// ゲーム全体を管理するクラス
/// </summary>
class BackGround;
class Player;
class GameTimeLimit;
class GameMission;
class MiniMap;
class GameStartSprite;
class DebugLog;
class Game : public IGameObject
{
public:
	Game() {};//コンストラクタ
	~Game();//デストラクタ

	bool Start();//開始処理
	void Update();//更新処理
	void Render(RenderContext& rc);

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
	bool m_isGameEnd = false;//ゲーム終了したか?
	bool m_isGameStartDirection = false;//ゲームスタート演出しているか?
	bool m_isFinishGameStartDirection = false;//ゲームスタート演出が終了しているか?
	BackGround* m_backGround = nullptr;//背景用のインスタンス
	Player* m_player = nullptr;//プレイヤー用のインスタンス
	GameTimeLimit* m_gameTimeLimit = nullptr;//ゲームの制限時間を管理する用のインスタンス
	GameMission* m_gameMission = nullptr;//ゲームミッションのスプライトを表示する用のインスタンス
	GameStartSprite* m_gameStartSprite = nullptr;//ゲームスタートのスプライトを表示する用のインスタンス
	MiniMap* m_miniMap = nullptr;//ミニマップ用のインスタンス
	DebugLog* m_debugLog = nullptr;//デバッグログ用のインスタンス
};

