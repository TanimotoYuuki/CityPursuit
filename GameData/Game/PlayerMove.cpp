#include "stdafx.h"
#include "PlayerMove.h"

//移動処理の実行
void PlayerMove::Execute(Vector3& position)
{
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//プレイヤーが地面に着いているときだけ左スティックの入力量を取得できる
	if (m_charaCon.IsOnGround())
	{
		//左スティックの入力量を取得
		float lStick_x = g_pad[0]->GetLStickXF();
		float lStick_y = g_pad[0]->GetLStickYF();
		//カメラの前方方向と右方向を取得
		Vector3 cameraForward = g_camera3D->GetForward();
		Vector3 cameraRight = g_camera3D->GetRight();
		//XZ平面での前方方向、右方向に変換する
		cameraForward.y = 0.0f;
		cameraForward.Normalize();
		cameraRight.y = 0.0f;
		cameraRight.Normalize();
		//XZ成分の移動速度をクリア
		m_moveSpeed += cameraForward * lStick_y * 325.0f;	//奥方向への移動速度を加算
		m_moveSpeed += cameraRight * lStick_x * 325.0f;		//右方向への移動速度を加算
	}

	//ジャンプ処理の実行
	m_playerJump.Execute(m_charaCon, m_moveSpeed);

	position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	m_charaCon.SetPosition(position);
}