#pragma once
/// <summary>
/// ゲーム全体を管理するクラス
/// </summary>
class BackGround;
class Player;
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
	/// プレイヤークラスのポインタの取得
	/// </summary>
	/// <returns>プレイヤークラスのポインタ</returns>
	Player* GetPlayerPtr()
	{
		return m_player;
	}

private://メンバ変数
	bool m_isGameClear = false;//ゲームクリアをしたか?
	BackGround* m_backGround = nullptr;//背景用のインスタンス
	Player* m_player = nullptr;//プレイヤー用のインスタンス
	DebugLog* m_debugLog = nullptr;//デバッグログ用のインスタンス
};

