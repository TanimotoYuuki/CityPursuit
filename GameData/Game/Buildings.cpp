#include "stdafx.h"
#include "Buildings.h"

namespace {
	//建物モデル
	const char* BUILDING_MODEL_FILE_PATH = "Assets/modelData/building/";//ビルモデルのファイルパス
	const char* TOWER_MODEL_FILE_PATH = "Assets/modelData/tower/";//タワーモデルのファイルパス
	const char* MODEL_FILE_EXTENSION = ".tkm";//モデルのファイル拡張子
}

//開始処理
bool Buildings::Start()
{
	switch (m_buildingsType)
	{
	case enBuildingsType_Building:
		m_buildingsModelPath = BUILDING_MODEL_FILE_PATH + m_buildingsModelName + MODEL_FILE_EXTENSION;
		break;
	case enBuildingsType_Tower:
		m_buildingsModelPath = TOWER_MODEL_FILE_PATH + m_buildingsModelName + MODEL_FILE_EXTENSION;
		break;
	default:
		break;
	}

	//建物モデルの初期化
	m_buildingsModel[m_buildingsType].Init(m_buildingsModelPath.c_str());

	//建物モデルの位置設定
	m_buildingsModel[m_buildingsType].SetPosition(m_position);

	//建物モデルの回転設定
	m_buildingsModel[m_buildingsType].SetRotation(m_rotation);

	//建物モデルの大きさ設定
	m_buildingsModel[m_buildingsType].SetScale(m_scale);

	//建物モデルの更新
	m_buildingsModel[m_buildingsType].Update();
	return true;
}

//更新処理
void Buildings::Update()
{
	//建物モデルの更新
	m_buildingsModel[m_buildingsType].Update();
}

//描画処理
void Buildings::Render(RenderContext& rc)
{
	//建物モデルの描画
	m_buildingsModel[m_buildingsType].Draw(rc);
}