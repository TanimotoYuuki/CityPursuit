#include "stdafx.h"
#include "EnemyEffect.h"

//デストラクタ
EnemyEffect::~EnemyEffect()
{
	for (int i = 0; i < enEnemyEffectList_Num; i++)
	{
		if (m_enemyEffect[i] == nullptr)
		{
			continue;
		}

		DeleteGO(m_enemyEffect[i]);
	}
}

//開始処理
bool EnemyEffect::Start()
{
	//エフェクトエンジンに敵のエフェクトに使う情報の登録
	EffectEngine::GetInstance()->ResistEffect(EffectList_EngineSmoke, u"Assets/effect/engineSmoke.efk");
	EffectEngine::GetInstance()->ResistEffect(EffectList_EngineSmoke_Large, u"Assets/effect/engineSmoke1.efk");
	EffectEngine::GetInstance()->ResistEffect(EffectList_Explosion, u"Assets/effect/explosion.efk");

	//エフェクトの初期化
	m_enemyEffect[enEnemyEffectList_EngineSmoke_Left] = NewGO<EffectEmitter>(1);
	m_enemyEffect[enEnemyEffectList_EngineSmoke_Left]->Init(EffectList_EngineSmoke);
	m_enemyEffect[enEnemyEffectList_EngineSmoke_Left]->SetLoop(true);

	m_enemyEffect[enEnemyEffectList_EngineSmoke_Right] = NewGO<EffectEmitter>(1);
	m_enemyEffect[enEnemyEffectList_EngineSmoke_Right]->Init(EffectList_EngineSmoke);
	m_enemyEffect[enEnemyEffectList_EngineSmoke_Right]->SetLoop(true);

	m_enemyEffect[enEnemyEffectList_EngineSmoke_Large_Left] = NewGO<EffectEmitter>(1);
	m_enemyEffect[enEnemyEffectList_EngineSmoke_Large_Left]->Init(EffectList_EngineSmoke_Large);
	m_enemyEffect[enEnemyEffectList_EngineSmoke_Large_Left]->SetLoop(true);

	m_enemyEffect[enEnemyEffectList_EngineSmoke_Large_Right] = NewGO<EffectEmitter>(1);
	m_enemyEffect[enEnemyEffectList_EngineSmoke_Large_Right]->Init(EffectList_EngineSmoke_Large);
	m_enemyEffect[enEnemyEffectList_EngineSmoke_Large_Right]->SetLoop(true);

	m_enemyEffect[enEnemyEffectList_Explosion] = NewGO<EffectEmitter>(1);
	m_enemyEffect[enEnemyEffectList_Explosion]->Init(EffectList_Explosion);
	m_enemyEffect[enEnemyEffectList_Explosion]->SetLoop(true);

	return true;
}

//実行処理
void EnemyEffect::Execute(const Vector3& position, const Quaternion& rotation)
{
	if (m_currentPlayEffect != enEnemyEffectList_None)
	{
		PlayEffect(m_currentPlayEffect);
	}

	if (m_formerPlayEffect != enEnemyEffectList_None)
	{
		StopEffect(m_formerPlayEffect);
	}

	switch (m_currentPlayEffect)
	{
	case EnemyEffect::enEnemyEffectList_EngineSmoke:
		PlayEngineSmokeEffect(position, rotation);
		break;
	case EnemyEffect::enEnemyEffectList_EngineSmoke_Large:
		PlayEngineSmokeLargeEffect(position, rotation);
		break;
	case EnemyEffect::enEnemyEffectList_Explosion:
		PlayExplosionEffect(position);
		break;
	default:
		break;
	}
}

//エフェクトの再生
void EnemyEffect::PlayEffect(EnEnemyEffectList currentPlayEffect)
{
	switch (currentPlayEffect)
	{
	case enEnemyEffectList_EngineSmoke:
		if (!m_enemyEffect[enEnemyEffectList_EngineSmoke_Left]->IsPlay())
		{
			m_enemyEffect[enEnemyEffectList_EngineSmoke_Left]->Play();
		}

		if (!m_enemyEffect[enEnemyEffectList_EngineSmoke_Right]->IsPlay())
		{
			m_enemyEffect[enEnemyEffectList_EngineSmoke_Right]->Play();
		}
		break;
	case enEnemyEffectList_EngineSmoke_Large:
		if (!m_enemyEffect[enEnemyEffectList_EngineSmoke_Large_Left]->IsPlay())
		{
			m_enemyEffect[enEnemyEffectList_EngineSmoke_Large_Left]->Play();
		}

		if (!m_enemyEffect[enEnemyEffectList_EngineSmoke_Large_Right]->IsPlay())
		{
			m_enemyEffect[enEnemyEffectList_EngineSmoke_Large_Right]->Play();
		}
		break;
	case enEnemyEffectList_Explosion:
		if (!m_enemyEffect[enEnemyEffectList_Explosion]->IsPlay())
		{
			m_enemyEffect[enEnemyEffectList_Explosion]->Play();
		}
		break;
	default:
		break;
	}
}

//エフェクトの停止
void EnemyEffect::StopEffect(EnEnemyEffectList formerPlayEffect)
{
	switch (formerPlayEffect)
	{
	case enEnemyEffectList_EngineSmoke:
		if (m_enemyEffect[enEnemyEffectList_EngineSmoke_Left]->IsPlay())
		{
			m_enemyEffect[enEnemyEffectList_EngineSmoke_Left]->Stop();
		}

		if (m_enemyEffect[enEnemyEffectList_EngineSmoke_Right]->IsPlay())
		{
			m_enemyEffect[enEnemyEffectList_EngineSmoke_Right]->Stop();
		}
		break;
	case enEnemyEffectList_EngineSmoke_Large:
		if (m_enemyEffect[enEnemyEffectList_EngineSmoke_Large_Left]->IsPlay())
		{
			m_enemyEffect[enEnemyEffectList_EngineSmoke_Large_Left]->Stop();
		}

		if (m_enemyEffect[enEnemyEffectList_EngineSmoke_Large_Right]->IsPlay())
		{
			m_enemyEffect[enEnemyEffectList_EngineSmoke_Large_Right]->Stop();
		}
		break;
	case enEnemyEffectList_Explosion:
		if (m_enemyEffect[enEnemyEffectList_Explosion]->IsPlay())
		{
			m_enemyEffect[enEnemyEffectList_Explosion]->Stop();
		}
		break;
	default:
		break;
	}
}

//エンジンの煙エフェクトの再生処理
void EnemyEffect::PlayEngineSmokeEffect(const Vector3& position, const Quaternion& rotation)
{
	//敵が回転してもエンジンの煙エフェクトが追従できるようにする処理
	Quaternion localRot;
	localRot.SetRotationDegY(180.0f);

	Quaternion effectRotation;
	effectRotation = rotation;
	effectRotation.Multiply(localRot);

	Vector3 localPosLeft = { -65.0f,0.0f,-150.0f };
	Vector3 localPosRight = { 65.0f,0.0f,-150.0f };

	rotation.Apply(localPosLeft);
	rotation.Apply(localPosRight);

	localPosLeft += position;
	localPosRight += position;

	//エンジンの煙エフェクトの位置の設定
	m_enemyEffect[enEnemyEffectList_EngineSmoke_Left]->SetPosition(localPosLeft);
	m_enemyEffect[enEnemyEffectList_EngineSmoke_Right]->SetPosition(localPosRight);

	//エンジンの煙エフェクトの回転の設定
	m_enemyEffect[enEnemyEffectList_EngineSmoke_Left]->SetRotation(effectRotation);
	m_enemyEffect[enEnemyEffectList_EngineSmoke_Right]->SetRotation(effectRotation);

	//エンジンの煙エフェクトの大きさの設定
	m_enemyEffect[enEnemyEffectList_EngineSmoke_Left]->SetScale({ 50.0f,50.0f,50.0f });
	m_enemyEffect[enEnemyEffectList_EngineSmoke_Right]->SetScale({ 50.0f,50.0f,50.0f });
}

//エンジンの煙大エフェクトの再生処理
void EnemyEffect::PlayEngineSmokeLargeEffect(const Vector3& position, const Quaternion& rotation)
{
	//敵が回転してもエンジンの煙エフェクトが追従できるようにする処理
	Quaternion localRot;
	localRot.SetRotationDegY(180.0f);

	Quaternion effectRotation;
	effectRotation = rotation;
	effectRotation.Multiply(localRot);

	Vector3 localPosLeft = { -65.0f,0.0f,-150.0f };
	Vector3 localPosRight = { 65.0f,0.0f,-150.0f };

	rotation.Apply(localPosLeft);
	rotation.Apply(localPosRight);

	localPosLeft += position;
	localPosRight += position;

	//エンジンの煙大エフェクトの位置の設定
	m_enemyEffect[enEnemyEffectList_EngineSmoke_Large_Left]->SetPosition(localPosLeft);
	m_enemyEffect[enEnemyEffectList_EngineSmoke_Large_Right]->SetPosition(localPosRight);

	//エンジンの煙大エフェクトの回転の設定
	m_enemyEffect[enEnemyEffectList_EngineSmoke_Large_Left]->SetRotation(effectRotation);
	m_enemyEffect[enEnemyEffectList_EngineSmoke_Large_Right]->SetRotation(effectRotation);

	//エンジンの煙大エフェクトの大きさの設定
	m_enemyEffect[enEnemyEffectList_EngineSmoke_Large_Left]->SetScale({ 50.0f,50.0f,50.0f });
	m_enemyEffect[enEnemyEffectList_EngineSmoke_Large_Right]->SetScale({ 50.0f,50.0f,50.0f });
}

//爆発エフェクトの再生処理
void EnemyEffect::PlayExplosionEffect(const Vector3& position)
{
	//爆発エフェクトの位置の設定
	m_enemyEffect[enEnemyEffectList_Explosion]->SetPosition(position);

	//爆発エフェクトの大きさの設定
	m_enemyEffect[enEnemyEffectList_Explosion]->SetScale({ 10.0f,10.0f,10.0f });
}