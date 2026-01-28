#pragma once
/// <summary>
/// デバッグ用のクラス
/// </summary>
class DebugLog : public IGameObject
{
public:
	DebugLog() {};//コンストラクタ
	~DebugLog() {};//デストラクタ

	bool Start();//開始処理
	void Update();//更新処理
	void Render(RenderContext& rc);//描画処理

private://メンバ関数

	/// <summary>
	/// デバッグログの更新処理
	/// </summary>
	void DebugLogUpdate();

public://メンバ関数

	/// <summary>
	/// デバッグログの出力に必要なデータの設定
	/// </summary>
	/// <param name="debugName">デバッグする名前</param>
	/// <param name="value">int型のデータ</param>
	void SetDebugLogData(std::string debugName, int value)
	{
		auto debugNameIt = std::find(m_debugLogName.begin(), m_debugLogName.end(), debugName);
		if (debugNameIt == m_debugLogName.end()) { m_debugLogName.push_back(debugName); }
		m_intDebugLogData[debugName] = value;
	}

	/// <summary>
	/// デバッグログの出力に必要なデータの設定
	/// </summary>
	/// <param name="debugName">デバッグする名前</param>
	/// <param name="value">int型のデータ</param>
	void SetDebugLogData(std::string debugName, float value)
	{
		auto debugNameIt = std::find(m_debugLogName.begin(), m_debugLogName.end(), debugName);
		if (debugNameIt == m_debugLogName.end()) { m_debugLogName.push_back(debugName); }
		m_floatDebugLogData[debugName] = value;
	}

	/// <summary>
	/// デバッグログの出力に必要なデータの設定
	/// </summary>
	/// <param name="debugName">デバッグする名前</param>
	/// <param name="value">int型のデータ</param>
	void SetDebugLogData(std::string debugName, const Vector3& value)
	{
		auto debugNameIt = std::find(m_debugLogName.begin(), m_debugLogName.end(), debugName);
		if (debugNameIt == m_debugLogName.end()) { m_debugLogName.push_back(debugName); }
		m_vector3DebugLogData[debugName] = value;
	}

private://メンバ変数
	std::array<FontRender, 100> m_debugLog;//デバッグログ
	std::map<std::string, int> m_intDebugLogData;//int型格納用のデバッグログのデータ
	std::map<std::string, float> m_floatDebugLogData;//float格納用のデバッグログのデータ
	std::map<std::string, Vector3> m_vector3DebugLogData;//Vector3格納用のデバッグログ用のデータ
	std::vector<std::string> m_debugLogName;//デバッグログの名前
	int m_debugLogID = 0;//デバッグログID
	Vector3 m_debugLogPosition = Vector3{ 300.0f, 500.0f, 0.0f };//デバッグログの表示位置
	
private:
	static DebugLog* m_instance;//シングルトンインスタンス

public:
	static void CreateInstance()//シングルトンインスタンスの生成
	{
		if (m_instance == nullptr)
		{
			m_instance = NewGO<DebugLog>(0, "debuglog");
		}
	}
	static DebugLog* GetInstance()//シングルトンインスタンスの取得
	{
		return m_instance;
	}
	static void DeleteInstance()//シングルトンインスタンスの削除
	{
		if (m_instance != nullptr)
		{
			delete m_instance;
			m_instance = nullptr;
		}
	}
};

