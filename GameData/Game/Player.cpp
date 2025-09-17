#include "stdafx.h"
#include "Player.h"
#include "PlayerMove.h"
#include "PlayerRotation.h"

//デストラクタ
Player::~Player()
{
	DeleteGO(m_playerMove);//プレイヤー移動
	DeleteGO(m_playerRotation);//プレイヤー回転
}

//開始処理
bool Player::Start()
{
	//プレイヤーモデルの初期化
	m_playerModel.Init("Assets/modelData/unitychan.tkm");

	//プレイヤーモデルの初期位置の設定
	m_playerModel.SetPosition(m_position);

	//プレイヤーモデルの大きさの設定
	m_playerModel.SetScale(m_scale * 1.2f);

	//プレイヤーモデルの回転の設定
	m_rotation.SetRotationDegY(0.0f);
	m_playerModel.SetRotation(m_rotation);

	//プレイヤー移動クラスのインスタンスの生成
	m_playerMove = NewGO<PlayerMove>(0, "playermove");
	//プレイヤー移動クラスの初期化
	m_playerMove->Init(m_position);

	//プレイヤーモデルの更新
	m_playerModel.Update();

	//プレイヤー回転クラスのインスタンスの生成
	m_playerRotation = NewGO<PlayerRotation>(0, "playerrotation");

	return true;
}

//更新処理
void Player::Update()
{
	//移動処理の実行
	m_playerMove->Execute(m_position);

	//プレイヤーの位置の更新
	m_playerModel.SetPosition(m_position);

	//プレイヤー回転クラスの初期化
	m_playerRotation->Execute(m_rotation);

	//プレイヤーの回転の更新
	m_playerModel.SetRotation(m_rotation);

	//プレイヤーモデルの更新
	m_playerModel.Update();
}

//描画処理
void Player::Render(RenderContext& rc)
{
	//プレイヤーモデルの描画
	m_playerModel.Draw(rc);
}