#include "stdafx.h"
#include "FadeManager.h"

FadeManager* FadeManager::m_instance = nullptr;//初期化

bool FadeManager::Start()
{
	//フェードの初期化
	m_fade.Init(m_fadeFilePath.c_str(), FRAME_BUFFER_W, FRAME_BUFFER_H);
	//フェードの更新処理
	m_fade.Update();

	//現在のフェードの透明度の設定
	m_currentFadeAlpha = m_fade.GetMulColor().a;
	return true;
}

void FadeManager::Update()
{
	switch (m_fadeState)
	{
	case enFadeState_FadeIn://透明にする
		m_currentFadeAlpha -= 2.0f * g_gameTime->GetFrameDeltaTime();
		if (m_currentFadeAlpha <= 0.0f)
		{
			m_currentFadeAlpha = 0.0f;
			m_isFinishFade = true;
			m_fadeState = enFadeState_None;
		}
		break;
	case enFadeState_FadeOut://不透明にする
		m_currentFadeAlpha += 2.0f * g_gameTime->GetFrameDeltaTime();
		if (m_currentFadeAlpha >= 1.0f)
		{
			m_currentFadeAlpha = 1.0f;
			m_isFinishFade = true;
			m_fadeState = enFadeState_None;
		}
		break;
	case enFadeState_None: //何もしない
		m_isFinishFade = false;
		break;
	}

	m_fade.SetMulColor({ 1.0f, 1.0f, 1.0f, m_currentFadeAlpha });
	m_fade.Update();
}

void FadeManager::Render(RenderContext& rc)
{
	//フェードの描画
	m_fade.Draw(rc);
}

