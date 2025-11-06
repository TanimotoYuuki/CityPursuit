#pragma once
/// <summary>
/// ゲームオーバークラス
/// </summary>
class Game;
class GameOverSprite;
class GameEndSelect;
class GameOver : public IGameObject
{
public:
	GameOver() {};//コンストラクタ
	~GameOver();//デストラクタ

	bool Start();//開始処理
	void Update();//更新処理

public://列挙型

	//ゲームオーバーの状態
	enum EnGameOverState
	{
		enGameOverState_TimeLimit,//時間切れ
		enGameOverState_Monochrome,//モノクロ化
		enGameOverState_GameOverText,//ゲームオーバーのテキスト
		enGameOverState_Select,//選択
		enGameOverState_Num//ゲームオーバーの状態数
	};

private://メンバ関数

	/// <summary>
	/// 時間切れの更新処理
	/// </summary>
	void TimeLimitUpdate();

	/// <summary>
	/// モノクロ化の更新処理
	/// </summary>
	void MonochromeUpdate();

	/// <summary>
	/// ゲームオーバーのテキストの更新処理
	/// </summary>
	void GameOverTextUpdate();

	/// <summary>
	/// 選択の更新処理
	/// </summary>
	void SelectUpdate();

public://メンバ関数

	/// <summary>
	/// ゲーム全体を管理するクラスのポインタの設定
	/// </summary>
	/// <param name="game">ゲーム全体を管理するクラスのポインタ</param>
	void SetGaamePtr(Game* game)
	{
		m_game = game;
	}

	/// <summary>
	/// ゲーム全体を管理するクラスのポインタの取得
	/// </summary>
	/// <returns>ゲーム全体を管理するクラスのポインタ</returns>
	Game* GetGamePtr() const
	{
		return m_game;
	}

private://メンバ変数
	EnGameOverState m_gameOverState = enGameOverState_TimeLimit;//ゲームオーバーの状態
	Game* m_game = nullptr;//ゲーム全体を管理する用のインスタンス
	GameOverSprite* m_gameOverSprite = nullptr;//ゲームオーバーのスプライトを表示する用のインスタンス
	GameEndSelect* m_gameEndSelect = nullptr;//ゲーム終了したときに選択する用のインスタンス
};

