#include "stdafx.h"
#include "FirstEnemyDirection.h"
#include "Player.h"
#include "Enemy.h"

namespace {
	//敵テキストUI
	const float ENEMY_TEXT_UI_WIDTH = 1024.0f;//敵テキストUIの横幅

	const float ENEMY_TEXT_UI_HEIGHT = 128.0f;//敵テキストUIの縦幅

	const Vector3 ENEMY_TEXT_UI_POSITION{ 0.0f,325.0f,0.0f };//敵テキストUIの位置

	const Vector3 ENEMY_TEXT_UI_SCALE{ 0.6f,0.6f,0.6f };//敵テキストUIの大きさ

	//矢印UI
	const float ARROW_UI_WIDTH_AND_HEIGHT = 1024;//矢印UIの横幅と縦幅

	const Vector3 ARROW_UI_POSITION{ 0.0f,215.0f,0.0f };//矢印UIの位置

	const Vector3 ARROW_UI_SCALE{0.1f,0.1f, 0.1f};//矢印UIの大きさ

	const float ARROW_UI_AMPLITUDE = 2.0f;//矢印UIの振幅

	const float ARROW_UI_MOVE_SPEED = 10.0f;//矢印UIの動作速度

	//カメラ演出
	const float CAMERA_START_MOVE_SPEED = 0.5f;//カメラ移動速度(演出開始時)

	const float CAMERA_END_MOVE_SPEED = 1.35f;//カメラ移動速度(演出終了時)
}

//開始処理
bool FirstEnemyDirection::Start()
{
	//敵テキストUIの初期化
	InitEnemyTextUI();

	//矢印UIの初期化
	InitArrowUI();

	//バネカメラの初期化
	InitSpringCamera();

	return true;
}

//更新処理
void FirstEnemyDirection::Update()
{
	//敵を初めて発見したときの演出の状態
	switch (m_firstEnemyDirectionState)
	{
	case enFirstEnemyDirectionState_CameraStart://カメラ(演出開始時)
		CameraStartDirectionUpdate(m_enemy);
		break;
	case enFirstEnemyDirectionState_DrawUI://UI描画
		DrawUIDrectionUpdate();
		break;
	case enFirstEnemyDirectionState_CameraEnd://カメラ(演出終了時)
		CameraEndDirectionUpdate(m_enemy);
		break;
	default:
		break;
	}
}

//描画処理
void FirstEnemyDirection::Render(RenderContext& rc)
{
	//敵を初めて発見したときの演出の状態がUI描画以外は描画しない
	if (m_firstEnemyDirectionState != enFirstEnemyDirectionState_DrawUI)
	{
		return;
	}

	//敵テキストUIの描画
	m_enemyTextUI.Draw(rc);

	//矢印UIの描画
	m_arrowUI.Draw(rc);
}

//敵テキストUIの初期化
void FirstEnemyDirection::InitEnemyTextUI()
{
	//敵テキストUIの初期化
	m_enemyTextUI.Init("Assets/sprite/text/enemy.dds", ENEMY_TEXT_UI_WIDTH, ENEMY_TEXT_UI_HEIGHT);
	//敵テキストUIの位置の設定
	m_enemyTextUI.SetPosition(ENEMY_TEXT_UI_POSITION);
	//敵テキストUIの大きさの設定
	m_enemyTextUI.SetScale(ENEMY_TEXT_UI_SCALE);
	//敵テキストUIの更新処理
	m_enemyTextUI.Update();
}


//矢印UIの初期化
void FirstEnemyDirection::InitArrowUI()
{
	//矢印UIの初期化
	m_arrowUI.Init("Assets/sprite/arrow/arrow.dds", ARROW_UI_WIDTH_AND_HEIGHT, ARROW_UI_WIDTH_AND_HEIGHT);
	//矢印UIの位置の設定
	m_arrowUI.SetPosition(ARROW_UI_POSITION);
	//矢印UIの大きさの設定
	m_arrowUI.SetScale(ARROW_UI_SCALE);
	//矢印UIの更新処理
	m_arrowUI.Update();
}

//バネカメラの初期化
void FirstEnemyDirection::InitSpringCamera()
{
	m_directionCamera.Init(
		*g_camera3D,//バネカメラに使うカメラ
		100000.0f,//カメラの移動速度の最大値
		false,//カメラと地形との当たり判定を取るかどうかのフラグ
		1.0f//カメラに設定される球体コリジョンの半径
	);

	//カメラの注視点を設定
	m_cameraTarget = m_player->GetPlayerCamera().GetSpringCamera().GetTarget();
	m_directionCamera.SetTarget(m_cameraTarget);

	//カメラの位置を設定
	m_cameraPosition = m_player->GetPlayerCamera().GetSpringCamera().GetPosition();
	m_directionCamera.SetPosition(m_cameraPosition);

	//カメラの更新処理
	m_directionCamera.Update();
}

//カメラ演出の更新処理(演出開始時)
void FirstEnemyDirection::CameraStartDirectionUpdate(Enemy* enemyData)
{
	m_cameraLarpRate += CAMERA_START_MOVE_SPEED * g_gameTime->GetFrameDeltaTime();//カメラの補間率

	if (m_cameraLarpRate > 1.0f)
	{
		m_firstEnemyDirectionState = enFirstEnemyDirectionState_DrawUI;
		return;
	}

	m_larpCameraTarget.Lerp(
		m_cameraLarpRate,//補間率
		m_directionCamera.GetTarget(),//プレイヤーのカメラの注視点
		enemyData->GetEnemyModel().GetDrawPosition() + Vector3(0.0f, 100.0f, 0.0f)//敵の位置+オフセット
	);

	m_larpCameraPosition.Lerp(
		m_cameraLarpRate,//補間率
		m_directionCamera.GetPosition(),//プレイヤーのカメラの位置
		enemyData->GetEnemyModel().GetDrawPosition() + Vector3(0.0f, 300.0f, 450.0f)//敵の位置+オフセット
	);

	m_directionCamera.SetTarget(m_larpCameraTarget);
	m_directionCamera.SetPosition(m_larpCameraPosition);
	m_directionCamera.Update();
}

//UI描画の更新処理
void FirstEnemyDirection::DrawUIDrectionUpdate()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_firstEnemyDirectionState = enFirstEnemyDirectionState_CameraEnd;
		m_cameraLarpRate = 0.0f;
		return;
	}

	//矢印UIを上下に浮遊させる挙動
	Vector3 arrowUIPos = m_arrowUI.GetPosition();
	m_arrowUIMoveTime += g_gameTime->GetFrameDeltaTime();
	float yOffset = sin(ARROW_UI_MOVE_SPEED * m_arrowUIMoveTime) * ARROW_UI_AMPLITUDE;
	arrowUIPos.y += yOffset;

	m_arrowUI.SetPosition(arrowUIPos);
	m_arrowUI.Update();
}

//カメラ演出の更新処理(演出終了時)
void FirstEnemyDirection::CameraEndDirectionUpdate(Enemy* enemyData)
{
	m_cameraLarpRate += CAMERA_END_MOVE_SPEED * g_gameTime->GetFrameDeltaTime();//カメラの補間率

	if (m_cameraLarpRate > 1.0f)
	{
		DeleteGO(this);
		return;
	}

	m_larpCameraTarget.Lerp(
		m_cameraLarpRate,//補間率
		enemyData->GetEnemyModel().GetDrawPosition() + Vector3(0.0f, 100.0f, 0.0f),//敵の位置+オフセット
		m_player->GetPlayerCamera().GetSpringCamera().GetTarget()//プレイヤーのカメラの注視点
	);

	m_larpCameraPosition.Lerp(
		m_cameraLarpRate,//補間率
		enemyData->GetEnemyModel().GetDrawPosition() + Vector3(0.0f, 300.0f, 450.0f),//敵の位置+オフセット
		m_player->GetPlayerCamera().GetSpringCamera().GetPosition()//プレイヤーのカメラの位置
	);

	m_directionCamera.SetTarget(m_larpCameraTarget);
	m_directionCamera.SetPosition(m_larpCameraPosition);
	m_directionCamera.Update();
}
