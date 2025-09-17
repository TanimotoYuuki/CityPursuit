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
	void Execute(CharacterController& characterController,Vector3& moveSpeed);
};

