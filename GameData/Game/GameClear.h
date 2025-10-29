#pragma once
/// <summary>
/// ゲームクリアクラス
/// </summary>
class Player;
class GameClearCamera;
class GameClear : public IGameObject
{
public:
	GameClear() {};//コンストラクタ
	~GameClear();//デストラクタ

	bool Start();//開始処理
	void Update();//更新処理

public://メンバ関数

	/// <summary>
	/// プレイヤークラスのポインタの設定
	/// </summary>
	/// <param name="player">プレイヤークラスのポインタの設定</param>
	void SetPlayerPtr(Player* player)
	{
		m_player = player;
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
	/// ゲームクリアしたか?
	/// </summary>
	/// <returns>trueならゲームクリアしている</returns>
	bool IsGameClear() const
	{
		return m_isGameClear;
	}

private://メンバ変数
	bool m_isGameClear = false;//ゲームクリアしたか?
	Player* m_player = nullptr;//ゲームクリア用のインスタンス
	GameClearCamera* m_gameClearCamera = nullptr;//ゲームクリア時のカメラ用のインスタンス
};

