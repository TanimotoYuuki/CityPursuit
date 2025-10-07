#include "stdafx.h"
#include "SwingActionManager.h"
#include "Player.h"
#include "PlayerMove.h"
#include "Buildings.h"
#include "SwingTarget.h"

//開始処理
bool SwingActionManager::Start()
{
	m_player = FindGO<Player>("player");
	return true;
}

//実行処理
void SwingActionManager::Execute()
{
	//建物のクラスのインスタンスの検索
	auto& buildingDatas = FindGOs<Buildings>("buildings");
	for (auto& buildingData : buildingDatas)
	{
		//スイングターゲットを探す処理
		FindSwingTarget(buildingData);
	}

	//ビルとその距離のデータを格納するコンテナのサイズが0の場合はスイングターゲットが見つかっていない
	if (m_swingTargetDistanceData.size() == 0)
	{
		m_isSwingTargetScopeRadiusFound = false;
	}
}

//スイングターゲットを探す処理
void SwingActionManager::FindSwingTarget(Buildings* buildingData)
{
	//建物のタイプがタワーのとき早期リターンする
	if (buildingData->GetBuildingType() == Buildings::enBuildingsType_Tower)
	{
		return;
	}

	//プレイヤーの位置からスイングターゲットの距離
	Vector3 distance = buildingData->GetPosition() - m_player->GetModelData().GetPosition();

	//有効範囲内にビルがあったら最短距離のスイングターゲットを探す処理をする
	if (distance.Length() < m_swingTargetScopeRadius)
	{
		//プレイヤーの位置からビルの距離をコンテナに格納する
		m_buildingDistanceData[buildingData] = distance;

		//距離データを距離の短い順にソートする
		std::vector<std::pair<Buildings*, Vector3>> distanceData(m_buildingDistanceData.begin(), m_buildingDistanceData.end());
		std::sort(distanceData.begin(), distanceData.end(),
			[](auto& a, auto& b)
			{
				return a.second.Length() < b.second.Length();
			});

		//プレイヤーの進行方向のベクトルの取得

		//X軸方向に移動したとき処理する
		//if (m_player->GetPlayerMove()->GetMoveSpeed().x != 0.0f)
		//{
		m_playerMoveDir = g_camera3D->GetForward()/*m_player->GetPlayerMove()->GetMoveSpeed().x*/;
		//}

		//Y軸要素の除外
		m_playerMoveDir.y = 0.0f;

		//Z軸方向に移動したとき処理する
		//if (m_player->GetPlayerMove()->GetMoveSpeed().z != 0.0f)
		//{
		/*	m_playerMoveDir.z = m_player->GetPlayerMove()->GetMoveSpeed().z;*/
		//}

		//プレイヤーの進行方向のベクトルの正規化
		m_playerMoveDir.Normalize();

		//プレイヤーの進行方向ベクトルが0のとき早期リターンする
		if (m_playerMoveDir.Length() == 0.0f)
		{
			return;
		}

		//プレイヤーの進行方向ベクトルとプレイヤーの位置からスイングターゲットの距離の内積の計算
		float dotResult = m_playerMoveDir.Dot(distance);

		//内積の結果がマイナスのとき早期リターンする
		if (dotResult < 0.0f)
		{
			//スイングターゲットを探す用のコンテナの削除
			//ビル用のコンテナ
			m_buildingDistanceData.erase(buildingData);

			//スイングターゲット用のコンテナ
			m_swingTargetDistanceData.erase(buildingData);

			return;
		}

		auto buildingDistanceData = m_buildingDistanceData.begin();
		//ある程度絞れた建物のスイングターゲットのインスタンスの検索
		auto* swingTarget = buildingDistanceData->first->GetBuildSwingTarget();

		//建物のスイングターゲットの位置番号
		int posNum = 0;

		//プレイヤーから一番近いスイングターゲットの計算
		//計算されたスイングターゲットの座標コンテナを取得
		const std::vector<Vector3>& swingTargetPositions = swingTarget->GetSwingTargetPositions();
		for (auto& stPos : swingTargetPositions)
		{
			Vector3 distance = swingTargetPositions[posNum] - m_player->GetModelData().GetPosition();

			//プレイヤーの位置からスイングターゲットの距離をコンテナに格納する
			m_swingTargetDistanceData[buildingData] = distance;

			//距離データを距離の短い順にソートする
			std::vector<std::pair<Buildings*, Vector3>> distanceData(m_swingTargetDistanceData.begin(), m_swingTargetDistanceData.end());
			std::sort(distanceData.begin(), distanceData.end(),
				[](auto& a, auto& b)
				{
					return a.second.Length() < b.second.Length();
				});

			auto swingTargetDistanceData = m_swingTargetDistanceData.begin();
			m_swingActionShortestDistance = swingTargetDistanceData->second;

			posNum++;
		}

		//スイングターゲットが見つかった
		m_isSwingTargetScopeRadiusFound = true;
	}
	//有効範囲内にビルがなかったら最短距離のスイングターゲットを探す用のコンテナの削除
	else
	{
		//ビル用のコンテナ
		m_buildingDistanceData.erase(buildingData);

		//スイングターゲット用のコンテナ
		m_swingTargetDistanceData.erase(buildingData);
	}
}