#pragma once
/// <summary>
/// ゲームクリアクラス
/// </summary>
class Game;
class GameClearSprite;
class GameClearCamera;
class GameResult;
class GameEndSelect;
class GameClear : public IGameObject
{
public:
	GameClear() {};//コンストラクタ
	~GameClear();//デストラクタ

	bool Start();//開始処理
	void Update();//更新処理

public://列挙型

	//ゲームクリアの状態
	enum EnGameClearState
	{
		enGameClearState_GameClearDirection,//ゲームクリア演出
		enGameClearState_Result,//リザルト
		enGameClearState_Select,//選択
		enGameClearState_TransitionScene,//シーン遷移
		enGameClearState_Num//ゲームクリアの状態数
	};

private://メンバ変数

	/// <summary>
	/// ゲームクリア演出の更新処理
	/// </summary>
	void GameClearDirectionUpdate();

	/// <summary>
	/// リザルトの更新処理
	/// </summary>
	void ResultUpdate();

	/// <summary>
	/// 選択の更新処理
	/// </summary>
	void SelectUpdate();

	/// <summary>
	/// シーンの遷移の更新処理
	/// </summary>
	void TransitionSceneUpdate();

public://メンバ関数

	/// <summary>
	/// ゲーム全体を管理するクラスのポインタの設定
	/// </summary>
	/// <param name="player">ゲーム全体を管理するクラスのポインタの設定</param>
	void SetGamePtr(Game* game)
	{
		m_game = game;
	}
	
	/// <summary>
	/// ゲーム全体を管理するクラスのポインタの取得
	/// </summary>
	/// <returns>ゲーム全体を管理するクラスのポインタ</returns>
	Game* GetGamePtr()
	{
		return m_game;
	}

	/// <summary>
	/// ゲームクリア時のカメラクラスのポインタの取得
	/// </summary>
	/// <returns>ゲームクリア時のカメラクラスのポインタ</returns>
	GameClearCamera* GetGameClearCamera()
	{
		return m_gameClearCamera;
	}

	/// <summary>
	/// ゲームクリアのスプライトを表示するクラスのポインタの取得
	/// </summary>
	/// <returns>ゲームクリアのスプライトを表示するクラスのポインタ</returns>
	GameClearSprite* GetGameClearSprite()
	{
		return m_gameClearSprite;
	}

private://メンバ変数
	EnGameClearState m_gameClearState = enGameClearState_GameClearDirection;//ゲームクリアの状態
	Game* m_game = nullptr;//ゲーム全体を管理する用のインスタンス
	GameClearSprite* m_gameClearSprite = nullptr;//ゲームクリアのスプライトを表示する用のインスタンス
	GameClearCamera* m_gameClearCamera = nullptr;//ゲームクリア時のカメラ用のインスタンス
	GameResult* m_gameResult = nullptr;//ゲームリザルト用のインスタンス
	GameEndSelect* m_gameEndSelect = nullptr;//ゲーム終了したときに選択する用のインスタンス
};

