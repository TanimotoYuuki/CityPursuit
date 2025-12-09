#include "stdafx.h"
#include "PlayerEffect.h"

//デストラクタ
PlayerEffect::~PlayerEffect()
{
	DeleteGO(m_playerEffect[enPlayerEffectList_Swing]);
}

//開始処理
bool PlayerEffect::Start()
{
	EffectEngine::GetInstance()->ResistEffect(EffectList_Swing, u"Assets/effect/swing.efk");

	//エフェクトの初期化
	m_playerEffect[enPlayerEffectList_Swing] = NewGO<EffectEmitter>(1);
	m_playerEffect[enPlayerEffectList_Swing]->Init(EffectList_Swing);
	m_playerEffect[enPlayerEffectList_Swing]->SetLoop(true);

	return true;
}

//実行処理
void PlayerEffect::Execute(const Vector3& position, const Quaternion& rotation)
{
	if (m_currentPlayEffect != enPlayerEffectList_None)
	{
		if (!m_playerEffect[m_currentPlayEffect]->IsPlay())
		{
			PlayEffect();
		}
	}

	if (m_formerPlayEffect != enPlayerEffectList_None)
	{
		if (m_playerEffect[m_formerPlayEffect]->IsPlay())
		{
			StopEffect();
		}
	}

	if (m_currentPlayEffect == enPlayerEffectList_Swing)
	{
		//エフェクトの位置の設定
		m_playerEffect[enPlayerEffectList_Swing]->SetPosition(position);

		//エフェクトの回転の設定
		m_playerEffect[enPlayerEffectList_Swing]->SetRotation(rotation);

		//エフェクトの大きさの設定
		m_playerEffect[enPlayerEffectList_Swing]->SetScale({ 25.0f,25.0f,25.0f });
	}
}