#include "stdafx.h"
#include "PlayerAnimation.h"
#include "Player.h"
#include "PlayerMove.h"
#include "PlayerJump.h"
#include "PlayerSwingAction.h"
#include "PlayerCatchEnemy.h"
#include "Game.h"
#include "GameClear.h"
#include "GameClearCamera.h"

namespace{
	const float INIT_PLAY_ANIMATION_SPEED = 1.0f;//初期のアニメーション再生速度
}

//初期化処理
void PlayerAnimation::Init()
{
	for (int i = 0; i < enAnimationList_Num; i++)
	{
		//アニメーションのファイルパスが書かれていなかったら処理しない
		if (m_animationFilePath[i] == "")
		{
			continue;
		}

		m_animationClips[i].Load(m_animationFilePath[i].c_str());
		if (i != enAnimationList_Jumping)//ジャンプ中アニメーション以外はループ再生する
		{
			m_animationClips[i].SetLoopFlag(true);
		}
		else//ジャンプ上昇中アニメーションはループ再生しない
		{
			m_animationClips[i].SetLoopFlag(false);
		}
	}
}

//アニメーションの実行処理
void PlayerAnimation::Execute(ModelRender& modelData, Player* playerData)
{
	//アニメーションの切り替え
	ChangeAnimation(modelData, playerData);

	modelData.SetAnimationSpeed(INIT_PLAY_ANIMATION_SPEED);
	//今再生しているアニメーション
	switch (m_nowPlayAnimation)
	{
	case PlayerAnimation::enAnimationList_Idle://待機アニメーション
		modelData.PlayAnimation(enAnimationList_Idle, m_animationInterpolateTime);
		break;
	case PlayerAnimation::enAnimationList_Walk://歩きアニメーション
		modelData.PlayAnimation(enAnimationList_Walk, m_animationInterpolateTime);
		break;
	case PlayerAnimation::enAnimationList_Run://ダッシュアニメーション
		modelData.PlayAnimation(enAnimationList_Run, m_animationInterpolateTime);
		break;
	case PlayerAnimation::enAnimationList_Jumping://ジャンプ中アニメーション
		modelData.SetAnimationSpeed(m_animationSpeed);
		modelData.PlayAnimation(enAnimationList_Jumping, m_animationInterpolateTime);
		break;
	case PlayerAnimation::enAnimationList_Falling://落下中アニメーション
		modelData.PlayAnimation(enAnimationList_Falling, m_animationInterpolateTime);
		break;
	case PlayerAnimation::enAnimationList_SwingLeft://スイング(左側)アニメーション
		modelData.PlayAnimation(enAnimationList_SwingLeft, m_animationInterpolateTime);
		break;
	case PlayerAnimation::enAnimationList_SwingRight://スイング(右側)アニメーション
		modelData.PlayAnimation(enAnimationList_SwingRight, m_animationInterpolateTime);
		break;
	case PlayerAnimation::enAnimationList_QteEvent://QTEイベント用アニメーション
		modelData.PlayAnimation(enAnimationList_QteEvent, m_animationInterpolateTime);
		break;
	case PlayerAnimation::enAnimationList_GameClear://ゲームクリア用アニメション
		modelData.PlayAnimation(enAnimationList_GameClear, m_animationInterpolateTime);
		break;
	default:
		break;
	}
}

//アニメーションの切り替え
void PlayerAnimation::ChangeAnimation(ModelRender& modelData, Player* playerData)
{
	//待機アニメーション
	m_nowPlayAnimation = enAnimationList_Idle;
	m_animationInterpolateTime = 0.1f;

	if (playerData->GetGamePtr()->IsGameEnd())
	{
		//ゲームクリアポインタの中身がnullptrだったら処理しない
		if (m_gameClear == nullptr && m_gameOver == nullptr)
		{
			return;
		}

		if (m_gameOver != nullptr)
		{

		}

		if (m_gameClear != nullptr)
		{
			//ゲームクリアしたときの演出が終了したか?
			if (m_gameClear->GetGameClearCamera()->IsFinishRotationCamera())
			{
				//ゲームクリア用のアニメション
				m_nowPlayAnimation = enAnimationList_GameClear;
			}
		}
		return;
	}

	if (playerData->GetPlayerCatchEnemy()->IsQteEvent())
	{
		//QTEイベント用のアニメーション
		m_nowPlayAnimation = enAnimationList_QteEvent;
		return;
	}

	if (playerData->GetPlayerCatchEnemy()->IsCatchingEnemy())
	{
		//敵をキャッチする直前にジャンプしていたらジャンプ終了処理を行う
		if (playerData->GetPlayerMove()->GetPlayerJump().IsJump())
		{
			playerData->GetPlayerMove()->GetPlayerJump().JumpEnd();//ジャンプ終了処理
		}

		//スイングアニメーション 
		m_nowPlayAnimation = enAnimationList_SwingLeft;
		return;
	}

	if (playerData->GetPlayerMove()->GetPlayerSwingAction()->IsSwingAction())
	{
		//スイングする直前にジャンプしていたらジャンプ終了処理を行う
		if (playerData->GetPlayerMove()->GetPlayerJump().IsJump())
		{
			playerData->GetPlayerMove()->GetPlayerJump().JumpEnd();//ジャンプ終了処理
		}

		//スイングアニメーション 
		m_nowPlayAnimation = enAnimationList_SwingLeft;
		return;
	}

	if (!playerData->GetCharacterController().IsOnGround())
	{
		if (playerData->GetPlayerMove()->GetPlayerJump().IsJump())//ジャンプしているか?
		{
			//ジャンプ中アニメーション 
			m_nowPlayAnimation = enAnimationList_Jumping;
			m_animationSpeed = 3.0f;
			m_animationInterpolateTime = 0.2f;

			//ジャンプ中アニメーションが終了したかどうかの判定
			if (!modelData.IsPlayingAnimation())
			{
				playerData->GetPlayerMove()->GetPlayerJump().JumpEnd();//ジャンプ終了処理
			}
		}
		else
		{
			//ジャンプ中アニメーション 
			m_nowPlayAnimation = enAnimationList_Falling;
		}
		return;
	}

	if (playerData->GetPlayerMove()->IsRun())
	{
		//ダッシュアニメーション
		m_nowPlayAnimation = enAnimationList_Run;
		m_animationInterpolateTime = 0.2f;
		return;
	}

	if (playerData->GetPlayerMove()->IsWalk())
	{
		//歩きアニメーション
		m_nowPlayAnimation = enAnimationList_Walk;
		m_animationInterpolateTime = 0.2f;
		return;
	}
}
