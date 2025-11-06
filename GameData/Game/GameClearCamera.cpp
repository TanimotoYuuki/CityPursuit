#include "stdafx.h"
#include "GameClearCamera.h"
#include "GameClear.h"
#include "Game.h"
#include "Player.h"
#include "PlayerMove.h"
#include "GameClearSprite.h"

namespace {
	const float CAMERA_PLAYER_ROTATION_ANGLE = 180.0f;//カメラがプレイヤーに向かって回転する角度

	const float DISTANCE_OFFSET = 50.0f;//距離のオフセット値

	const float Z_OFFSET = 75.0f;//Z軸のオフセット

	const float CAMERA_MOVE_SPEED = 600.0f;//カメラの移動速度

	const float CAMERA_MOVE_AMOUNT_LIMIT = 150.0f;//カメラの移動量の上限
}

bool GameClearCamera::Start()
{
	//プレイヤークラスのポインタの取得
	m_player = GetGameClearPtr()->GetGamePtr()->GetPlayerPtr();

	//バネカメラの初期化
	m_gameClearCamera.Init(
		*g_camera3D,//バネカメラに使うカメラ
		100000.0f,//カメラの移動速度の最大値
		false,//カメラと地形との当たり判定を取るかどうかのフラグ
		1.0f//カメラに設定される球体コリジョンの半径
	);

	//カメラの注視点を設定
	m_cameraTarget = m_player->GetPlayerCamera().GetSpringCamera().GetTarget();
	m_gameClearCamera.SetTarget(m_cameraTarget);

	//カメラの位置を設定
	m_cameraPosition = m_player->GetPlayerCamera().GetSpringCamera().GetPosition();
	m_gameClearCamera.SetPosition(m_cameraPosition);

	//カメラの更新処理
	m_gameClearCamera.Update();

	//カメラからプレイヤーまでの距離を求める
	m_playerPosition = m_player->GetModelData().GetPosition();
	m_cameraToPlayer = m_cameraTarget - m_cameraPosition;

	//ゲームクリア時にカメラがプレイヤーに向かって回転する角度を求める
	//1.プレイヤーの進行方向を取得
	Vector3 playerForward = m_player->GetPlayerMove()->GetMoveDirection();
	playerForward.y = 0.0f;//Y軸要素除外
	playerForward.Normalize();//方向ベクトル化

	//2.カメラの注視点からカメラの位置の距離を求める
	Vector3 cameraToPlayer = m_cameraPosition - m_cameraTarget;
	cameraToPlayer.y = 0.0f;//Y軸要素除外
	cameraToPlayer.Normalize();//方向ベクトル化

	//3.プレイヤーの進行方向とカメラからプレイヤーまでのベクトルの内積を求める
	float dot = Dot(playerForward, cameraToPlayer);

	//4.求めた内積の値でゲームクリア時にカメラを回転する角度を求める
	m_rotationAngleDeg = Math::RadToDeg(acosf(dot));
	////プレイヤーの進行方向が左側なら
	if (m_player->GetPlayerMove()->GetMoveDirection().x > 0.0f)
	{
		m_rotationAngleDeg *= -1;//-180度～180度の範囲に収める
	}

	//回転角度の設定
	m_currentAngleDeg = 0.0f;

	return true;
}

void GameClearCamera::Execute()
{
	//カメラの状態
	switch (m_cameraState)
	{
	case GameClearCamera::enCameraState_Rotation://カメラの回転
		CameraRotation();
		break;
	case GameClearCamera::enCameraState_Move://カメラの移動
		CameraMove();
		break;
	default:
		break;
	}
}

//カメラの回転処理
void GameClearCamera::CameraRotation()
{
	//角度の更新
	m_currentAngleDeg += 600.0f * g_gameTime->GetFrameDeltaTime();

	//一定の回転角度を超えたら回転終了
	if (m_currentAngleDeg > m_rotationAngleDeg)
	{
		m_currentAngleDeg = m_rotationAngleDeg;
	}

	//角度をラジアンに変換
	float currentRad = Math::DegToRad(m_currentAngleDeg);
	float cosRad = cos(currentRad);
	float sinRad = sin(currentRad);

	//回転をした位置を求める
	Vector3 currentPosition = Vector3::Zero;
	currentPosition.x = m_playerPosition.x + (m_cameraToPlayer.x * cosRad - m_cameraToPlayer.z * sinRad);
	currentPosition.y = m_cameraPosition.y;
	currentPosition.z = m_playerPosition.z + (m_cameraToPlayer.x * sinRad - m_cameraToPlayer.z * cosRad);
	currentPosition.z -= Z_OFFSET;

	//カメラの位置を設定
	m_gameClearCamera.SetPosition(currentPosition);

	//カメラの更新処理
	m_gameClearCamera.Update();

	if (m_gameClearCamera.GetRealPosition().z == m_gameClearCamera.GetPosition().z)
	{
		m_isFinishRotationCamera = true;//カメラの回転終了

		if (m_gameClear->GetGameClearSprite()->IsFinishAlphaSpriteAnimation(GameClearSprite::enSpriteAlphaState_Opaque))
		{
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				m_afterRotationCameraPosition = currentPosition;
				m_cameraState = enCameraState_Move;
			}
		}
	}
}

//カメラの移動処理
void GameClearCamera::CameraMove()
{
	Vector3 currentTarget = m_gameClearCamera.GetTarget();
	Vector3 currentPosition = m_gameClearCamera.GetPosition();

	currentTarget.x -= CAMERA_MOVE_SPEED * g_gameTime->GetFrameDeltaTime();
	currentPosition.x -= CAMERA_MOVE_SPEED * g_gameTime->GetFrameDeltaTime();

	float moveAmount = fabsf(m_afterRotationCameraPosition.x - currentPosition.x);
	if (moveAmount >= CAMERA_MOVE_AMOUNT_LIMIT)
	{
		m_isFinishMoveCamera = true;
		return;
	}

	//カメラの注視点を設定
	m_gameClearCamera.SetTarget(currentTarget);

	//カメラの位置を設定
	m_gameClearCamera.SetPosition(currentPosition);

	//カメラの更新処理
	m_gameClearCamera.Update();
}