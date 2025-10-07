#include "stdafx.h"
#include "PlayerCamera.h"

namespace {
	// カメラ。
	const float CAMERA_NEAR(1.0f);//カメラの近平面。

	const float CAMERA_FAR(1000000000.0f);//カメラの遠平面。
}

//初期化
void PlayerCamera::Init()
{
	//注視点から視点までのベクトルを設定。
	m_toCameraPos.Set(0.0f, 10.0f, -300.0f);

	//カメラのニアクリップとファークリップを設定する。
	g_camera3D->SetNear(CAMERA_NEAR);
	g_camera3D->SetFar(CAMERA_FAR);
}

//カメラ追従処理の実行
void PlayerCamera::Execute(Vector3& position)
{
	//注視点を計算する
	Vector3 target = position;
	//プレイヤの足元からちょっと上を注視点とする。
	target.y += 125.0f;

	Vector3 toCameraPosOld = m_toCameraPos;

	//パッドの入力を使ってカメラを回す
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	//Y軸周りの回転
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 1.3f * x);
	qRot.Apply(m_toCameraPos);
	//X軸周りの回転
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 1.3f * y);
	qRot.Apply(m_toCameraPos);

	//カメラの回転の上限を判断する処理
	Vector3 toPosDir = m_toCameraPos;
	//注視点から視点までのベクトルを正規化
	toPosDir.Normalize();
	if (toPosDir.y < -0.2f) {
		//カメラが上向きすぎ
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.9f) {
		//カメラが下向きすぎ
		m_toCameraPos = toCameraPosOld;
	}

	//視点を計算する
	Vector3 pos = target + m_toCameraPos;

	//メインカメラに注視点と視点を設定する
	g_camera3D->SetTarget(target);
	g_camera3D->SetPosition(pos);

	//カメラの更新
	g_camera3D->Update();
}