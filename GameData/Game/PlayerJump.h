#pragma once
/// <summary>
/// プレイヤージャンプクラス
/// </summary>
class PlayerJump : Noncopyable
{
public:
	PlayerJump() {};//コンストラクタ
	~PlayerJump() {};//デストラクタ

	/// <summary>
	/// ジャンプ処理の実行
	/// </summary>
	/// <param name="moveSpeed">移動速度</param>
	void Execute(Vector3& moveSpeed);
};

