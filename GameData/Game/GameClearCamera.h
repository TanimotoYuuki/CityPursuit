#pragma once
/// <summary>
/// ゲームクリア時のカメラ用のクラス
/// </summary>
class Player;
class GameClear;
class GameClearCamera : public IGameObject
{
public:
	GameClearCamera() {};
	~GameClearCamera() {};

	bool Start();//開始処理委

public://メンバ関数

	/// <summary>
	/// ゲームクリア時のカメラの実行処理
	/// </summary>
	void Execute();

	/// <summary>
	/// ゲームクリアクラスのポインタの設定
	/// </summary>
	/// <param name="gameclear">ゲームクリアクラスのポインタ</param>
	void SetGameClearPtr(GameClear* gameclear)
	{
		m_gameClear = gameclear;
	}

	/// <summary>
	/// ゲームクリアクラスのポインタの取得
	/// </summary>
	/// <returns>ゲームクリアクラス</returns>
	GameClear* GetGameClearPtr()
	{
		return m_gameClear;
	}

	/// <summary>
	/// カメラの回転が終わったか?
	/// </summary>
	/// <returns>trueならカメラの回転が終わっている</returns>
	bool IsFinishRotationCamera()
	{
		return m_isFinishRotationCamera;
	}

private://メンバ変数
	SpringCamera m_gameClearCamera;//ゲームクリア時のカメラ
	Vector3 m_cameraTarget = Vector3::Zero;//カメラの注視点
	Vector3 m_cameraPosition = Vector3::Zero;//カメラの位置
	Vector3 m_playerPosition = Vector3::Zero;//プレイヤーの位置
	Vector3 m_cameraToPlayer = Vector3::Zero;//カメラからプレイヤーまでの距離
	float m_angle = 0.0f;//角度
	float m_currentAngleDeg = 0.0f;//現在の角度
	float m_rotationAngleDeg = 0.0f;//回転する角度
	bool m_isFinishRotationCamera = false;//カメラの回転が終わったか?
	Player* m_player = nullptr;//プレイヤー用のインスタンス
	GameClear* m_gameClear = nullptr;//ゲームクリア用のインスタンス
};

