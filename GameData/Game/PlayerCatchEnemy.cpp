#include "stdafx.h"
#include "PlayerCatchEnemy.h"
#include "Player.h"
#include "PlayerMove.h"
#include "PlayerSwingAction.h"
#include "PlayerEffect.h"
#include "SwingModel.h"
#include "Enemy.h"
#include "EnemyAI.h"
#include "EnemyEffect.h"
#include "QteEvent.h"
#include "Game.h"
#include "GameTimeLimit.h"
#include "GameMission.h"
#include "MiniMap.h"

namespace{ 
	const float CATCH_ENEMY_LENGTH = 1500.0f;//敵をキャッチできる距離

	const float CATCH_ENEMY_ANGLE_DIFF = 3.14f * 0.25f;//敵をキャッチできる角度差
	
	const float ON_ENEMY_HEIGHT = 150.0f;//敵の上に乗る時の高さ

	const float GO_ON_ENEMY_TIME = 0.8f;//敵の上に行くまでの時間

	const float ON_ENEMY_BACK_LENGTH = 1.0f;//敵の上に乗るときの後ろの長さ

	const float WIREING_TO_ENEMY_JUMP_FORCE = 10.0f;//敵に糸を伸ばすときに、ジャンプの姿勢になるように、ちょっとジャンプさせる時の力

	const float LEAVE_ENEMY_JUMP_FORCE_FRONT = 100.0f;//敵の上から離れるときの、ジャンプの力の、前方向

	const float LEAVE_ENEMY_JUMP_FORCE_UP = 1000.0f;//敵の上から離れるときの、ジャンプの力の、上方向
}

PlayerCatchEnemy::~PlayerCatchEnemy()
{
	DeleteGO(m_qteEvent);
}

bool PlayerCatchEnemy::Start()
{
	m_player = FindGO<Player>("player");

	m_playerSwingAction = FindGO<PlayerSwingAction>("playerswingaction");

	m_qteEvent = NewGO<QteEvent>(0, "qteevent");
	m_qteEvent->SetPlayerPtr(m_player);
	return true;
}

void PlayerCatchEnemy::Execute()
{
	//ゲーム全体を管理するクラスのポインタがnullptrだったら
	//現在のシーンはタイトル画面
	if (m_game == nullptr)
	{
		m_game = m_player->GetGamePtr();
		return;
	}
	//ゲーム全体を管理するクラスのポインタがnullptrではなかったら
	//現在のシーンはインゲーム画面・ゲームオーバー画面・ゲームクリア画面
	else
	{
		m_qteEvent->SetGamePtr(m_game);
	}

	//ゲームスタート演出が終わっていないとき処理をしない
	if (!m_game->IsFinishGameStartDirection())
	{
		return;
	}

	//敵をキャッチする入力していないとき
	if (m_isInputCatchEnemy != true)
	{
		if (m_player->GetCharacterController().IsOnGround())
		{
			if (!m_isQteEvent)
			{
				m_player->GetPlayerMove()->SetCanMove(true);
				m_player->GetPlayerMove()->SetCanJump(true);
				m_player->GetPlayerCamera().SetCanMoveCamera(true);
				m_game->GetGameTimeLimitPtr()->DisableTimeStop();
			}
		}

		FindTarget();//ターゲットを探す処理
		
		if (m_catchEnemy != nullptr)
		{
			if (g_pad[0]->IsTrigger(enButtonY))
			{
				m_playerSwingAction->Reset();
				m_isInputCatchEnemy = true;

				if (m_swingModel == nullptr)
				{
					m_swingModel = FindGO<SwingModel>("swingmodel");
				}
				return;
			}
		}
	}

	//敵をキャッチする入力しているとき
	if (m_isInputCatchEnemy == true)
	{
		if (!m_qteEvent->IsQteEventResult(QteEvent::enQteEventResult_Success) &&
			m_catchEnemy->GetEnemyEffectPtr()->IsPlayEffect() != EnemyEffect::enEnemyEffectList_EngineSmoke_Large)
		{
			// ステートによって処理を振り分ける
			switch (m_catchEnemyState)
			{
			case enStartWireToEnemy:
				StartWireToEnemy(m_catchEnemy);//敵に向かってワイヤーを伸ばし始める処理
				break;
			case enWireingToEnemy:
				WireingToEnemy(m_catchEnemy);//敵に向かって糸を伸ばす処理
				break;
			case enGoOnEnemy:
				GoOnEnemy(m_catchEnemy);//敵の上に行く処理
				break;
			case enOnEnemy:
				OnEnemy(m_catchEnemy);//敵の上にいる処理
				break;
			}
		}
	}
}

//リセット処理
void PlayerCatchEnemy::Reset()
{
	m_catchEnemyState = enStartWireToEnemy;

	m_isInputCatchEnemy = false;
	m_isQteEventMove = false;

	m_player->GetPlayerCamera().SetIsOnEnemyCamera(false);
	m_player->GetPlayerCamera().Reset(m_player);

	if (!m_catchEnemy->IsDead())
	{
		m_catchEnemy->SetOnPlayer(false);
	}
	m_catchEnemy = nullptr;

	FinishQteEvent();

	m_qteEvent->Reset();
	m_qteEvent->FinishResultDirection();

	//制限時間UIを描画する
	m_game->GetGameTimeLimitPtr()->EnableDrawingUI();
	//ゲームミッションUIを描画する
	m_game->GetGameMissionPtr()->EnableDrawingUI();
	//ミニマップを描画する
	m_game->GetMiniMapPtr()->EnableDrawingUI();
}

//QTEイベントで成功した時のプレイヤーの挙動
void PlayerCatchEnemy::QteEventSuccessMove()
{
	//QTEイベントで成功した時のプレイヤーの挙動していたら処理しない
	if (m_isQteEventMove)
	{
		return;
	}

	//移動ベクトルをリセットする
	m_player->GetPlayerMove()->ResetMoveSpeedX();
	m_player->GetPlayerMove()->ResetMoveSpeedY();
	m_player->GetPlayerMove()->ResetMoveSpeedZ();

	//入力情報のリセット
	m_player->GetPlayerMove()->ResetInput();

	//移動状態のリセット
	m_player->GetPlayerMove()->ResetMoveState();

	m_player->GetPlayerMove()->SetUseGravity(true);

	Vector3 leaveJumpForce = Vector3::Front;
	m_player->GetRotation().Apply(leaveJumpForce);
	leaveJumpForce.Scale(LEAVE_ENEMY_JUMP_FORCE_FRONT);
	leaveJumpForce.y = LEAVE_ENEMY_JUMP_FORCE_UP;

	m_player->GetPlayerMove()->AddMoveSpeed(leaveJumpForce);
	m_isQteEventMove = true;
}

//QTEイベントで失敗した時のプレイヤーの挙動
void PlayerCatchEnemy::QteEventFailedMove()
{
	//QTEイベントで失敗した時のプレイヤーの挙動していたら処理しない
	if (m_isQteEventMove)
	{
		return;
	}

	//移動ベクトルをリセットする
	m_player->GetPlayerMove()->ResetMoveSpeedX();
	m_player->GetPlayerMove()->ResetMoveSpeedY();
	m_player->GetPlayerMove()->ResetMoveSpeedZ();

	//入力情報のリセット
	m_player->GetPlayerMove()->ResetInput();

	//移動状態のリセット
	m_player->GetPlayerMove()->ResetMoveState();

	m_player->GetPlayerMove()->SetUseGravity(true);

	Vector3 leaveJumpForce = Vector3::Front;
	m_player->GetRotation().Apply(leaveJumpForce);
	leaveJumpForce.Scale(LEAVE_ENEMY_JUMP_FORCE_FRONT);
	leaveJumpForce.y = LEAVE_ENEMY_JUMP_FORCE_UP;

	m_player->GetPlayerMove()->AddMoveSpeed(leaveJumpForce);
	m_isQteEventMove = true;
}

//ターゲットを探す処理
void PlayerCatchEnemy::FindTarget()
{
	auto& enemys = FindGOs<Enemy>("enemy");
	for (const auto& enemy : enemys)
	{
		//プレイヤーから敵の距離を求める
		Vector3 playerToEnemyDis = enemy->GetEnemyModel().GetDrawPosition() - m_player->GetPosition();
		const float playerToEnemyLen = playerToEnemyDis.Length();

		//距離が一定以上離れているか?
		if (playerToEnemyLen >= CATCH_ENEMY_LENGTH)
		{
			//一定以上離れていたら処理しない
			m_catchEnemy = nullptr;
			continue;
		}

		//カメラから敵の角度差を求める
		Vector3 cameraToEnemyDis = enemy->GetEnemyModel().GetDrawPosition() - g_camera3D->GetPosition();
		Vector3 cameraToEnemyNorm = cameraToEnemyDis;
		cameraToEnemyNorm.Normalize();
		float angleDiff = Dot(cameraToEnemyNorm, g_camera3D->GetForward());
		angleDiff = acos(angleDiff);

		//角度差が一定以上の大きさか?
		if (angleDiff >= CATCH_ENEMY_ANGLE_DIFF)
		{
			//一定以上大きさのため処理しない
			m_catchEnemy = nullptr;
			continue;
		}

		m_catchEnemy = enemy;
		return;
	}
}

//敵に向かってワイヤーを伸ばし始める処理
void PlayerCatchEnemy::StartWireToEnemy(Enemy* enemy)
{
	//プレイヤーが移動できないようにする
	m_player->GetPlayerMove()->SetCanMove(false);
	//プレイヤーがジャンプできないようにする
	m_player->GetPlayerMove()->SetCanJump(false);
	//カメラ移動ができないようにする
	m_player->GetPlayerCamera().SetCanMoveCamera(false);
	//スイングアクション用の重力を使わないようににする
	m_player->GetPlayerMove()->SetUseSwingActionGravity(false);

	//敵の方を向かせる
	LookAtEnemy(enemy);

	//敵をキャッチしているか?
	m_isCatchEnemy = true;

	//エフェクトを消す
	m_player->GetPlayerEffect()->ChangeEffect(PlayerEffect::enPlayerEffectList_None);

	//敵がいれば、ステートを遷移する。
	ChangeState(enemy, enWireingToEnemy);
}

//敵の方を向かせる
void PlayerCatchEnemy::LookAtEnemy(Enemy* enemy)
{
	// プレイヤーから敵への方向ベクトル
	Vector3 playerToEnemyNorm = enemy->GetEnemyModel().GetDrawPosition() - m_player->GetPosition();
	playerToEnemyNorm.y = 0.0f;
	playerToEnemyNorm.Normalize();	// 正規化する
	// 回転
	Quaternion qRot;
	qRot.SetRotation(Vector3::Front, playerToEnemyNorm);
	// プレイヤーに回転を設定する
	m_player->SetRotation(qRot);

	return;
}

//敵に向かって糸を伸ばす処理
void PlayerCatchEnemy::WireingToEnemy(Enemy* enemy)
{
	// ターゲット座標は、敵の座標
	Vector3 targetPos = enemy->GetEnemyModel().GetDrawPosition();
	// 高さをちょっと上げる
	targetPos.y += ON_ENEMY_HEIGHT;

	// 敵に向かって糸を伸ばす
	m_swingModel->SetToStretchPos(targetPos);

	if (m_swingModel->IsWireStretched())
	{
		// 伸ばし切ったら、ステート遷移。
		ChangeState(enemy, enGoOnEnemy);
	}
}

//敵の上に行く処理
void PlayerCatchEnemy::GoOnEnemy(Enemy* enemy)
{
	//ターゲット座標は、敵の座標
	Vector3 targetPos = enemy->GetEnemyModel().GetDrawPosition();
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
	LookAtEnemy(enemy);

	//補間率を進める
	m_goOnEnemyTimer += g_gameTime->GetFrameDeltaTime();

	if (m_goOnEnemyTimer >= GO_ON_ENEMY_TIME)
	{
		// 敵の上まで着いたら、ステートを遷移する。
		ChangeState(enemy, enOnEnemy);
	}
}

//敵の上にいる処理
void PlayerCatchEnemy::OnEnemy(Enemy* enemy)
{
	//乗っている敵と同じ回転にする
	Quaternion qRot = enemy->GetRotation();

	//ターゲット座標は、敵の座標
	Vector3 targetPos = enemy->GetEnemyModel().GetDrawPosition();
	//高さをちょっと上げる
	targetPos.y += ON_ENEMY_HEIGHT;

	// ちょっと後ろに乗る
	Vector3 targetBackVec = Vector3::Back;
	qRot.Apply(targetBackVec);
	targetBackVec.Normalize();
	targetBackVec.Scale(ON_ENEMY_BACK_LENGTH);
	targetPos += targetBackVec;

	m_player->SetDirectPosition(targetPos);
	m_player->SetRotation(qRot);
}

//ステートを変更する
void PlayerCatchEnemy::ChangeState(Enemy* enemy, const EnCatchEnemyState newState)
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
		m_swingModel->StartWireStretchToPos(enemy->GetEnemyModel().GetDrawPosition());
		m_player->GetPlayerMove()->SetUseGravity(false);
		//ジャンプの姿勢になるように、ちょっとジャンプさせる
		m_player->GetPlayerMove()->AddMoveSpeed(Vector3::Up * WIREING_TO_ENEMY_JUMP_FORCE);

		//制限時間を動かさない
		m_game->GetGameTimeLimitPtr()->EnableTimeStop();
		break;

	case enGoOnEnemy:
		m_startGoOnEnemyPos = m_player->GetPosition();
		break;

	case enOnEnemy:
		//補完率をリセットして、糸の伸ばしを終える。
		m_goOnEnemyTimer = 0.0f;
		m_swingModel->EndWireStretchToPos();
		m_qteEvent->SetTargetEnemy(enemy);
		enemy->SetOnPlayer(true);
		m_isCatchEnemy = false;
		StartQteEvent();
		m_game->GetGameTimeLimitPtr()->DisableDrawingUI();
		m_game->GetGameMissionPtr()->DisableDrawingUI();
		m_game->GetMiniMapPtr()->DisableDrawingUI();
		break;
	}

	return;
}
