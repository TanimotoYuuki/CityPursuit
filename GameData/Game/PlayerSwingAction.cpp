#include "stdafx.h"
#include "PlayerSwingAction.h"
#include "SwingActionManager.h"
#include "SwingModel.h"
#include "SwingTarget.h"
#include "Player.h"
#include "PlayerMove.h"
#include "PlayerCatchEnemy.h"
#include "PlayerEffect.h"

namespace{ 
//スイング
	const float MIN_VELOCITY_OF_AFTER_SWING_ACCELERATION = 50.0f;//スイング後の加速の最低速度

	const float INIT_VELOCITY_OF_ATER_SWING_ACCELERATION = 1200.0f;//スイング後の加速の初速度

	const float SWING_ROLL_UP_POWER = 500.0f;//スイングロールの上昇力

//歩き
	const float WALK_ACCELERATION = 50.0f;//歩き時の加速度

	const float WALK_MAX_SPEED = 325.0f;//歩き時の最高速度

//スピード
	const float START_DECELERATE_SWING_SPEED_INIT_VALUE = -700.0f;//減速し始めるスピードの初期値

	const float CAN_START_SWING_FALL_SPEED = -10.0f;//スイングを開始できる落下速度

	const float INIT_SWING_SPEED = 900.0f;//最初のスイングスピード
}

//デストラクタ
PlayerSwingAction::~PlayerSwingAction()
{
	DeleteGO(m_swingActionManager);
	DeleteGO(m_swingModel);
}

//開始処理
bool PlayerSwingAction::Start()
{
	//スイングアクションマネージャークラスのインスタンスの生成
	m_swingActionManager = NewGO<SwingActionManager>(0, "swingactionmanager");

	//スイングモデルクラスのインスタンスの生成
	m_swingModel = NewGO<SwingModel>(0,"swingmodel");

	//スイングターゲットのインスタンスの検索
	m_swingTarget = FindGO<SwingTarget>("swingtarget");

	//プレイヤークラスのインスタンスの検索
	m_player = FindGO<Player>("player");
	return true;
}

//スイングアクションの実行処理
void PlayerSwingAction::Execute()
{
	//スイングアクションの前に行う処理
	PreSwingAction();

	//プレイヤーモデルが地面に付いていないときに処理する
	if (!m_player->GetCharacterController().IsOnGround())
	{
		//敵をキャッチする入力していたら処理しない
		if (m_player->GetPlayerCatchEnemy()->IsInputCatchEnemy())
		{
			return;
		}

		//RB2ボタンが長押ししているときの処理
		if (g_pad[0]->IsPress(enButtonRB2))
		{
			m_isInputSwingAction = true;

			//スイングアクションの更新処理
			SwingActionUpdate();
		}
		else
		{
			//地面に付いていない時にスイングの状態がスイング後の空中状態だったら
			if (m_swingState == enSwingState_AirAfterSwing)
			{
				AirAfterSwing();//スイング後の空中状態
			}

			//スイングアクションの入力していない
			m_isInputSwingAction = false;
		}
	}
	else
	{
		//地面に付いている時にスイングの状態がスイング終了状態だったら
		if (m_swingState == enSwingState_SwingEnd)
		{
			SwingEnd();//スイング終了処理
		}

		//スイングアクション用の重力を使わない
		m_player->GetPlayerMove()->SetUseSwingActionGravity(false);

		//エフェクトを消す
		m_player->GetPlayerEffect()->ChangeEffect(PlayerEffect::enPlayerEffectList_None);

		//スイングアクションの入力していない
		m_isInputSwingAction = false;
	}
}

//スイングアクションの前に行う処理
void PlayerSwingAction::PreSwingAction()
{
	//スイングターゲットを探すか？
	if (m_swingState == enSwingState_FindSwingTarget)
	{
		//スイングターゲットを探す
		FindSwingTarget();

		//早期リターン
		return;
	}

	//地面に着いているか
	if (m_player->GetCharacterController().IsOnGround() == true)
	{
		//スイングターゲットを探さないか？
		if(m_swingState != enSwingState_FindSwingTarget)
		{
			//スイング状態がスイング終了状態ではないとき
			if (m_swingState != enSwingState_SwingEnd)
			{
				//スイングアクションを終了する
				ChangeState(enSwingState_SwingEnd);
			}

			//早期リターン
			return;
		}
	}

	//ジャンプ入力があるか？、かつ
	//スイング中か？
	if (g_pad[0]->IsTrigger(enButtonA) &&
		m_swingState == enSwingState_Swinging)
	{
		m_swingRollFlag = true;

		GameSoundEngine::GetInstance()->PlaySE(GameSoundList_SE_AfterSwing, 0.5f);

		//ステートをスイング後の空中状態に遷移する
		ChangeState(enSwingState_AirAfterSwing);

		//早期リターン
		return;
	}

	//スイングアクションの入力していないかつ
	//スイング後の空中状態ではないか？
	//スイングターゲットを探している状態ではないか？
	if (m_isInputSwingAction != true &&
		m_swingState != enSwingState_AirAfterSwing &&
		m_swingState != enSwingState_FindSwingTarget)
	{
		//ステートをスイング後の空中状態に遷移する
		ChangeState(enSwingState_AirAfterSwing);

		//早期リターン
		return;
	}
}

//スイングアクションの更新処理
void PlayerSwingAction::SwingActionUpdate()
{
	////スイング状態がスイングターゲット検索状態のとき
	//if (m_swingState == enSwingState_FindSwingTarget)
	//{
	//	ChangeState(enSwingState_WireStretching);//ワイヤーを伸ばしている状態に変更
	//}

	//スイングの状態によって処理を変える
	switch (m_swingState)
	{
	case PlayerSwingAction::enSwingState_FindSwingTarget://スイングターゲット検索状態
		if (m_swingActionManager->IsSwingTargetScopeRadiusFound())//スイングターゲットが見つかったか？
		{
			ChangeState(enSwingState_WireStretching);//ワイヤーを伸ばしている状態に変更
		}
		break;
	case PlayerSwingAction::enSwingState_WireStretching://ワイヤーを伸ばしている状態
		WireStretching();//ワイヤーを伸ばしている処理
		break;
	case PlayerSwingAction::enSwingState_Swinging://スイングしている状態
		Swinging();//スイングしている処理
		break;
	case PlayerSwingAction::enSwingState_AirAfterSwing://スイング後の空中状態
		AirAfterSwing();//スイング後の空中処理
		break;
	case PlayerSwingAction::enSwingState_SwingEnd://スイング終了状態
		SwingEnd();//スイング終了処理
		break;
	default:
		break;
	}
}

//ワイヤーを伸ばしている処理
void PlayerSwingAction::WireStretching()
{
	//ワイヤーが伸びきっていなかったら処理を抜ける
	if (m_swingModel->IsWireStretched() != true)
	{
		return;
	}

	//スイングの状態をスイングしている状態にする
	ChangeState(enSwingState_Swinging);
}

//スイングしている処理
void PlayerSwingAction::Swinging()
{
	//スイングによるプレイヤーの移動
	SwingPlayerMove();
}

//スイング後の空中処理
void PlayerSwingAction::AirAfterSwing()
{
	//カメラのイージング処理
	CameraEasing();


	//落下速度がマイナスになっていたらスイングターゲットを探す状態に戻る
	if (m_player->GetPlayerMove()->GetMoveSpeed().y < CAN_START_SWING_FALL_SPEED)
	{
		//エフェクトを消す
		m_player->GetPlayerEffect()->ChangeEffect(PlayerEffect::enPlayerEffectList_None);
		//スイングアクションの入力があるとき
		if (m_isInputSwingAction == true)
		{
			ChangeState(enSwingState_FindSwingTarget);
			return;
		}
	}

	//スイング後の加速の減衰
	m_accelerationAfterSwing *= 0.995f;
	if (m_accelerationAfterSwing < MIN_VELOCITY_OF_AFTER_SWING_ACCELERATION)
	{
		m_accelerationAfterSwing = MIN_VELOCITY_OF_AFTER_SWING_ACCELERATION;
	}

	//スイング後の速度どスイング後の加速を合わせて速度とする
	float velocity = m_velocityAfterSwing + m_accelerationAfterSwing;

	//加算移動ベクトル
	Vector3 addMoveVec = m_player->GetPlayerMove()->GetMoveSpeed();

	//スイングの直後か？
	if (m_afterSwing)
	{
		// スイングの直後ではなくする
		m_afterSwing = false;
		// スイングロールをするか？
		if (m_swingRollFlag)
		{
			// スイングロールを行った
			m_swingRollFlag = false;
			// Y成分にスイングロールの上昇力を加える
			addMoveVec.y += SWING_ROLL_UP_POWER;
		}
	}
	else
	{
		//最初以外はY成分は加えない
		addMoveVec.y = 0.0f;
	}

	//速度で伸ばす
	addMoveVec.Normalize();
	addMoveVec.Scale(velocity);

	//移動入力処理
	if (g_pad[0]->GetLStickXF() != 0.0f || g_pad[0]->GetLStickYF() != 0.0f)
	{
		Vector3 forwardDirXZ = g_camera3D->GetForward();
		forwardDirXZ.y = 0.0f;
		forwardDirXZ.Normalize();
		Vector3 rightDirXZ = g_camera3D->GetRight();
		rightDirXZ.y = 0.0f;
		rightDirXZ.Normalize();
		Vector3 inputDirXZ = forwardDirXZ * g_pad[0]->GetLStickYF();
		inputDirXZ += rightDirXZ * g_pad[0]->GetLStickXF();
		float radAngle = acosf(Dot(inputDirXZ, m_swingForwardDir));
		if (radAngle >= 3.14f * 0.7f)
		{
			if (radAngle >= 3.14f * 0.7f)
			{
				m_velocityAfterSwing -= WALK_ACCELERATION * 0.3f;
				if (m_velocityAfterSwing <= WALK_MAX_SPEED)
				{
					ChangeState(enSwingState_SwingEnd);
				}
			}
		}
		else
		{
			inputDirXZ *= WALK_MAX_SPEED / 2.5f;
			m_inputMoveDirXZ.Lerp(0.2f, m_inputMoveDirXZ, inputDirXZ);
		}

		addMoveVec += m_inputMoveDirXZ;
	}

	m_player->GetPlayerMove()->ResetMoveSpeedX();
	m_player->GetPlayerMove()->ResetMoveSpeedZ();

	m_player->GetPlayerMove()->AddMoveSpeed(addMoveVec);
}

//スイング終了処理
void PlayerSwingAction::SwingEnd()
{
	//スイングの状態をスイングターゲットを検索状態にする
	ChangeState(enSwingState_FindSwingTarget);
	//ワイヤーに終了を知らせる
	m_swingModel->EndWireStretchToPos();
	//スイングスピードをリセット
	m_swingSpeed = 0.0f;
	//カメラの値をリセット
	m_player->GetPlayerCamera().LerpDampingRate(0.0f);
	m_player->GetPlayerCamera().LerpTargetOffsetUp(0.0f);
	m_player->GetPlayerCamera().LerpTargetOffsetForward(0.0f);
}

//スイングの状態の変更
void PlayerSwingAction::ChangeState(const EnSwingState enSwingState)
{
	if (m_swingState == enSwingState)
	{
		//同じなら変更する必要なし。早期リターン。
		return;
	}

	//スイングアクションステートを変更
	m_swingState = enSwingState;

	//////// 変更した時一度だけ呼ばれる処理 ////////

	//各種、変更した時一度だけ呼ばれるイベント
	switch (m_swingState)
	{
	case enSwingState_FindSwingTarget://スイングターゲット検索状態
		//スイングアクションしてしていない
		m_isSwingAction = false;
		break;
	case enSwingState_WireStretching://ワイヤーを伸ばしている状態
		IsWireStretchingEvent();
		break;
	case enSwingState_Swinging://スイングしている状態
		IsSwingingEvent();
		break;
	case enSwingState_AirAfterSwing://スイング後の空中状態
		IsAirAfterSwingEvent();
		break;
	case enSwingState_SwingEnd://スイング終了状態
		//スイングアクションしてしていない
		m_isSwingAction = false;
		break;
	}

	return;
}

//スイングターゲットを探す
void PlayerSwingAction::FindSwingTarget()
{
	//1.糸を出す方向を決める
	
	//糸を出す方向への回転クォータニオン
	Quaternion swingRotationQRot = Quaternion::Identity;

	//カメラの前方向に糸を出す。
	Vector3 cameraDir = g_camera3D->GetForward();
	cameraDir.y = 0.0f;
	cameraDir.Normalize();
	swingRotationQRot.SetRotation(Vector3::Front, cameraDir);

	//回転から糸を出す方向を求める
	m_swingForwardDir = Vector3::Front;
	swingRotationQRot.Apply(m_swingForwardDir);
	m_swingForwardDir.Normalize();

	//2.スイングターゲットを探す
	
	//スイングアクションマネージャーの実行処理
	m_swingActionManager->Execute();
}

//スイングによるプレイヤーの移動
void PlayerSwingAction::SwingPlayerMove()
{
	//カメラのイージング処理
	CameraEasing();

	//1.必要なベクトルを用意
	//プレイヤーからスイングターゲットまでのXZ平面でのベクトル
	Vector3 playerToTargetVecXZ = m_swingActionManager->GetSwingActionShortestDistance() - m_player->GetPosition();
	//Y成分を消去
	playerToTargetVecXZ.y = 0.0f;

	//XZ平面の前方向に、プレイヤーからスイングターゲットへのXZ平面のベクトルを射影する
	float projectToTargetVecXZToSwingForwardDir = Dot(m_swingForwardDir, playerToTargetVecXZ);

	//XZ平面での、前方向のみの、スイングターゲットへのベクトル
	Vector3 toTargetFowardVecXZ = m_swingForwardDir;
	//射影して求めた長さまで伸ばす
	toTargetFowardVecXZ.Scale(projectToTargetVecXZToSwingForwardDir);

	//XZ平面での、前方向のみの、スイングターゲットの座標
	const Vector3 toTargetForwardPosXZ = m_player->GetPosition() + toTargetFowardVecXZ;

	//前方向と上方向のみの、スイングターゲットの座標
	Vector3 toTargetForwardUpPos = toTargetForwardPosXZ;
	//Y座標を実際のスイングターゲットの座標と同じにする
	toTargetForwardUpPos.y = m_swingActionManager->GetSwingActionShortestDistance().y;

	//前方向と上方向のみのスイングターゲットの座標からプレイヤーへのベクトル
	const Vector3 toTargetForwardUpToPlayerVec = m_player->GetPosition() - toTargetForwardUpPos;

	//前方向と上方向のみのスイングターゲットの座標からプレイヤーへの方向
	Vector3 targetUptoPlayerDir = toTargetForwardUpToPlayerVec;
	//正規化する
	targetUptoPlayerDir.Normalize();

	//XZ平面での前方向と、スイングターゲットからプレイヤーへの方向の内積
	float doSwingForwardDirAndToPlayerDir =
		Dot(m_swingForwardDir, targetUptoPlayerDir);

	//加算移動方向ベクトル
	Vector3 addMoveDir = Vector3::Zero;

	//加算移動方向ベクトルを回転させる回転クォータニオン
	Quaternion qRotForAddMoveDir;

	//加算移動方向ベクトルを回転させる回転軸。
	Vector3 rotAxisForAddMoveDir = Cross(targetUptoPlayerDir, m_swingForwardDir);

	//プレイヤーがスイングターゲットより上にいるか？
	if (m_player->GetPosition().y >= m_swingActionManager->GetSwingActionShortestDistance().y)
	{
		//上にいるとき、回転軸の求め方を変える
		rotAxisForAddMoveDir = Cross(Vector3::Down, m_swingForwardDir);
	}

	//求めた回転軸を正規化する
	rotAxisForAddMoveDir.Normalize();

	//2.スイングの角度を計算

	//内積が負か？、つまり、スイングターゲットより手前側にいるか？
	if (doSwingForwardDirAndToPlayerDir < 0.0f)
	{
		//手前側

		//プレイヤーが最低スイング高度より上にいるか？
		if (m_player->GetPosition().y > 250.0f)
		{
			//上にいる

			//0.0f～1.0fに変化するレート
			float rate = doSwingForwardDirAndToPlayerDir + 1.0f;

			//pattern1
			//指数関数的な変化にする
			//rate = pow(rate, 5.0f);
			//1.0f～0.0fに変化するようにする
			//rate = 1.0f - rate;
			//float radAngle = 3.14f * 0.1f * -rate;

			//pattern2
			//二次関数的な変化にする
			rate = pow(rate - 0.5f, 3.0f);
			//上に凸にする
			rate *= -1.0f;
			//rateが最低値と最高値で0.0fになるような切片にする
			rate += pow(0.5f, 3.0f);
			//90度にレートをかける
			float radAngle = 3.14f * 0.5f * -rate;

			//進む方向をまっすぐ前にする
			addMoveDir = m_swingForwardDir;
			//回転クォータニオンを90度回転させる
			qRotForAddMoveDir.SetRotation(rotAxisForAddMoveDir, radAngle/*3.14f * 0.5f*/);
			//加算移動方向ベクトルを回転させる
			qRotForAddMoveDir.Apply(addMoveDir);
		}
		else
		{
			//最低高度にいるため、まっすぐ進む。
			addMoveDir = m_swingForwardDir;
		}

	}
	else
	{
		//奥側

		float radAngle = 3.14f * 0.5f;

		//プレイヤーの高さがスイングターゲットより高くなったら。
		if (m_player->GetPosition().y >= m_swingActionManager->GetSwingActionShortestDistance().y)
		{
			//初回ループだけ減速し始めるスピードを設定
			if (m_startDecelerateSwingSpeed <= -50.0f)
			{
				m_startDecelerateSwingSpeed = m_swingSpeed;
			}
			float rad = acosf(doSwingForwardDirAndToPlayerDir);
			float angleRate = rad / (3.14f * 0.1f);
			addMoveDir = m_swingForwardDir;
			// radAngle = 3.14f * 0.5f * min(1.0f, angleRate);
			radAngle = Math::Lerp<float>(min(1.0f, angleRate), 3.14f * 0.4f, 3.14f * 0.5f);
		}

		addMoveDir = targetUptoPlayerDir;
		//回転クォータニオンを90度回転させる
		qRotForAddMoveDir.SetRotation(rotAxisForAddMoveDir, radAngle);
		//加算移動方向ベクトルを回転させる
		qRotForAddMoveDir.Apply(addMoveDir);

	}

	//////// 3.スイングスピードを計算 ////////

	//任意の位置での振り子の速度
	//g = 重力加速度、l = 振り子の長さ、v任 = 任意の場所での振り子の速度、
	//cosΘ任 = 任意の場所の角度、cosΘ上 = 一番上の時の角度
	//v任 = √2gl(cosΘ任 - cosΘ上)

	//cosΘ任 = 任意の場所の角度
	float anyCos = Dot(targetUptoPlayerDir, Vector3::Down);
	//cosΘ上 = 一番上の時の角度（一番上の時の角度は90度）
	float highestCos = 0.0f;
	//g = 重力加速度
	m_g += 980.0f * g_gameTime->GetFrameDeltaTime() * 0.45f;
	const float maxG = 3500.0f;
	if (m_g > maxG)
	{
		m_g = maxG;
	}
	//l = 振り子の長さ
	const float l = toTargetForwardUpToPlayerVec.Length();
	//v任 = √2gl(cosΘ任 - cosΘ上)
	if (doSwingForwardDirAndToPlayerDir < 0.0f)
	{
		//手前側なら角度による減速しない
		m_swingSpeed = 2.0f * m_g * l;
	}
	else
	{
		//奥側なら、上に上がるほど減速する
		m_swingSpeed = 2.0f * m_g * l * (anyCos - highestCos);
		if (anyCos <= 0.01f)
		{
			//一定以上、上に行くとスイング状態を強制解除
			//ステートをスイング後の空中状態に遷移する
			ChangeState(enSwingState_AirAfterSwing);
		}
	}
	m_swingSpeed = std::sqrtf(m_swingSpeed);


	//////// 4.スイング中の左右への方向転換 ////////
	Vector3 rightDirXZ = g_camera3D->GetRight();
	rightDirXZ.y = 0.0f;
	rightDirXZ.Normalize();
	if (fabsf(g_pad[0]->GetLStickXF()) >= 0.45f)
	{
		float rightPower = g_pad[0]->GetLStickXF() / 4.5f;
		rightDirXZ.Scale(rightPower);
		m_inputMoveDirXZ.Lerp(0.2f, m_inputMoveDirXZ, rightDirXZ);
	}
	addMoveDir += m_inputMoveDirXZ;

	//正規化して方向ベクトルにする
	addMoveDir.Normalize();

	//加算移動ベクトル
	Vector3 addMoveVec = addMoveDir;
	addMoveVec.Scale(m_swingSpeed);

	//現在の移動方向と、スイングの加算移動方向が逆向きなら
	//現在の移動ベクトルをリセットする。
	Vector3 moveDir = m_player->GetPlayerMove()->GetMoveSpeed();
	moveDir.Normalize();
	if (Dot(addMoveDir, moveDir) <= 0.0f)
	{
		m_player->GetPlayerMove()->ResetMoveSpeedX();
		m_player->GetPlayerMove()->ResetMoveSpeedZ();
	}

	//移動ベクトルに、加算移動ベクトルを加算する
	if (!isnan(addMoveVec.x) && !isnan(addMoveVec.y) && !isnan(addMoveVec.z) && addMoveVec.Length() > 0.0001f)
	{
		m_player->GetPlayerMove()->AddMoveSpeed(addMoveVec);
	}

	float safeSwingSpeed = (isnan(m_swingSpeed) || m_swingSpeed < 0.0001f) ? 1.0f : m_swingSpeed;
	m_player->GetPlayerMove()->LimitMoveSpeed(safeSwingSpeed);

	//スイングの前方向を更新
	m_swingForwardDir = m_player->GetPlayerMove()->GetMoveSpeed();
	// Y成分を消去
	m_swingForwardDir.y = 0.0f;
	m_swingForwardDir.Normalize();

	if (!GameSoundEngine::GetInstance()->IsPlayingSound(GameSoundList_SE_Swing))
	{
		GameSoundEngine::GetInstance()->PlaySE(GameSoundList_SE_Swing,0.5f);
	}
}

//カメラのイージング処理
void PlayerSwingAction::CameraEasing()
{
	if (m_swingState == enSwingState_Swinging)//スイング中のカメラのイージング処理
	{
		if (m_cameraEasingTime > 1.0f)
		{
			return;
		}

		m_cameraEasingTime += 1.75f * g_gameTime->GetFrameDeltaTime();
		m_player->GetPlayerCamera().LerpDampingRate(m_cameraEasingTime * m_cameraEasingTime);
		m_player->GetPlayerCamera().LerpTargetOffsetUp(m_cameraEasingTime * m_cameraEasingTime);
		m_player->GetPlayerCamera().LerpTargetOffsetForward(m_cameraEasingTime * m_cameraEasingTime);
	}
	else if (m_swingState == enSwingState_AirAfterSwing)//スイング後の空中状態のカメラのイージング処理
	{
		if (m_cameraEasingTime < 0.0f)
		{
			m_player->GetPlayerMove()->SetUseSwingActionGravity(false);//スイングアクション用の重力を使わない
			return;
		}

		m_cameraEasingTime -= 3.0f * g_gameTime->GetFrameDeltaTime();
		m_player->GetPlayerCamera().LerpDampingRate((m_cameraEasingTime * m_cameraEasingTime) / 2.0f);
		m_player->GetPlayerCamera().LerpTargetOffsetUp(m_cameraEasingTime * m_cameraEasingTime);
		m_player->GetPlayerCamera().LerpTargetOffsetForward(m_cameraEasingTime * m_cameraEasingTime);
	}
}

//ワイヤーが伸びるイベント処理
void PlayerSwingAction::IsWireStretchingEvent()
{
	//ワイヤーをスイングターゲットに向かって伸ばし始める
	m_swingModel->StartWireStretchToPos(m_swingActionManager->GetSwingActionShortestDistance());

	// カメラの値を線形変化させるタイマーをリセットする
	m_cameraEasingTime = 0.0f;

	//スイングアクションしている
	m_isSwingAction = true;
}

//スイングしているイベント処理
void PlayerSwingAction::IsSwingingEvent()
{
	//減速し始めるスイングスピードを初期化する
	m_startDecelerateSwingSpeed = START_DECELERATE_SWING_SPEED_INIT_VALUE;
	//入力によって生じたXZ平面での移動方向を初期化する
	m_inputMoveDirXZ = Vector3::Zero;

	m_afterSwing = true;

	//スイングアクションしている
	m_isSwingAction = true;

	//スイングのエフェクトを出す
	m_player->GetPlayerEffect()->ChangeEffect(PlayerEffect::enPlayerEffectList_Swing);

	//スイングスピードが初期速度より遅いか？
	if (m_swingSpeed <= INIT_SWING_SPEED)
	{
		//遅い。初期速度のスイングスピードを設定する。落下速度が速いとスイングスピードも速くなる。
		m_g = Math::Lerp<float>(
			min(1.0f, fabsf(m_player->GetPlayerMove()->GetMoveSpeed().y) / 5000.0f),
			700.0f,
			1800.0f
		);

		//早期リターン
		return;
	}
	m_g = m_player->GetPlayerMove()->GetXZSpeed();
	//スイングスピードが最大速度を超えているか？
	const float maxG = 3500.0f;
	if (m_g > maxG)
	{
		// 超えている。最大速度に設定する。
		m_g = maxG;
	}
}

//スイング後の空中処理
void PlayerSwingAction::IsAirAfterSwingEvent()
{
	//スイングアクションしてしていない
	m_isSwingAction = false;

	//糸に終了を知らせる
	m_swingModel->EndWireStretchToPos();

	//スイング中にスイングをやめたか？
	if (m_afterSwing)
	{
		//スイング後の加速の初速をリセット
		m_accelerationAfterSwing = INIT_VELOCITY_OF_ATER_SWING_ACCELERATION;
		//スイング後の速度を保持
		m_velocityAfterSwing = m_player->GetPlayerMove()->GetXZSpeed();
		//入力によって生じたXZ平面での移動方向をリセット
		m_inputMoveDirXZ = Vector3::Zero;
		
		m_player->GetPlayerMove()->SetUseSwingActionGravity(true);//スイングアクション用の重力を使う

		m_swingForwardDir = m_player->GetPlayerMove()->GetMoveSpeed();
		m_swingForwardDir.y = 0.0f;
		m_swingForwardDir.Normalize();
	}
	else
	{
		//スイング中ではない
		m_accelerationAfterSwing = 0.0f;
		m_velocityAfterSwing = 0.0f;
		ChangeState(enSwingState_SwingEnd);
	}
}