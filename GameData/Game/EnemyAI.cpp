#include "stdafx.h"
#include "EnemyAI.h"
#include "Enemy.h"
#include "EnemyEffect.h"
#include "SwingTarget.h"
#include "Buildings.h"
#include "QteEvent.h"

namespace {
	const float MOVE_SPEED = 1200.0f;//移動速度
	const float QTE_FAILED_MOVE_SPEED = 2000.0f;//移動速度(QTEイベントで失敗したとき)
	const float BUILDING_DISTANCE_OFFSET = 650.0f;//ビルの距離用のオフセット
}

//開始処理
bool EnemyAI::Start()
{
	//ナビメッシュの初期化
	m_navMesh.Init("Assets/navMesh/enemyNavMesh.tkn");

	int numCells = m_navMesh.GetNumCell();

	//乱数生成器の初期化
	std::random_device rd;
	m_randomEngine.seed(rd());

	//乱数の範囲の設定
	m_cellDistribution = std::uniform_int_distribution<int>(0, numCells - 1);

	//目標位置を設定する
	m_isSetTargetPos = true;

	//移動速度の設定
	m_moveSpeed = MOVE_SPEED;

	//QTEイベントクラスのインスタンスの生成
	m_qteEvent = FindGO<QteEvent>("qteevent");

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

	//移動処理
	Vector3 oldPosition = position;//前の位置を設定
	float moveAmount = m_moveSpeed * g_gameTime->GetFrameDeltaTime();
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
	Vector3 moveDir = position - oldPosition;
	moveDir.y = 0.0f;

	if (moveDir.LengthSq() > 0.001f)
	{
		moveDir.Normalize();//方向ベクトル化

		Quaternion targetRotation;

		targetRotation.SetRotation(Vector3::Front, moveDir);

		float interpolationFactor = 0.1f;
		m_rotation.Slerp(interpolationFactor, m_rotation, targetRotation);
	}
	rotation = m_rotation;

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
	//乱数生成器の実行
	int randomCellIndex = m_cellDistribution(m_randomEngine);
	
	//乱数生成器で出た値を使ってナビメッシュのセルを取得
	const nsAI::Cell& randomCell = m_navMesh.GetCell(randomCellIndex);

	//ナビメッシュの中央座標の取得
	m_targetPos = randomCell.GetCenterPosition();

	m_isSetTargetPos = false;//目標位置の設定が終わった
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
			m_enemy->GetEnemyEffectPtr()->ChangeEffect(EnemyEffect::enEnemyEffectList_Explosion);
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