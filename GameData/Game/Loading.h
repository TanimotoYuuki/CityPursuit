#pragma once
/// <summary>
/// ローディングクラス
/// </summary>
class Loading : public IGameObject
{
public:
	Loading() {};//コンストラクタ
	~Loading() {};//デストラクタ

	bool Start();//開始処理
	void Update();//更新処理
	void Render(RenderContext& rc);//描画処理

private://列挙型

	//ローディングUIの構造
	enum EnLoadingStructure
	{
		enLoadingStructure_Icon,//アイコン
		enLoadingStructure_RingOne,//リング(アイコンから一番目)
		enLoadingStructure_RingTwo,//リング(アイコンから二番目)
		enLoadingStructure_RingThree,//リング(アイコンから三番目)
		enLoadingStructure_Num//ローディング構造数
	};

private://メンバ関数

	/// <summary>
	/// ローディングUIを初期化
	/// </summary>
	/// <param name="enLoadingStructure">ローディングUIの構造</param>
	void InitLoadingUI(EnLoadingStructure enLoadingStructure);

	/// <summary>
	/// ローディングUIの回転の更新処理
	/// </summary>
	void LoadingUIRotationUpdate();

	/// <summary>
	/// ローディングUIの乗算カラーの更新処理
	/// </summary>
	void LoadingUIMulColorUpdate();

public://メンバ関数

	/// <summary>
	/// ローディング開始
	/// </summary>
	void StartLoading()
	{
		m_isLoading = true;
	}

	/// <summary>
	/// ローディング終了
	/// </summary>
	void FinishLoading()
	{
		m_isLoading = false;
	}

	/// <summary>
	/// ローディング中?
	/// </summary>
	/// <returns>trueならローディング中</returns>
	bool IsLoading()
	{
		return m_isLoading;
	}

private://メンバ変数
	SpriteRender m_loadingUI[enLoadingStructure_Num];//ローディングUI
	Quaternion m_leftRotation = Quaternion::Identity;//左回転
	Quaternion m_rightRotation = Quaternion::Identity;//右回転
	float m_elapsedTime = 0.0f;//経過時間
	bool m_isLoading = false;//ローディング中?

private://ローディングUIを表示する用のファイルパス
	const std::string m_loadingUIFilePath[enLoadingStructure_Num] = {
		"Assets/sprite/loading/icon.dds",
		"Assets/sprite/loading/ring1.dds",
		"Assets/sprite/loading/ring2.dds",
		"Assets/sprite/loading/ring3.dds"
	};

public://シングルトン用関数

	/// <summary>
	/// シングルトンインスタンスの生成
	/// </summary>
	static void CreateInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = NewGO<Loading>(2, "loading");
		}
	}

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static Loading* GetInstance()
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

private://シングルトン
	static Loading* m_instance;//シングルトンインスタンス
};

