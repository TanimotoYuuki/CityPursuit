#pragma once
#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"

/// <summary>
/// 敵AIを制御するクラス
/// </summary>
class EnemyAI : public IGameObject
{
public:
	EnemyAI() {};//コンストラクタ
	~EnemyAI() {};//デストラクタ

	bool Start();//開始処理

	/// <summary>
	/// 敵AIの制御の実行処理
	/// </summary>
	/// <param name="position">敵の位置</param>
	/// <param name="targetPosition">ターゲットの位置</param>
	void Execute(Vector3& position, const Vector3& targetPosition);

private://メンバ変数
	nsAI::NaviMesh m_navMesh;//ナビメッシュ
	nsAI::Path m_path;//パス
	nsAI::PathFinding m_pathFiding;//パス検索
};

