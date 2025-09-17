#pragma once
/// <summary>
/// プレイヤークラス
/// </summary>
class Player : public IGameObject
{
public:
	Player() {};//コンストラクタ
	~Player() {};//デストラクタ

	bool Start();//開始処理
	void Update();//更新処理
	void Render(RenderContext& rc);//描画処理

private://メンバ変数
	ModelRender m_playerModel;//プレイヤークラス
	Vector3 m_position = Vector3::Zero;//プレイヤーの位置
	Quaternion m_rotation = Quaternion::Identity;//プレイヤーの回転
	Vector3 m_scale = Vector3::One;//プレイヤーの大きさ
};

