#pragma once
#include "PlayerCamera.h"
#include "Game.h"
/// <summary>
/// プレイヤークラス
/// </summary>
class PlayerAnimation;
class PlayerMove;
class PlayerRotation;
class PlayerCatchEnemy;
class PlayerEffect;
class Title;
class Player : public IGameObject
{
public:
	Player() {};//コンストラクタ
	~Player();//デストラクタ

	bool Start();//開始処理
	void Update();//更新処理
	void Render(RenderContext& rc);//描画処理

public://メンバ関数

	/// <summary>
	/// プレイヤーの位置の設定
	/// </summary>
	/// <param name="position">プレイヤーの位置</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	/// <summary>
	/// プレイヤーの位置の取得
	/// </summary>
	/// <returns>プレイヤーの位置</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// <summary>
	/// プレイヤーの回転の設定
	/// </summary>
	/// <param name="rotation">プレイヤーの回転</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	/// <summary>
	/// プレイヤーの回転の取得
	/// </summary>
	/// <returns>プレイヤーの回転</returns>
	const Quaternion GetRotation() const
	{
		return m_rotation;
	}

	/// <summary>
	/// プレイヤーの大きさの設定
	/// </summary>
	/// <param name="scale">プレイヤーの大きさ</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

	/// <summary>
	/// プレイヤーの大きさの取得
	/// </summary>
	/// <returns>プレイヤーの大きさ</returns>
	const Vector3& GetScale() const
	{
		return m_scale;
	}

	/// <summary>
	/// タイトルクラスのポインタの設定
	/// </summary>
	/// <param name="title">タイトルクラスのポインタ</param>
	void SetTitlePtr(Title* title)
	{
		m_title = title;
	}

	/// <summary>
	/// ゲーム全体を管理するクラスのポインタの取得
	/// </summary>
	/// <param name="game">ゲーム全体を管理する</param>
	void SetGamePtr(Game* game)
	{
		m_game = game;
	}

	/// <summary>
	/// 直接座標を設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetDirectPosition(const Vector3& pos);

	/// <summary>
	/// プレイヤーモデルのデータの取得
	/// </summary>
	/// <returns>プレイヤーモデルのデータ</returns>
	ModelRender& GetModelData()
	{
		return m_playerModel;
	}

	/// <summary>
	/// キャラクターコントローラーの取得
	/// </summary>
	/// <returns>キャラクターコントローラー</returns>
	CharacterController& GetCharacterController()
	{
		return m_charaCon;
	}

	/// <summary>
	/// プレイヤーアニメーション用のインスタンスの取得
	/// </summary>
	/// <returns>プレイヤーアニメーション用のインスタンス</returns>
	PlayerAnimation* GetPlayerAnimation()
	{
		return m_playerAnimation;
	}

	/// <summary>
	/// プレイヤー移動用のインスタンスの取得
	/// </summary>
	/// <returns>プレイヤー移動用のインスタンス</returns>
	PlayerMove* GetPlayerMove()
	{
		return m_playerMove;
	}

	/// <summary>
	/// プレイヤーが敵をキャッチする用のインスタンスの取得
	/// </summary>
	/// <returns>プレイヤーが敵をキャッチする用のインスタンス</returns>
	PlayerCatchEnemy* GetPlayerCatchEnemy()
	{
		return m_playerCatchEnemy;
	}

	/// <summary>
	/// プレイヤーのエフェクト用のインスタンスの取得
	/// </summary>
	/// <returns>プレイヤーのエフェクト用のインスタンス</returns>
	PlayerEffect* GetPlayerEffect()
	{
		return m_playerEffect;
	}

	/// <summary>
	/// プレイヤーカメラの取得
	/// </summary>
	/// <returns>プレイヤーカメラ</returns>
	PlayerCamera& GetPlayerCamera()
	{
		return m_playerCamera;
	}

	/// <summary>
	/// タイトルクラスのポインタの取得
	/// </summary>
	/// <returns>タイトルクラスのポインタ</returns>
	Title* GetTitlePtr()
	{
		return m_title;
	}

	/// <summary>
	/// ゲーム全体を管理するクラスのポインタの取得
	/// </summary>
	/// <returns>ゲーム全体を管理するクラスのポインタ</returns>
	Game* GetGamePtr()
	{
		//ゲーム全体を管理するクラスが削除されていたらそのインスタンスをnullptrにする
		if (m_game != nullptr)
		{
			if (m_game->IsDead())
			{
				m_game = nullptr;
			}
		}

		return m_game;
	}

private://メンバ変数
	ModelRender m_playerModel;//プレイヤークラス
	CharacterController m_charaCon;//キャラクターコントローラ
	Vector3 m_position = Vector3::Zero;//プレイヤーの位置
	//Vector3 m_position = Vector3{ -11242.1758,-0.00112915039,-9552.204110 };//プレイヤーの位置(デバッグ用)
	//Vector3 m_position = Vector3{ 0.0,-0.00112915039,-11284.0f };//プレイヤーの位置(デバッグ用)
	Quaternion m_rotation = Quaternion::Identity;//プレイヤーの回転
	Vector3 m_scale = Vector3::One;//プレイヤーの大きさ
	PlayerAnimation* m_playerAnimation = nullptr;//プレイヤーアニメーション用のインスタンス
	PlayerMove* m_playerMove = nullptr;//プレイヤー移動用のインスタンス
	PlayerRotation* m_playerRotation = nullptr;//プレイヤー回転用のインスタンス
	PlayerCatchEnemy* m_playerCatchEnemy = nullptr;//プレイヤーが敵をキャッチする用のインスタンス
	PlayerEffect* m_playerEffect = nullptr;//プレイヤーのエフェクト用のインスタンス
	PlayerCamera m_playerCamera;//プレイヤーカメラクラス
	Title* m_title = nullptr;//タイトルクラスのインスタンス
	Game* m_game = nullptr;//ゲーム全体を管理する用のインスタンス
};

