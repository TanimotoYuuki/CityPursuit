#include "stdafx.h"
#include "Enemy.h"
#include "EnemyAI.h"
#include "Player.h"//デバッグ用

//デストラクタ
Enemy::~Enemy()
{
	DeleteGO(m_enemyAI);
}

//開始処理
bool Enemy::Start()
{
	//敵モデルの初期化
	m_enemyModel.Init("Assets/modelData/car/car.tkm");
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
	return true;
}

//更新処理
void Enemy::Update()
{
	//プレイヤーのインスタンスが存在しているとき処理する
	if (m_player != nullptr)
	{
		//敵AIの実行処理
		m_enemyAI->Execute(m_position, m_player->GetModelData().GetPosition());
	}
	else
	{
		//デバッグ用
		m_player = FindGO<Player>("player");
	}

	//敵モデルの位置の設定
	m_enemyModel.SetPosition(m_position);

	//敵モデルの更新処理
	m_enemyModel.Update();
}

//描画処理
void Enemy::Render(RenderContext& rc)
{
	//敵モデルの描画処理
	m_enemyModel.Draw(rc);
}