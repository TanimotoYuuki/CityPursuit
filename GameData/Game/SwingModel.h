#pragma once
/// <summary>
/// スイングモデルクラス
/// </summary>
class Player;
class SwingModel : public IGameObject
{
public:
	SwingModel() {};//コンストラクタ
	~SwingModel() {};//デストラクタ

	bool Start();//開始処理
	void Update();//描画処理
	void Render(RenderContext& rc);//描画処理

	/// <summary>
	/// 指定した位置に向かって伸びる処理を開始する
	/// </summary>
	/// <param name="position">伸びる先の位置</param>
	void StartWireStretchToPos(const Vector3& position);

	/// <summary>
	/// 指定した座標に向かって伸びる処理を終了する
	/// </summary>
	void EndWireStretchToPos();

	/// <summary>
	/// 伸びきっているか？
	/// </summary>
	/// <returns>trueなら伸びきっている</returns>
	bool IsWireStretched() const
	{
		return m_isStretched;
	}

	/// <summary>
	/// 糸の長さの取得
	/// </summary>
	/// <returns>糸の長さ</returns>
	float GetStringLength() const
	{
		return m_swingModel.GetScale().z;
	}

	/// <summary>
	/// 伸びる先の位置を設定する
	/// </summary>
	/// <param name="position">位置</param>
	void SetToStretchPos(const Vector3& position)
	{
		m_toStretchPos = position;
	}

	/// <summary>
	/// モデルを描画する
	/// </summary>
	void EnableDrawingModel()
	{
		m_isDrawingModel = true;
	}

	/// <summary>
	/// モデルを描画しない
	/// </summary>
	void DisableDrawingModel()
	{
		m_isDrawingModel = false;
	}

private://メンバ関数
	/// <summary>
	/// モデルを回転させる処理
	/// </summary>
	void ModelRotation();

private://メンバ変数
	ModelRender m_swingModel;//スイングモデル
	bool m_isStretched = true;//伸びきっているか？
	Vector3 m_startStretchPos = Vector3::Zero;//伸びる開始座標
	Vector3 m_toStretchPos = Vector3::Zero;//伸びる先の座標
	float m_stretchSpeed = 0.0f;//伸びる速度
	float m_stretchRate = 0.0f;
	bool m_isDrawingModel = false;//モデルを描画しているか?
	Player* m_player = nullptr;//プレイヤー用のインスタンス
};

