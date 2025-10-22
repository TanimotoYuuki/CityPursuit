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
	/// <param name="characterController">キャラクターコントローラー</param>
	void Execute(Vector3& moveSpeed, CharacterController& characterController);

	/// <summary>
	/// ジャンプしているか?
	/// </summary>
	/// <returns>trueならジャンプしている</returns>
	bool IsJump() const
	{
		return m_isJump;
	}

	/// <summary>
	/// ジャンプ終了
	/// </summary>
	void JumpEnd()
	{
		m_isJump = false;
	}

private:
	bool m_isJump = false;//ジャンプしているか?
};

