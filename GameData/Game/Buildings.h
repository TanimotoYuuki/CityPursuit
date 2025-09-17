#pragma once
/// <summary>
/// 建物クラス
/// </summary>
class Buildings : public IGameObject
{
public:
	Buildings() {};//コンストラクタ
	~Buildings() {};//デストラクタ

	bool Start();//開始処理
	void Update();//更新処理
	void Render(RenderContext& rc);//描画処理

	//建物の種類
	enum EnBuildingsType
	{
		enBuildingsType_Building,//ビル
		enBuildingsType_Tower,//タワー
		enBuildingsType_Num//建物の種類
	};

	/// <summary>
	/// 建物モデルの名前の設定
	/// </summary>
	/// <param name="buildingsModelName">ビルの名前</param>
	void SetBuildingsModelName(std::string buildingsModelName)
	{
		m_buildingsModelName = buildingsModelName;
	}

	/// <summary>
	/// 建物のタイプの設定
	/// </summary>
	/// <param name="enBuildingsType">建物の名前</param>
	void SetBuildingModelType(EnBuildingsType enBuildingsType)
	{
		m_buildingsType = enBuildingsType;
	}

	/// <summary>
	/// 建物の位置の設定
	/// </summary>
	/// <param name="position">位置</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	/// <summary>
	/// 建物の回転の設定
	/// </summary>
	/// <param name="rotation">回転</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	/// <summary>
	/// 建物の大きさの設定
	/// </summary>
	/// <param name="scale">大きさ</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

private://メンバ変数
	int m_buildingsType = enBuildingsType_Building;//建物のタイプ
	ModelRender m_buildingsModel[enBuildingsType_Num];//ビルモデル
	std::string m_buildingsModelName = "building001";//建物モデルの名前
	std::string m_buildingsModelPath = "";//建物モデルのファイルパス
	Vector3 m_position = Vector3::Zero;//建物の位置
	Quaternion m_rotation = Quaternion::Identity;//建物の回転
	Vector3 m_scale = Vector3::One;//建物の大きさ
};

