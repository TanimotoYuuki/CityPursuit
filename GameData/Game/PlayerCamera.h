#pragma once
/// <summary>
/// プレイヤーカメラクラス
/// </summary>
class PlayerCamera : public IGameObject
{
public:
	PlayerCamera() {};//コンストラクタ
	~PlayerCamera() {};//デストラクタ

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// カメラ追従処理の実行
	/// </summary>
	void Execute(Vector3& position);

private://メンバ変数
	Vector3 m_toCameraPos = Vector3::Zero;//注視点から視点までのベクトル
};

