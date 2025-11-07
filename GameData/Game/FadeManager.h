#pragma once
class FadeManager : public IGameObject
{
public:
	FadeManager() {};//コンストラクタ
	~FadeManager() {};//デストラクタ

	bool Start();//開始処理
	void Update();//更新処理
	void Render(RenderContext& rc);//描画処理

public://列挙型

	//フェードの状態
	enum EnFadeState
	{
		enFadeState_FadeIn,
		enFadeState_FadeOut,
		enFadeState_None
	};

public://メンバ関数

	/// <summary>
	/// フェードの状態の設定
	/// </summary>
	/// <param name="createSceneID">フェードの状態</param>
	void SetFadeState(EnFadeState enFadeState)
	{
		m_fadeState = enFadeState;
	}

	/// <summary>
	/// フェードの状態の取得
	/// </summary>
	/// <returns>フェードの状態</returns>
	EnFadeState GetFadeState() const
	{
		return m_fadeState;
	}

	/// <summary>
	/// フェード処理が終わったか?
	/// </summary>
	/// <returns>trueならフェード処理が終わっている</returns>
	bool IsFinishFade() const
	{
		return m_isFinishFade;
	}

public://シングルトン用

	/// <summary>
	/// シングルトンインスタンスの生成
	/// </summary>
	static void CreateInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = NewGO<FadeManager>(1, "fademanager");
		}
	}

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static FadeManager* GetInstance()
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

private://メンバ変数
	SpriteRender m_fade;//フェード
	EnFadeState m_fadeState = enFadeState_None;//フェードステート
	float m_currentFadeAlpha = 0.0f;//現在のフェードの透明度
	bool m_isFinishFade = false;//フェード処理が終わったか?
	const std::string m_fadeFilePath = "Assets/sprite/fade/fade.dds";//フェードのファイルパス
	static FadeManager* m_instance;//シングルトンインスタンス
};

