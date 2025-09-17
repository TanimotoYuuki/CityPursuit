#include "stdafx.h"
#include "PlayerRotation.h"
#include "PlayerMove.h"

//開始処理
bool PlayerRotation::Start()
{
	//プレイヤー移動のクラスのインスタンスの検索
	m_playerMove = FindGO<PlayerMove>("playermove");

	return true;
}

//回転処理の実行
void PlayerRotation::Execute(Quaternion& rotaion)
{
	//xかzの移動速度があったら
	if (fabsf(m_playerMove->GetMoveSpeed().x) >= 0.001f || fabsf(m_playerMove->GetMoveSpeed().z) >= 0.001f)
	{
		//プレイヤーモデルの方向を変える
		rotaion.SetRotationYFromDirectionXZ(m_playerMove->GetMoveSpeed());
	}
}