#pragma once
/// <summary>
/// プレイヤー回転クラス
/// </summary>
class PlayerMove;
class PlayerRotation : public IGameObject
{
public:
	PlayerRotation() {};//コンストラクタ
	~PlayerRotation() {};//デストラクタ

	bool Start();//開始処理

	/// <summary>
	/// 回転処理の実行
	/// </summary>
	void Execute(Quaternion& rotation);

private:
	PlayerMove* m_playerMove = nullptr;//プレイヤー移動用のインスタンス
};

