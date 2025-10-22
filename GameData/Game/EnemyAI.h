#pragma once
#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"
#include <random>

/// <summary>
/// 敵AIを制御するクラス
/// </summary>
class EnemyAI : public IGameObject
{
public:
	EnemyAI() {};//コンストラクタ
	~EnemyAI() {};//デストラクタ

	bool Start();//開始処理

private://メンバ関数

	/// <summary>
	/// 目標位置を設定する処理
	/// </summary>
	void SetTargetPos();

	/// <summary>
	/// 目標地点への経路の更新処理
	/// </summary>
	/// <param name="position">敵の位置</param>
	void PathUpdate(const Vector3& position);

public://メンバ関数

	/// <summary>
	/// 敵AIの制御の実行処理
	/// </summary>
	/// <param name="position">敵の位置</param>
	/// <param name="rotation">敵の回転</param>
	void Execute(Vector3& position, Quaternion& rotation);

private://メンバ変数
	Vector3 m_targetPos = Vector3::Zero;//目標位置
	Quaternion m_rotation = Quaternion::Identity;//回転
	bool m_isSetTargetPos = false;//目標位置を設定するかどうか?
	nsAI::NaviMesh m_navMesh;//ナビメッシュ
	nsAI::Path m_path;//パス
	nsAI::PathFinding m_pathFiding;//パス検索
	std::mt19937 m_randomEngine;//乱数生成器
	std::uniform_int_distribution<int> m_cellDistribution;//セル番号を均一に選ぶための分布
};

