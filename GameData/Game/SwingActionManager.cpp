#include "stdafx.h"
#include "SwingActionManager.h"
#include "Player.h"
#include "PlayerMove.h"
#include "Buildings.h"
#include "SwingTarget.h"

namespace{
	const float SWING_ACTION_SCOPE_ANGLE_MIN = 0.78533;//スイングアクション有効範囲の角度の下限(Rad単位)
	
	const float SWING_ACTION_SCOPE_ANGLE_MAX = 1.04719;//スイングアクション有効範囲の角度の上限(Rad単位)
}

//開始処理
bool SwingActionManager::Start()
{
	m_player = FindGO<Player>("player");
	return true;
}

//実行処理
void SwingActionManager::Execute()
{
	// 毎フレーム候補リストをクリアする
	std::vector<Vector3> allTargets;
	Vector3 playerPos = m_player->GetPosition();

	//ビルから、有効範囲内のスイングターゲットを探す
	auto& buildingDatas = FindGOs<Buildings>("buildings");
	for (auto& buildingData : buildingDatas)
	{
		//建物のタイプがタワーのとき処理をスキップする
		if (buildingData->GetBuildingType() == Buildings::enBuildingsType_Tower)
		{
			continue;
		}

		//ビルからの距離が遠すぎたら処理をスキップする
		if ((buildingData->GetPosition() - playerPos).LengthSq() > m_swingTargetScopeRadius * m_swingTargetScopeRadius)
		{
			continue;
		}

		//このビルのスイングターゲットを全て候補リストに追加
		auto* swingTarget = buildingData->GetBuildSwingTarget();
		const auto& targetPositions = swingTarget->GetSwingTargetPositions();
		allTargets.insert(allTargets.end(), targetPositions.begin(), targetPositions.end());
	}

	//スイングターゲットがなければ、見つからなかったとして処理を終了
	if (allTargets.empty())
	{
		m_isSwingTargetScopeRadiusFound = false;
		return;
	}

	//探したスイングターゲットの中から、プレイヤーに最も近いものを探す
	float shortestDistSq = FLT_MAX;
	Vector3 bestTarget = Vector3::Zero;

	//プレイヤーの前方向を取得
	Vector3 playerForward = g_camera3D->GetForward();
	playerForward.y = 0.0f;
	playerForward.Normalize();

	for (const auto& targetPos : allTargets)
	{
		Vector3 toTarget = targetPos - playerPos;

		//プレイヤーの背後にあるターゲットは除外
		if (playerForward.Dot(toTarget) < 0.0f)
		{
			continue;
		}

		//支点からプレイヤーへのベクトル
		Vector3 toPlayer = playerPos - targetPos;
		//toPlayer.x = 0.0f;
		toPlayer.Normalize();

		//支点から真下へのベクトル
		const Vector3 verticalDown = Vector3(0.0f, -1.0f, 0.0f);

		//2つのベクトルの内積を求める
		float dotResult = verticalDown.Dot(toPlayer);

		float angle = acos(dotResult);

		//スイングできる有効範囲の角度ではなければ除外
		if (angle < SWING_ACTION_SCOPE_ANGLE_MIN && angle > SWING_ACTION_SCOPE_ANGLE_MAX)
		{
			continue;
		}

		float distSq = toTarget.LengthSq();
		if (distSq < shortestDistSq)
		{
			shortestDistSq = distSq;
			bestTarget = targetPos;
		}
	}

	//最終的に見つかった最も近いターゲットをセットする
	if (shortestDistSq != FLT_MAX)
	{
		m_swingActionShortestDistance = bestTarget;
		m_isSwingTargetScopeRadiusFound = true;
	}
	else
	{
		m_isSwingTargetScopeRadiusFound = false;
	}
}