#include "stdafx.h"
#include "Enemy.h"
#include "EnemyAI.h"
#include "EnemyEffect.h"

//デストラクタ
Enemy::~Enemy()
{
	DeleteGO(m_enemyAI);
}

//開始処理
bool Enemy::Start()
{
	//敵モデルの初期化
	m_enemyModel.Init("Assets/modelData/car/car.tkm", nullptr, 0, enModelUpAxisZ, true, true);
	//敵モデルの位置の設定
	m_enemyModel.SetPosition(m_position);
	//敵モデルの向きの設定
	m_enemyModel.SetRotation(m_rotation);
	//敵モデルの大きさの設定
	m_enemyModel.SetScale(m_scale);
	//敵モデルの更新処理
	m_enemyModel.Update();

	//敵AIクラスのインスタンスの生成
	m_enemyAI = NewGO<EnemyAI>(0, "enemyai");
	m_enemyAI->SetEnemyPtr(this);

	//敵のエフェクトクラスのインスタンスの生成
	m_enemyEffect = NewGO<EnemyEffect>(0, "enemyeffect");

	return true;
}

//更新処理
void Enemy::Update()
{
	//敵AIの実行処理
	m_enemyAI->Execute(m_position, m_rotation);

	////検証用のステージは直進のみなので
	////敵を真っすぐ移動させる
	//m_position.z += 900.0f * g_gameTime->GetFrameDeltaTime();

	//敵のエフェクトの実行処理
	m_enemyEffect->Execute(m_position, m_rotation);

	//敵モデルの位置の設定
	m_enemyModel.SetPosition(m_position);

	//敵モデルの回転の設定
	m_enemyModel.SetRotation(m_rotation);

	//敵モデルの更新処理
	m_enemyModel.Update();
}

//描画処理
void Enemy::Render(RenderContext& rc)
{
	//敵モデルの描画処理
	m_enemyModel.Draw(rc);
}