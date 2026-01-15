#include "stdafx.h"
#include "PlayerCamera.h"
#include "Player.h"
#include "PlayerMove.h"
#include "PlayerCatchEnemy.h"
#include "Enemy.h"
#include "DebugLog.h"

namespace {
	//カメラ。
	const float TARGET_OFFSET_UP = 120.0f;//注視点のオフセット

	const float ON_ENEMY_CAMERA_VERTICAL_ANGLE = -45.0f;

	const float NEAR_TO_CAMERA_DISTANCE = 500.0f;

	const Vector3 DEFAULT_TO_CAMERA_POS{0.0f, 10.0f, -300.0f};//デフォルトの注視点から視点までのベクトル
}

//初期化
void PlayerCamera::Init()
{
	m_targetOffsetUp = TARGET_OFFSET_UP;

	//注視点から視点までのベクトルを設定。
	m_toCameraPos.Set(DEFAULT_TO_CAMERA_POS);

	//バネカメラの初期化
	m_springCamera.Init(
		*g_camera3D,//バネカメラに使うカメラ
		100000.0f,//カメラの移動速度の最大値
		true,//カメラと地形との当たり判定を取るかどうかのフラグ
		1.0f//カメラに設定される球体コリジョンの半径
	);

	m_springCamera.SetDampingRate(0.85f);//減衰率の設定
}

//カメラ追従処理の実行
void PlayerCamera::Execute(Player* playerData, const Vector3& position)
{
	if (m_isOnEnemyCamera)
	{
		OnEnemyCamera(playerData, position);//敵の上に乗っている用のカメラ
	}

	//注視点を計算する
	Vector3 target = position;
	//プレイヤの足元からちょっと上を注視点とする。
	target.x += m_targetOffsetXZ * g_camera3D->GetForward().x;
	target.y += m_targetOffsetUp;
	target.z += m_targetOffsetXZ * g_camera3D->GetForward().z;
	target += g_camera3D->GetForward() * 30.0f;

	Vector3 toCameraPosOld = m_toCameraPos;

	if (m_canMoveCamera)
	{
		//パッドの入力を使ってカメラを回す
		m_inputRStick.x = g_pad[0]->GetRStickXF();
		m_inputRStick.y = g_pad[0]->GetRStickYF();
	}

	//Y軸周りの回転
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 1.3f * m_inputRStick.x);
	qRot.Apply(m_toCameraPos);
	//X軸周りの回転
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 1.3f * m_inputRStick.y);
	qRot.Apply(m_toCameraPos);

	//カメラの回転の上限を判断する処理
	Vector3 toPosDir = m_toCameraPos;
	//注視点から視点までのベクトルを正規化
	toPosDir.Normalize();
	if (toPosDir.y < -0.5f) {
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
	m_springCamera.SetTarget(target);
	m_springCamera.SetPosition(pos);

	//カメラの更新
	m_springCamera.Update();
}

//敵の上に乗っている用のカメラ
void PlayerCamera::OnEnemyCamera(Player* playerData, const Vector3& position)
{
	const Quaternion& playerRot = playerData->GetRotation();
	m_toCameraPos = Vector3::Back;
	playerRot.Apply(m_toCameraPos);

	// 垂直方向のカメラの回転を計算する
	// 水平な軸
	Vector3 axisH;
	// 外積で、Y軸と、注視点から視点へのベクトルに、直交するベクトルを求める
	axisH.Cross(Vector3::AxisY, m_toCameraPos);
	// 正規化する
	axisH.Normalize();
	// 水平な軸周りで回転させる
	Quaternion cameraVRot;
	cameraVRot.SetRotationDeg(
		axisH,
		ON_ENEMY_CAMERA_VERTICAL_ANGLE
	);
	// 注視点から視点へのベクトルを回転させる
	cameraVRot.Apply(m_toCameraPos);

	// 向きを正規化してから
	m_toCameraPos.Normalize();
	// 伸ばす
	m_toCameraPos.Scale(NEAR_TO_CAMERA_DISTANCE);
}

//プレイヤーの向きを基準にカメラをリセット
void PlayerCamera::ResetToPlayerView(Player* playerData)
{
	//プレイヤーの位置を設定
	const Vector3& playerPosition = playerData->GetPosition();

	//プレイヤーの回転を設定
	const Quaternion& playerRotation = playerData->GetRotation();

	//カメラの支点をプレイヤーの位置に設定
	Vector3 target = playerPosition;

	m_toCameraPos.Set(DEFAULT_TO_CAMERA_POS);
	playerRotation.Apply(m_toCameraPos);
}