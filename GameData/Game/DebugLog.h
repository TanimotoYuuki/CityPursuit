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

	/// <summary>
	/// デバッグログの更新処理
	/// </summary>
	void DebugLogUpdate();

	/// <summary>
	/// デバッグする名前の設定
	/// </summary>
	/// <param name="name">デバッグする名前</param>
	void SetDebugName(std::string name)
	{
		m_debugLogName.push_back(name);
	}

	/// <summary>
	/// デバッグログの出力に必要なデータの設定
	/// </summary>
	/// <param name="value">int型のデータ</param>
	void SetDebugLogData(std::string name, int value)
	{
		m_intDebugLogData[name] = value;
	}

	/// <summary>
	/// デバッグログの出力に必要なデータの設定
	/// </summary>
	/// <param name="value">int型のデータ</param>
	void SetDebugLogData(std::string name, float value)
	{
		m_floatDebugLogData[name] = value;
	}

	/// <summary>
	/// デバッグログの出力に必要なデータの設定
	/// </summary>
	/// <param name="value">int型のデータ</param>
	void SetDebugLogData(std::string name, const Vector3& value)
	{
		m_vector3DebugLogData[name] = value;
	}

private://メンバ変数
	std::array<FontRender, 100> m_debugLog;//デバッグログ
	std::map<std::string, int> m_intDebugLogData;//int型格納用のデバッグログのデータ
	std::map<std::string, float> m_floatDebugLogData;//float格納用のデバッグログのデータ
	std::map<std::string, Vector3> m_vector3DebugLogData;//Vector3格納用のデバッグログ用のデータ
	std::vector<std::string> m_debugLogName;//デバッグログの名前
	int m_debugLogID = 0;//デバッグログID
	Vector3 m_DebugLogPosition = Vector3{ 300.0f, 500.0f, 0.0f };//デバッグログの表示位置
};

