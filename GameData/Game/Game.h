#pragma once
/// <summary>
/// ゲーム全体を管理するクラス
/// </summary>
class BackGround;
class Player;
class GameTimeLimit;
class DebugLog;
class Game : public IGameObject
{
public:
	Game() {};//コンストラクタ
	~Game() {};//デストラクタ

	bool Start();//開始処理
	void Update();//更新処理
	void Render(RenderContext& rc);

public://メンバ関数

	/// <summary>
	/// ゲームクリアをしたか?
	/// </summary>
	/// <returns>trueならゲームクリアをしている</returns>
	bool IsGameClear()
	{
		return m_isGameClear;
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

private://メンバ変数
	int m_qteEventSuccessCount = 0;//QTEイベントで成功した回数
	int m_qteEventFailedCount = 0;//QTEイベントで失敗した回数
	bool m_isGameClear = false;//ゲームクリアをしたか?
	BackGround* m_backGround = nullptr;//背景用のインスタンス
	Player* m_player = nullptr;//プレイヤー用のインスタンス
	GameTimeLimit* m_gameTimeLimit = nullptr;//ゲームの制限時間を管理する用のインスタンス
	DebugLog* m_debugLog = nullptr;//デバッグログ用のインスタンス
};

