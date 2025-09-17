#include "stdafx.h"
#include "Player.h"

//開始処理
bool Player::Start()
{
	//プレイヤーモデルの初期化
	m_playerModel.Init("Assets/modelData/unitychan.tkm");

	//プレイヤーモデルの初期位置の設定
	m_playerModel.SetPosition(m_position);

	//プレイヤーモデルの大きさの設定
	m_playerModel.SetScale(m_scale * 2.0f);

	//プレイヤーモデルの回転の設定
	m_rotation.SetRotationDegY(0.0f);
	m_playerModel.SetRotation(m_rotation);

	//プレイヤーモデルの更新
	m_playerModel.Update();
	return true;
}

//更新処理
void Player::Update()
{
	//プレイヤーモデルの更新
	m_playerModel.Update();
}

//描画処理
void Player::Render(RenderContext& rc)
{
	//プレイヤーモデルの描画
	m_playerModel.Draw(rc);
}