#include "stdafx.h"
#include "Player.h"
#include "PlayerAnimation.h"
#include "PlayerMove.h"
#include "PlayerRotation.h"
#include "PlayerCatchEnemy.h"
#include "PlayerEffect.h"
#include "Game.h"

//デストラクタ
Player::~Player()
{
	DeleteGO(m_playerAnimation);//プレイヤーアニメーション
	DeleteGO(m_playerMove);//プレイヤー移動
	DeleteGO(m_playerRotation);//プレイヤー回転
	DeleteGO(m_playerCatchEnemy);//プレイヤーが敵をキャッチする
	DeleteGO(m_playerEffect);//プレイヤーのエフェクト
}

//開始処理
bool Player::Start()
{
	//プレイヤーアニメーションクラスのインスタンスの生成
	m_playerAnimation = NewGO<PlayerAnimation>(0,"playeranimation");

	//アニメーションの初期化
	m_playerAnimation->Init();

	//プレイヤーモデルの初期化
	m_playerModel.Init("Assets/modelData/player/player.tkm", m_playerAnimation->GetAnimationClips(), PlayerAnimation::enAnimationList_Num, enModelUpAxisZ, true);

	//プレイヤーモデルの初期位置の設定
	m_playerModel.SetPosition(m_position);

	//プレイヤーモデルの大きさの設定
	m_playerModel.SetScale(m_scale);

	//プレイヤーモデルの回転の設定
	m_rotation.SetRotationDegY(0.0f);
	m_playerModel.SetRotation(m_rotation);

	//プレイヤー移動クラスのインスタンスの生成
	m_playerMove = NewGO<PlayerMove>(0, "playermove");

	//プレイヤーモデルの更新
	m_playerModel.Update();

	//キャラクターコントローラの設定
	m_charaCon.Init(25.0f, 85.0f, m_position);

	//プレイヤー回転クラスのインスタンスの生成
	m_playerRotation = NewGO<PlayerRotation>(0, "playerrotation");

	//プレイヤーが敵をキャッチするクラスのインスタンス生成
	m_playerCatchEnemy = NewGO<PlayerCatchEnemy>(0, "playercatchenemy");

	//プレイヤーのエフェクトクラスのインスタンスの生成
	m_playerEffect = NewGO<PlayerEffect>(0, "playereffect");

	//プレイヤーカメラクラスの初期化
	m_playerCamera.Init();

	return true;
}

//更新処理
void Player::Update()
{
	if (GetGamePtr() != nullptr)
	{
		if (!GetGamePtr()->IsGameEnd())
		{
			//移動処理の実行
			m_playerMove->Execute(m_position, m_charaCon);

			//プレイヤー回転クラスの実行
			m_playerRotation->Execute(m_rotation);

			//プレイヤーが敵をキャッチする処理の実行
			m_playerCatchEnemy->Execute();

			//カメラ追従処理の実行
			m_playerCamera.Execute(this, m_position);

			Vector3 effectPos = m_playerCamera.GetSpringCamera().GetRealPosition();
			effectPos += m_playerCamera.GetSpringCamera().GetCamera()->GetForward() * 300.0f;

			Quaternion cameraRot = m_playerCamera.GetCameraRotation();

			//プレイヤーのエフェクトの実行
			m_playerEffect->Execute(effectPos, cameraRot);
		}
	}

	//アニメーションの実行
	m_playerAnimation->Execute(m_playerModel, this);

	//プレイヤーの位置の更新
	m_playerModel.SetPosition(m_position);

	//プレイヤーの回転の更新
	m_playerModel.SetRotation(m_rotation);

	//プレイヤーの大きさの設定
	m_playerModel.SetScale(m_scale);

	//プレイヤーモデルの更新
	m_playerModel.Update();
}

//直接座標を設定
void Player::SetDirectPosition(const Vector3& pos)
{
	m_charaCon.SetPosition(pos);
	m_playerModel.SetPosition(pos);
}

//描画処理
void Player::Render(RenderContext& rc)
{
	//プレイヤーモデルの描画
	m_playerModel.Draw(rc);
}