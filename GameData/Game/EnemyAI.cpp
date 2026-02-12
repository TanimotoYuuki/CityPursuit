#include "stdafx.h"
#include "EnemyAI.h"
#include "Enemy.h"
#include "EnemyEffect.h"
#include "SwingTarget.h"
#include "Player.h"
#include "Game.h"
#include "GameMission.h"
#include "Buildings.h"
#include "QteEvent.h"
#include "DebugLog.h"

namespace {
	const float MOVE_SPEED = 1200.0f;//移動速度
	const float QTE_FAILED_MOVE_SPEED = 2000.0f;//移動速度(QTEイベントで失敗したとき)
	const float BUILDING_DISTANCE_OFFSET = 650.0f;//ビルの距離用のオフセット
	const float DISTANCE_OFFSET = 1500.0f * 1500.0f;//距離用のオフセット
	const int CELL_NUM = 480;//ナビメッシュのセル数
}

//開始処理
bool EnemyAI::Start()
{
	//ナビメッシュの初期化
	m_navMesh.Init("Assets/navMesh/enemyNavMesh.tkn");

	m_numCells = m_navMesh.GetNumCell();

	//乱数生成器の初期化
	std::random_device rd;
	m_randomEngine.seed(rd());

	//乱数の範囲の設定
	m_cellDistribution = std::uniform_int_distribution<int>(0, m_numCells - 1);

	//目標位置を設定する
	m_isSetTargetPos = true;

	//移動速度の設定
	m_moveSpeed = MOVE_SPEED;

	//QTEイベントクラスのインスタンスの生成
	m_qteEvent = FindGO<QteEvent>("qteevent");

	//ナビメッシュのセルインデックスのメモリ確保
	m_cellIndices.reserve(CELL_NUM);

	//距離データ格納用の配列のメモリ確保
	m_distanceData.reserve(CELL_NUM);

	return true;
}

//敵のAIの制御の実行処理
void EnemyAI::Execute(Vector3& position, Quaternion& rotation)
{
	//QTEイベントで結果が出ていないとき
	if (m_qteEvent->IsQteEventResult(QteEvent::enQteEventResult_Success) != true)
	{
		//目標位置を設定するなら
		if (m_isSetTargetPos)
		{
			SetTargetPos();//目標位置を設定する処理

			PathUpdate(position);//目標地点への経路の更新処理
		}
	}
	//QTEイベントで結果が出ているとき
	else
	{
		//プレイヤーが乗っているときに処理する
		if (m_enemy->IsOnPlayer())
		{
			//QTEイベントで成功したとき
			if (m_qteEvent->IsQteEventResult(QteEvent::enQteEventResult_Success))
			{
				//QTEイベントで成功したときの敵の挙動
				QteEventSuccessEnemyMove(position);
			}
		}
	}

	Vector3 currentForward = Vector3::Front;
	m_rotation.Apply(currentForward);
	float dot = currentForward.Dot(m_moveDirection);

	float speedMultiplier = 1.0f;
	if (dot < 0.7f)
	{
		speedMultiplier = Math::Lerp(0.4f, 1.0f, (dot + 1.0f) / 1.7f);
	}

	//移動処理
	Vector3 oldPosition = position;//前の位置を設定
	float moveAmount = (m_moveSpeed * speedMultiplier) * g_gameTime->GetFrameDeltaTime();
	bool isEnd = false;
	position = m_path.Move(
		position,//移動位置
		moveAmount,//移動速度
		isEnd//パス移動が終了したかどうかのフラグ
	);

	//目標位置まで行ったら
	if (isEnd)
	{
		if (!m_isSetBuildingCollisionTargetPos)
		{
			m_isSetTargetPos = true;//目標位置を設定する
		}
	}

	//回転処理
	m_moveDirection = position - oldPosition;
	m_moveDirection.y = 0.0f;
	
	if (m_moveDirection.LengthSq() > 0.001f)
	{
		m_moveDirection.Normalize();//方向ベクトル化

		Quaternion targetRotation;
		targetRotation.SetRotation(Vector3::Front, m_moveDirection);

		float interpolationFactor = (dot < 0.0f) ? 0.05f : 0.1f;
		m_rotation.Slerp(interpolationFactor, m_rotation, targetRotation);

		Vector3 currentRight = Vector3::Right;
		m_rotation.Apply(currentRight);
		float steerAmount = m_moveDirection.Dot(currentRight);

		Quaternion rollRotation;

		float rollAngle = -steerAmount * Math::DegToRad(5.0f);
		rollRotation.SetRotation(Vector3::Front, rollAngle);

		rotation = m_rotation * rollRotation;
	}
	else
	{
		rotation = m_rotation;
	}

	//プレイヤーが乗っているときに処理する
	if (m_enemy->IsOnPlayer())
	{
		//QTEイベントで失敗したとき
		if (m_qteEvent->IsQteEventResult(QteEvent::enQteEventResult_Failed))
		{
			//QTEイベントで失敗したときの敵の挙動
			QteEventFailedEnemyMove(rotation);
		}
	}
	else
	{
		m_qteFailedRotationCount = 0;
		m_moveSpeed = MOVE_SPEED;

		if (!GameSoundEngine::GetInstance()->IsPlayingSound(GameSoundList_SE_CarEngine))
		{
			GameSoundEngine::GetInstance()->PlaySE(GameSoundList_SE_CarEngine, 15.0f, true);
		}
		GameSoundEngine::GetInstance()->SetPosition(GameSoundList_SE_CarEngine, position);

		if (m_enemy->GetEnemyEffectPtr()->IsPlayEffect() != EnemyEffect::enEnemyEffectList_EngineSmoke_Large)
		{
			m_isSpeedUp = false;
		}
		if (m_enemy->GetEnemyEffectPtr()->IsPlayEffect() != EnemyEffect::enEnemyEffectList_Explosion)
		{
			m_enemy->GetEnemyEffectPtr()->ChangeEffect(EnemyEffect::enEnemyEffectList_EngineSmoke);
		}
	}
}

//目標位置を設定する処理
void EnemyAI::SetTargetPos()
{
	auto* capruterEnemy = m_enemy->GetGamePtr()->GetGameMissionPtr();

	//逃走車を何台か捕獲したときに処理する
	if (capruterEnemy->GetCurrentCaptureEnemyNum() > 0)
	{
		//捕獲する逃走車の数が少ないときの目標位置を設定する処理
		FewEscapeCarSetTargetPos();
	}
	//それ以外の場合
	else
	{
		//通常の目標位置の設定する処理
		NormalSetTargetPos();
	}

	m_isSetTargetPos = false;//目標位置の設定が終わった
}

//通常の目標位置の設定する処理
void EnemyAI::NormalSetTargetPos()
{
	//乱数生成器の実行
	int randomCellIndex = m_cellDistribution(m_randomEngine);

	//乱数生成器で出た値を使ってナビメッシュのセルを取得
	const nsAI::Cell& randomCell = m_navMesh.GetCell(randomCellIndex);

	//ナビメッシュの中央座標の取得
	m_targetPos = randomCell.GetCenterPosition();
}

//捕獲する逃走車の数が少ないときの目標位置を設定する処理
void EnemyAI::FewEscapeCarSetTargetPos()
{
	//この関数を実行するたびにコンテナをクリアする
	m_cellIndices.clear();
	m_distanceData.clear();

	m_totalDistance = 0;

	//プレイヤーの位置の取得
	Vector3 playerPosition = m_enemy->GetGamePtr()->GetPlayerPtr()->GetPosition();

	for (int i = 0; i < m_numCells; i++)
	{
		//乱数生成器で出た値を使ってナビメッシュのセルを取得
		const nsAI::Cell& randomCell = m_navMesh.GetCell(i);

		//ナビメッシュの中央座標の取得
		Vector3 cellCenterPos = randomCell.GetCenterPosition();

		//プレイヤーとの距離を計算
		Vector3 distanceToPlayer = cellCenterPos - playerPosition;

		//距離データの格納
		m_distanceData.push_back(distanceToPlayer.LengthSq());

		//距離の加算
		m_totalDistance += distanceToPlayer.LengthSq();

		//進行方向データの格納
		distanceToPlayer.Normalize();
		distanceToPlayer.y = 0.0f;
	}

	//平均の距離の計算
	float averageDistance = m_totalDistance / m_numCells;

	//目標位置を設定する距離の計算
	float setTargetPosDistance = averageDistance - DISTANCE_OFFSET;

	for (int i = 0; i < m_numCells; i++)
	{
		if (m_distanceData[i] < setTargetPosDistance)
		{
			m_cellIndices.push_back(i);
		}
	}

	//乱数の範囲の設定
	m_cellDistribution = std::uniform_int_distribution<int>(m_cellIndices[0], m_cellIndices[m_cellIndices.size() - 1]);

	//乱数生成器の実行
	int randomCellIndex = m_cellDistribution(m_randomEngine);

	//乱数生成器で出た値を使ってナビメッシュのセルを取得
	const nsAI::Cell& randomCell = m_navMesh.GetCell(randomCellIndex);

	//ナビメッシュの中央座標の取得
	m_targetPos = randomCell.GetCenterPosition();
}

//目標地点への経路の更新処理
void EnemyAI::PathUpdate(const Vector3& position)
{
	//ビルに衝突する位置が設定されていたら処理する
	if (m_isSetBuildingCollisionTargetPos)
	{
		//パス検索(QTEイベントで成功用の演出)
		m_pathFiding.Execute(
			m_path,//構築されたパスの格納先
			m_navMesh,//ナビメッシュ
			position,//開始処理
			m_targetPos,//移動目標位置
			PhysicsWorld::GetInstance(),//物理エンジン
			50000.0f,//AIエージェントの半径
			200000.0f//AIエージェントの高さ
		);
		return;
	}

	//パス検索(通常)
	m_pathFiding.Execute(
		m_path,//構築されたパスの格納先
		m_navMesh,//ナビメッシュ
		position,//開始処理
		m_targetPos,//移動目標位置
		PhysicsWorld::GetInstance(),//物理エンジン
		50.0f,//AIエージェントの半径
		200.0f//AIエージェントの高さ
	);
}

//QTEイベントで失敗したときの敵の挙動
void EnemyAI::QteEventFailedEnemyMove(Quaternion& rotation)
{
	//揺さぶる処理
	if (m_qteFailedRotationCount < 60)//60フレームまで揺さぶる
	{
		float angle = Math::DegToRad(10.0f * std::sin(m_qteFailedRotationCount * 0.3f));//揺さぶる角度を設定
		Quaternion shakeRotation;
		shakeRotation.SetRotation(Vector3::Up, angle);//Y軸回転のクォータニオンを設定
		rotation = shakeRotation * rotation;//揺さぶりを適用
		m_qteFailedRotationCount++;//カウントアップ
	}
	else
	{
		m_moveSpeed = QTE_FAILED_MOVE_SPEED;
		m_isSpeedUp = true;
		m_enemy->GetEnemyEffectPtr()->ChangeEffect(EnemyEffect::enEnemyEffectList_EngineSmoke_Large);
	}
}

//QTEイベントで成功したときの敵の挙動
void EnemyAI::QteEventSuccessEnemyMove(const Vector3& position)
{
	//建物衝突用の目標位置が設定されていたら処理しない
	if (m_isSetBuildingCollisionTargetPos)
	{
		//敵とビルの距離を計算
		Vector3 distanceToTarget = m_targetPos - position;

		//一定距離まで行ったら移動を停止する
		if (distanceToTarget.LengthSq() <= BUILDING_DISTANCE_OFFSET * BUILDING_DISTANCE_OFFSET)
		{
			m_moveSpeed = 0.0f;//移動速度を0

			if (!m_isPlayBuildingCollisionSe)
			{
				GameSoundEngine::GetInstance()->PlaySE(GameSoundList_SE_BuildingCollision, 5.0f);
				m_isPlayBuildingCollisionSe = true;
			}
			
			if (!GameSoundEngine::GetInstance()->IsPlayingSound(GameSoundList_SE_BuildingCollision))
			{
				if (!m_isPlayExplosionSe)
				{
					GameSoundEngine::GetInstance()->PlaySE(GameSoundList_SE_Explosion, 1.0f);
					m_enemy->GetEnemyEffectPtr()->ChangeEffect(EnemyEffect::enEnemyEffectList_Explosion);
					m_isPlayExplosionSe = true;
				}
			}

			return;
		}

		return;
	}

	//スイングターゲットクラスのインスタンスの検索
	auto& swingTargets = FindGOs<SwingTarget>("swingtarget");
	//全てのスイングターゲットを調べる
	for (auto& swingTarget : swingTargets) {
		//計算されたスイングターゲットの座標コンテナを取得
		const std::vector<Vector3>& swingTargetPositions = swingTarget->GetSwingTargetPositions();

		int targetNo = 0;
		//全ての座標を調べる
		for (auto& stPos : swingTargetPositions)
		{
			//スイングターゲットの位置と敵の位置の距離を計算
			Vector3 distance = swingTargetPositions[targetNo] - position;
			targetNo++;

			//最短距離の更新処理
			if (distance.LengthSq() < m_buildingCollisionShortDistance.LengthSq())
			{
				m_targetPos = distance;
			}
		}
	}

	//建物クラスのインスタンスの検索
	auto& buildings = FindGOs<Buildings>("buildings");
	//全ての建物を調べる
	for (auto& building : buildings) {
		//タワーは無視する
		if (building->GetBuildingType() == Buildings::enBuildingsType_Tower)
		{
			continue;
		}

		//ビルの位置と敵の位置の距離を計算
		Vector3 buildingPosition = building->GetPosition();
		Vector3 distanceToBuilding = buildingPosition - position;

		//最短距離の更新処理
		if (distanceToBuilding.LengthSq() < m_buildingCollisionShortDistance.LengthSq())
		{
			m_buildingCollisionShortDistance = distanceToBuilding;
			m_targetPos = buildingPosition; // 最短距離のビルを目標位置に設定
		}
	}

	m_isSetBuildingCollisionTargetPos = true;//建物衝突用の目標位置が設定された
	//目標地点への経路の更新処理
	PathUpdate(position);
}