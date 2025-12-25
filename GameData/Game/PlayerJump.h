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
	/// ジャンプ開始
	/// </summary>
	void JumpStart()
	{
		m_isJump = true;
	}

	/// <summary>
	/// ジャンプ終了
	/// </summary>
	void JumpEnd()
	{
		m_isJump = false;
	}

	/// <summary>
	/// 地面に付いている
	/// </summary>
	void OnGround()
	{
		m_isOnGround = true;
	}

	/// <summary>
	/// 地面に付いているか?
	/// </summary>
	/// <returns>trueなら地面に付いている</returns>
	bool IsOnGround() const
	{
		return m_isOnGround;
	}

private:
	bool m_isJump = false;//ジャンプしているか?
	bool m_isOnGround = true;//地面に付いているか?
};

