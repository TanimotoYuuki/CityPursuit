#include "stdafx.h"
#include "Buildings.h"
#include "SwingTarget.h"

namespace {
	//建物モデル
	const char* BUILDING_MODEL_FILE_PATH = "Assets/modelData/building/";//ビルモデルのファイルパス
	const char* TOWER_MODEL_FILE_PATH = "Assets/modelData/tower/";//タワーモデルのファイルパス
	const char* MODEL_FILE_EXTENSION = ".tkm";//モデルのファイル拡張子

	//セグメント
	constexpr UINT BUILDINGS_WIDTH_SEGMENTS = 5;				//幅セグメント
	constexpr UINT BUILDINGS_HEIGHT_SEGMENTS = 10;				//高さセグメント
	constexpr UINT BUILDINGS_LENGTH_SEGMENTS = 5;				//長さセグメント
	constexpr float BUILDINGS_HEIGHT_LOWER_LIMIT = 1000.0f;		//高さの下限
	
}

Buildings::~Buildings()
{
	DeleteGO(m_swingTarget);
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

	//建物のタイプがビルの場合スイングターゲットの初期化をする
	if (m_buildingsType == enBuildingsType_Building)
	{
		//建物クラスのワールド行列の計算
		Matrix worldMatrix = m_buildingsModel[m_buildingsType].GetModel().CalcWorldMatrix(m_position, m_rotation, m_scale);

		//スイングターゲットクラスのインスタンスの生成
		m_swingTarget = NewGO<SwingTarget>(0, "swingtarget");

		//スイングターゲットの初期化
		m_swingTarget->Init(m_buildingsModel[m_buildingsType].GetModel().GetTkmFile());

		// スイングターゲットの座標を計算する
		m_swingTarget->CalcSwingTargetPositions(
			worldMatrix,
			BUILDINGS_WIDTH_SEGMENTS,
			BUILDINGS_HEIGHT_SEGMENTS,
			BUILDINGS_LENGTH_SEGMENTS,
			BUILDINGS_HEIGHT_LOWER_LIMIT
		);
	}

	//モデルの静的オブジェクトの作成
	m_physicsStaticObject.CreateFromModel(m_buildingsModel[m_buildingsType].GetModel(), m_buildingsModel[m_buildingsType].GetModel().GetWorldMatrix());

	////デバッグ用
	////建物のタイプがビルの場合デバッグ用のモデルの初期化をする
	//if (m_buildingsType == enBuildingsType_Building)
	//{
	//	//計算されたスイングターゲットの座標コンテナを取得
	//	const std::vector<Vector3>& swingTargetPositions = m_swingTarget->GetSwingTargetPositions();

	//	//全ての座標を調べる
	//	for (auto& stPos : swingTargetPositions)
	//	{
	//		m_debugModel[m_debugModelNum].Init("Assets/modelData/debug/light.tkm");
	//		m_debugModel[m_debugModelNum].SetPosition(swingTargetPositions[m_debugModelNum]);
	//		m_debugModel[m_debugModelNum].SetScale(Vector3::One * 3.0f);
	//		m_debugModel[m_debugModelNum].Update();
	//		m_debugModelNum++;
	//	}
	//}
	
	return true;
}

//更新処理
void Buildings::Update()
{
	//建物モデルの更新
	m_buildingsModel[m_buildingsType].Update();

	////デバッグ用
	//for (int i = 0; i < m_debugModelNum; i++)
	//{
	//	//デバッグ用のモデルの更新
	//	m_debugModel[i].Update();
	//}
}

//描画処理
void Buildings::Render(RenderContext& rc)
{
	//建物モデルの描画
	m_buildingsModel[m_buildingsType].Draw(rc);

	////デバッグ用
	//for (int i = 0; i < m_debugModelNum; i++)
	//{
	//	//デバッグ用のモデルの描画
	//	m_debugModel[i].Draw(rc);
	//}
}