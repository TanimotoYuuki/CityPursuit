#include "stdafx.h"
#include "PlayerCatchEnemy.h"
#include "Player.h"
#include "PlayerMove.h"
#include "PlayerSwingAction.h"
#include "SwingModel.h"
#include "Enemy.h"
#include "QteEvent.h"

namespace{ 
	const float CATCH_ENEMY_LENGTH = 1500.0f;//敵をキャッチできる距離
	
	const float ON_ENEMY_HEIGHT = 150.0f;//敵の上に乗る時の高さ

	const float GO_ON_ENEMY_TIME = 0.8f;//敵の上に行くまでの時間

	const float ON_ENEMY_BACK_LENGTH = 70.0f;//敵の上に乗るときの後ろの長さ

	const float WIREING_TO_ENEMY_JUMP_FORCE = 10.0f;//敵に糸を伸ばすときに、ジャンプの姿勢になるように、ちょっとジャンプさせる時の力

	const float LEAVE_ENEMY_JUMP_FORCE_FRONT = 100.0f;//敵の上から離れるときの、ジャンプの力の、前方向

	const float LEAVE_ENEMY_JUMP_FORCE_UP = 1500.0f;//敵の上から離れるときの、ジャンプの力の、上方向
}

bool PlayerCatchEnemy::Start()
{
	m_player = FindGO<Player>("player");
	m_playerSwingAction = FindGO<PlayerSwingAction>("playerswingaction");
	m_enemy = FindGO<Enemy>("enemy");
	return true;
}

void PlayerCatchEnemy::Execute()
{
	if (m_enemy == nullptr)
	{
		return;
	}

	FindTarget();//ターゲットを探す処理

	//敵をキャッチしない場合
	if (m_isCatchEnemy != true)
	{
		if(m_player->GetCharacterController().IsOnGround())
		{
			m_player->GetPlayerMove()->SetCanMove(true);
		}

		if (m_distance.Length() < CATCH_ENEMY_LENGTH)
		{
			if (g_pad[0]->IsTrigger(enButtonY))
			{
				m_playerSwingAction->Reset();
				m_isCatchEnemy = true;

				if (m_swingModel == nullptr)
				{
					m_swingModel = FindGO<SwingModel>("swingmodel");
				}
			}
		}
		return;
	}

	// ステートによって処理を振り分ける
	switch (m_catchEnemyState)
	{
	case enStartWireToEnemy:
		StartWireToEnemy();//敵に向かってワイヤーを伸ばし始める処理
		break;
	case enWireingToEnemy:
		WireingToEnemy();//敵に向かって糸を伸ばす処理
		break;
	case enGoOnEnemy:
		GoOnEnemy();//敵の上に行く処理
		break;
	case enOnEnemy:
		OnEnemy();//敵の上にいる処理
		break;
	}
}

//リセット処理
void PlayerCatchEnemy::Reset()
{
	m_catchEnemyState = enStartWireToEnemy;
	m_player->GetPlayerCamera().SetIsOnEnemyCamera(false);
	m_player->GetPlayerMove()->SetUseGravity(true);
	m_player->GetPlayerCamera().SetCanMoveCamera(true);

	Vector3 leaveJumpForce = Vector3::Front;
	m_player->GetModelData().GetRotation().Apply(leaveJumpForce);
	leaveJumpForce.Scale(LEAVE_ENEMY_JUMP_FORCE_FRONT);
	leaveJumpForce.y = LEAVE_ENEMY_JUMP_FORCE_UP;

	m_player->GetPlayerMove()->AddMoveSpeed(leaveJumpForce);
	m_isCatchEnemy = false;
}

//ターゲットを探す処理
void PlayerCatchEnemy::FindTarget()
{
	//プレイヤーから敵の距離を求める
	m_distance = m_enemy->GetPosition() - m_player->GetModelData().GetPosition();
}

//敵に向かってワイヤーを伸ばし始める処理
void PlayerCatchEnemy::StartWireToEnemy()
{
	//プレイヤーが移動できないようにする
	m_player->GetPlayerMove()->SetCanMove(false);
	//カメラ移動ができないようにする
	m_player->GetPlayerCamera().SetCanMoveCamera(false);

	// 敵の方を向かせる
	LookAtEnemy();

	//敵がいれば、ステートを遷移する。
	ChangeState(enWireingToEnemy);
}

//敵の方を向かせる
void PlayerCatchEnemy::LookAtEnemy()
{
	// プレイヤーから敵への方向ベクトル
	Vector3 playerToEnemyNorm = m_enemy->GetPosition() - m_player->GetModelData().GetPosition();
	playerToEnemyNorm.y = 0.0f;
	playerToEnemyNorm.Normalize();	// 正規化する
	// 回転
	Quaternion qRot;
	qRot.SetRotation(Vector3::Front, playerToEnemyNorm);
	// プレイヤーに回転を設定する
	m_player->GetModelData().SetRotation(qRot);

	return;
}

//敵に向かって糸を伸ばす処理
void PlayerCatchEnemy::WireingToEnemy()
{
	// ターゲット座標は、敵の座標
	Vector3 targetPos = m_enemy->GetPosition();
	// 高さをちょっと上げる
	targetPos.y += ON_ENEMY_HEIGHT;

	// 敵に向かって糸を伸ばす
	m_swingModel->SetToStretchPos(targetPos);

	if (m_swingModel->IsWireStretched())
	{
		// 伸ばし切ったら、ステート遷移。
		ChangeState(enGoOnEnemy);
	}
}

//敵の上に行く処理
void PlayerCatchEnemy::GoOnEnemy()
{
	//ターゲット座標は、敵の座標
	Vector3 targetPos = m_enemy->GetPosition();
	//高さをちょっと上げる
	targetPos.y += ON_ENEMY_HEIGHT;

	//糸の伸ばす先を更新する。
	m_swingModel->SetToStretchPos(targetPos);

	//移動ベクトルをリセットする
	m_player->GetPlayerMove()->ResetMoveSpeedX();
	m_player->GetPlayerMove()->ResetMoveSpeedY();
	m_player->GetPlayerMove()->ResetMoveSpeedZ();

	const float t = m_goOnEnemyTimer / GO_ON_ENEMY_TIME;
	//移動先の座標
	Vector3 pos = Vector3::Zero;
	//最初の座標から、ターゲット座標まで、線形補完で徐々に近づく。
	pos.Lerp(t, m_startGoOnEnemyPos, targetPos);
	//プレイヤーの座標を直接設定する
	m_player->SetDirectPosition(pos);

	// 敵の方を向ける
	LookAtEnemy();

	//補間率を進める
	m_goOnEnemyTimer += g_gameTime->GetFrameDeltaTime();

	if (m_goOnEnemyTimer >= GO_ON_ENEMY_TIME)
	{
		// 敵の上まで着いたら、ステートを遷移する。
		ChangeState(enOnEnemy);
	}
}

//敵の上にいる処理
void PlayerCatchEnemy::OnEnemy()
{
	//乗っている敵と同じ回転にする
	Quaternion qRot = m_enemy->GetRotation();

	//ターゲット座標は、敵の座標
	Vector3 targetPos = m_enemy->GetPosition();
	//高さをちょっと上げる
	targetPos.y += ON_ENEMY_HEIGHT;

	// ちょっと後ろに乗る
	Vector3 targetBackVec = Vector3::Back;
	qRot.Apply(targetBackVec);
	targetBackVec.Normalize();
	targetBackVec.Scale(ON_ENEMY_BACK_LENGTH);
	targetPos += targetBackVec;

	m_player->SetDirectPosition(targetPos);
	m_player->GetModelData().SetRotation(qRot);
}

//ステートを変更する
void PlayerCatchEnemy::ChangeState(const EnCatchEnemyState newState)
{
	if (m_catchEnemyState == newState)
	{
		// 現在のステートと同じなら、何しない。早期リターン。
		return;
	}

	// ステートを変更
	m_catchEnemyState = newState;

	// 変更した瞬間に、一度だけ呼ばれる処理。
	switch (m_catchEnemyState)
	{
	case enStartWireToEnemy:
		break;

	case enWireingToEnemy:
		//糸を伸ばし始めて、重力を切って、敵の上に乗っているカメラにする。
		m_player->GetPlayerCamera().SetIsOnEnemyCamera(true);
		m_swingModel->StartWireStretchToPos(m_enemy->GetPosition());
		m_player->GetPlayerMove()->SetUseGravity(false);
		//ジャンプの姿勢になるように、ちょっとジャンプさせる
		m_player->GetPlayerMove()->AddMoveSpeed(Vector3::Up * WIREING_TO_ENEMY_JUMP_FORCE);
		break;

	case enGoOnEnemy:
		m_startGoOnEnemyPos = m_player->GetModelData().GetPosition();
		break;

	case enOnEnemy:
		//補完率をリセットして、糸の伸ばしを終える。
		m_goOnEnemyTimer = 0.0f;
		m_swingModel->EndWireStretchToPos();
		m_qteEvent = NewGO<QteEvent>(0, "qteevent");
		m_qteEvent->SetTargetEnemy(m_enemy);
		m_qteEvent->SetPlayerCatchEnemyPtr(this);
		break;
	}

	return;
}
