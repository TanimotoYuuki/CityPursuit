#pragma once

class Game;
class GameOver;
class GameClear;
class IScene : public Noncopyable
{
public:
	IScene() {};//コンストラクタ
	virtual ~IScene() {};//仮想純粋のデストラクタ
	virtual bool Start() = 0;//開始処理
	virtual void Update() = 0;//更新処理

//列挙型
	//シーンID
	enum EnSceneID
	{
		enSceneID_InGame,
		enSceneID_GameOver,
		enSceneID_GameClear,
		enSceneID_None
	};

	Game* m_game = nullptr;//ゲーム用のインスタンス
	GameOver* m_gameOver = nullptr;//ゲームオーバー用のインスタンス
	GameClear* m_gameClear = nullptr;//ゲームクリア用のインスタンス
	EnSceneID m_createScene = enSceneID_None;//生成するシーン
	EnSceneID m_currentScene = enSceneID_None;//現在のシーン
};

class SceneManager : public IScene
{
public:
	SceneManager() {};//コンストラクタ
	~SceneManager() {};//デストラクタ

	bool Start();//開始処理
	void Update();//更新処理

public://メンバ変数

	/// <summary>
	/// シーンの生成
	/// </summary>
	/// <param name="createSceneID">生成するシーンID</param>
	void CreateScene(EnSceneID createSceneID)
	{
		m_createScene = createSceneID;
	}
	
public://シングルトン用

	/// <summary>
	/// シングルトンインスタンスの生成
	/// </summary>
	static void CreateInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new SceneManager();
		}
	}

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static SceneManager* GetInstance()
	{
		return m_instance;
	}

	/// <summary>
	/// シングルトンインスタンスの削除
	/// </summary>
	static void DestroyInstance()
	{
		if (m_instance != nullptr)
		{
			delete m_instance;
			m_instance = nullptr;
		}
	}

private:
	static SceneManager* m_instance;//シングルトンインスタンス
};

