#pragma once
#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"
#include <random>

/// <summary>
/// 敵AIを制御するクラス
/// </summary>
class Enemy;
class QteEvent;
class Game;
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
	/// 通常の目標位置の設定する処理
	/// </summary>
	void NormalSetTargetPos();

	/// <summary>
	/// 捕獲する逃走車の数が少ないときの目標位置を設定する処理
	/// </summary>
	void FewEscapeCarSetTargetPos();

	/// <summary>
	/// 目標地点への経路の更新処理
	/// </summary>
	/// <param name="position">敵の位置</param>
	void PathUpdate(const Vector3& position);

	/// <summary>
	/// QTEイベントで失敗したときの敵の動作
	/// </summary>
	/// <param name="rotation">敵の回転</param>
	void QteEventFailedEnemyMove(Quaternion& rotation);

	/// <summary>
	/// QTEイベントで成功したときの敵の動作
	/// </summary>
	/// <param name="position">敵の位置</param>
	void QteEventSuccessEnemyMove(const Vector3& position);

public://メンバ関数

	/// <summary>
	/// 敵AIの制御の実行処理
	/// </summary>
	/// <param name="position">敵の位置</param>
	/// <param name="rotation">敵の回転</param>
	void Execute(Vector3& position, Quaternion& rotation);

	/// <summary>
	/// 敵クラスのポインタの設定
	/// </summary>
	/// <param name="enemy">敵クラスのポインタ</param>
	void SetEnemyPtr(Enemy* enemy)
	{
		m_enemy = enemy;
	}

	/// <summary>
	/// 急加速しているか?
	/// </summary>
	/// <returns>trueなら急加速している</returns>
	bool IsSpeedUp() const
	{
		return m_isSpeedUp;
	}

private://メンバ変数
	Vector3 m_targetPos = Vector3::Zero;//目標位置
	Vector3 m_moveDirection = Vector3::Zero;//進行方向
	Vector3 m_velocity = Vector3::Zero;//慣性
	Quaternion m_rotation = Quaternion::Identity;//回転
	int m_qteFailedRotationCount = 0;//QTEイベントで失敗したときに敵が回転した回数
	int m_numCells = 0;//ナビメッシュのセル数
	float m_moveSpeed = 0.0f;//移動速度
	float m_totalDistance = 0.0f;//合計距離
	Vector3 m_buildingCollisionShortDistance = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);//ビルに衝突する最短距離
	bool m_isSetTargetPos = false;//目標位置を設定するかどうか?
	bool m_isSetBuildingCollisionTargetPos = false;//ビルに衝突する位置を設定したか?
	bool m_isSpeedUp = false;//急加速しているか?
	bool m_isPlayBuildingCollisionSe = false;//ビル衝突SEが再生されているか?
	bool m_isPlayExplosionSe = false;//爆発SEが再生されているか?
	nsAI::NaviMesh m_navMesh;//ナビメッシュ
	nsAI::Path m_path;//パス
	nsAI::PathFinding m_pathFiding;//パス検索
	std::mt19937 m_randomEngine;//乱数生成器
	std::uniform_int_distribution<int> m_cellDistribution;//セル番号を均一に選ぶための分布
	std::vector<int> m_cellIndices;//ナビメッシュのセル番号の配列
	std::vector<float> m_distanceData;//距離データ格納用の配列
	Enemy* m_enemy = nullptr;//敵用のインスタンス
	QteEvent* m_qteEvent = nullptr;//QTEイベント用のインスタンス
};

