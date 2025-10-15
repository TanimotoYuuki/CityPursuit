#include "stdafx.h"
#include "EnemyAI.h"

//開始処理
bool EnemyAI::Start()
{
	//ナビメッシュの初期化
	m_navMesh.Init("Assets/navMesh/enemyNavMesh.tkn");
	return true;
}

//敵のAIの制御の実行処理
void EnemyAI::Execute(Vector3& position, const Vector3& targetPosition)
{
	bool isEnd;
	if (g_pad[0]->IsTrigger(enButtonRB1))
	{
		//パス検索
		m_pathFiding.Execute(
			m_path,//構築されたパスの格納先
			m_navMesh,//ナビメッシュ
			position,//開始処理
			targetPosition,//移動目標位置
			PhysicsWorld::GetInstance(),//物理エンジン
			1000.0f,//AIエージェントの半径
			2000.0f//AIエージェントの高さ
		);
	}

	position = m_path.Move(
		position,//移動位置
		10,//移動速度
		isEnd//パス移動が終了したかどうかのフラグ
	);
}