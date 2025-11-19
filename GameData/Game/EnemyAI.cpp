#include "stdafx.h"
#include "EnemyAI.h"

namespace {
	const float MOVE_SPEED = 1200.0f;//移動速度
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

	return true;
}

//敵のAIの制御の実行処理
void EnemyAI::Execute(Vector3& position, Quaternion& rotation)
{
	//目標位置を設定するなら
	if (m_isSetTargetPos)
	{
		SetTargetPos();//目標位置を設定する処理

		PathUpdate(position);//目標地点への経路の更新処理
	}

	//移動処理
	Vector3 oldPosition = position;//前の位置を設定
	float moveAmount = MOVE_SPEED * g_gameTime->GetFrameDeltaTime();
	bool isEnd = false;
	position = m_path.Move(
		position,//移動位置
		moveAmount,//移動速度
		isEnd//パス移動が終了したかどうかのフラグ
	);

	//目標位置まで行ったら
	if (isEnd)
	{
		m_isSetTargetPos = true;//目標位置を設定する
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
	//パス検索
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