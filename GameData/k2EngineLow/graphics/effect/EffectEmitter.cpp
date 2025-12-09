#include "k2EngineLowPreCompile.h"
#include "EffectEmitter.h"

namespace nsK2EngineLow {
	EffectEmitter::EffectEmitter()
	{

	}

	EffectEmitter::~EffectEmitter()
	{
		m_effect.Stop();
	}

	void EffectEmitter::Init(const int number)
	{
		m_effect.Init(number);
	}

	void EffectEmitter::Update()
	{
		m_effect.Update();

		if (!m_isLoop)
		{
			if (!IsPlay())
			{
				DeleteGO(this);
			}
		}
	}
}