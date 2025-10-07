#include "stdafx.h"
#include "DebugLog.h"

namespace
{
	const int DEBUG_LOG_DRAWING__MAX = 100;//デバッグログを描画の上限
}

bool DebugLog::Start()
{
	m_debugLogName.reserve(100);
	return true;
}

void DebugLog::Update()
{
	//デバッグログの更新処理
	DebugLogUpdate();
}

//デバッグログの更新処理
void DebugLog::DebugLogUpdate()
{
	//デバッグログの名前に一致しているデータが合ったらデータを格納している変数の型に応じて処理する
	for (int i = 0; i < m_debugLogName.size(); i++)
	{
		auto intDataIt = m_intDebugLogData.find(m_debugLogName[i].c_str());
		if (intDataIt != m_intDebugLogData.end())
		{
			wchar_t intDebug[256];
			swprintf_s(intDebug, 256, L"I:%d", int(intDataIt->second));

			m_debugLog[i].SetText(intDebug);
			m_debugLog[i].SetPosition(Vector3{ m_DebugLogPosition.x,m_DebugLogPosition.y - i * 50,m_DebugLogPosition.z });
			m_debugLog[i].SetScale(1.0f);
			m_debugLog[i].SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		}

		auto floatDataIt = m_floatDebugLogData.find(m_debugLogName[i].c_str());
		if (floatDataIt != m_floatDebugLogData.end())
		{
			wchar_t floatDebug[256];
			swprintf_s(floatDebug, 256, L"F:%0.2f", float(floatDataIt->second));

			m_debugLog[i].SetText(floatDebug);
			m_debugLog[i].SetPosition(Vector3{ m_DebugLogPosition.x,m_DebugLogPosition.y - i + 50,m_DebugLogPosition.z });
			m_debugLog[i].SetScale(1.0f);
			m_debugLog[i].SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		}

		auto vector3DataIt = m_vector3DebugLogData.find(m_debugLogName[i].c_str());
		if (vector3DataIt != m_vector3DebugLogData.end())
		{
			wchar_t vector3Debug[256];
			swprintf_s(vector3Debug, 256, L"X:%0.2f Y:%0.2f Z:%0.2f", float(vector3DataIt->second.x), float(vector3DataIt->second.y), float(vector3DataIt->second.z));

			m_debugLog[i].SetText(vector3Debug);
			m_debugLog[i].SetPosition(Vector3{ m_DebugLogPosition.x,m_DebugLogPosition.y - i * 50,m_DebugLogPosition.z });
			m_debugLog[i].SetScale(1.0f);
			m_debugLog[i].SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		}

	}
}

//描画処理
void DebugLog::Render(RenderContext& rc)
{
	//デバッグログの描画処理
	for (int i = 0; i < m_debugLogName.size(); i++)
	{
		m_debugLog[i].Draw(rc);
	}
}