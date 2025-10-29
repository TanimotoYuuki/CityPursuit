#include "stdafx.h"
#include "PlayerMove.h"
#include "PlayerAnimation.h"
#include "PlayerSwingAction.h"
#include "DebugLog.h"

PlayerMove::~PlayerMove()
{
	DeleteGO(m_playerSwingAction);
}

//開始処理
bool PlayerMove::Start()
{
	//プレイヤースイングアクションクラスのインスタンスの生成
	m_playerSwingAction = NewGO<PlayerSwingAction>(0, "playerswingaction");
	//m_debugLog = FindGO<DebugLog>("debuglog");
	//m_debugLog->SetDebugName("MoveSpeed");
	//m_debugLog->SetDebugName("Position");
	return true;
}

//移動処理の実行
void PlayerMove::Execute(Vector3& position, CharacterController& characterController)
{	
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//ジャンプ処理
	//プレイヤーが地面に着いているときだけ左スティックの入力量を取得できる
	if (characterController.IsOnGround())
	{
		//ジャンプ処理の実行
		m_playerJump.Execute(m_moveSpeed, characterController);
	}
	else
	{
		if (m_useGravity)
		{
			if (m_ussSwingActionGravity)//スイングアクション用の重力
			{
				m_moveSpeed.y -= 19.6f / 1.5f;
			}
			else//通常の重力
			{
				m_moveSpeed.y -= 19.6f;
			}
		}
	}

	//移動処理
	//地面に着いているとき
	if(characterController.IsOnGround())
	{
		//左スティックの入力量を取得
		m_inputLStickMemory.x = g_pad[0]->GetLStickXF();
		m_inputLStickMemory.y = g_pad[0]->GetLStickYF();
		m_inputLStick = m_inputLStickMemory;
	}
	//地面に着いていないとき
	else
	{
		//左スティックの入力量を取得
		m_inputLStickMemory.x = g_pad[0]->GetLStickXF();
		m_inputLStickMemory.y = g_pad[0]->GetLStickYF();

		if (m_inputLStickMemory.x != 0.0f ||
			m_inputLStickMemory.y != 0.0f)
		{
			m_inputLStick = m_inputLStickMemory;
		}
	}

	//カメラの前方方向と右方向を取得
	m_cameraForward = g_camera3D->GetForward();
	m_cameraRight = g_camera3D->GetRight();
	//XZ平面での前方方向、右方向に変換する
	m_cameraForward.y = 0.0f;
	m_cameraForward.Normalize();
	m_cameraRight.y = 0.0f;
	m_cameraRight.Normalize();

	if(m_canMove)
	{
		//ダッシュ
		if (g_pad[0]->IsPress(enButtonRB2))
		{
			//地面に着いているときのみダッシュできる
			if (characterController.IsOnGround())
			{
				//XZ成分の移動速度をクリア
				m_moveSpeed += m_cameraForward * m_inputLStick.y * 700.0f;	//奥方向への移動速度を加算
				m_moveSpeed += m_cameraRight * m_inputLStick.x * 700.0f;	//右方向への移動速度を加算

				if (m_moveSpeed.Length() != 0.0f)
				{
					m_isRun = true;//走っている
					m_isWalk = false;//歩いていない
				}
				else
				{
					m_isRun = false;//走っていない
					m_isWalk = false;//歩いていない
				}
			}
		}
		//歩き
		else
		{
			//XZ成分の移動速度をクリア
			m_moveSpeed += m_cameraForward * m_inputLStick.y * 325.0f;	//奥方向への移動速度を加算
			m_moveSpeed += m_cameraRight * m_inputLStick.x * 325.0f;	//右方向への移動速度を加算

			if (m_moveSpeed.Length() != 0.0f)
			{
				m_isWalk = true;//歩いている
				m_isRun = false;//走っていない
			}
			else
			{
				m_isWalk = false;//歩いていない
				m_isRun = false;//走っていない
			}
		}
	}

	//スイングアクションの更新処理
	m_playerSwingAction->Execute();
	
	//m_debugLog->SetDebugLogData("MoveSpeed", m_moveSpeed);

	//プレイヤーのが移動しているときに処理する
	if (m_moveSpeed.x != 0.0f ||
		m_moveSpeed.z != 0.0f)
	{
		//プレイヤーの進行方向を計算
		m_moveDirection = m_moveSpeed;
		m_moveDirection.y = 0.0f;
		m_moveDirection.Normalize();
	}

	position = characterController.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	characterController.SetPosition(position);

	//m_debugLog->SetDebugLogData("Position", position);
}

//移動速度の制限
void PlayerMove::LimitMoveSpeed(const float limitSpeed)
{
	if (m_moveSpeed.Length() <= limitSpeed)
	{
		return;
	}

	m_moveSpeed.Normalize();
	m_moveSpeed.Scale(limitSpeed);
}