#include "stdafx.h"
#include "BackGround.h"
#include "Street.h"
#include "Buildings.h"

namespace {
	// スカイキューブ
	const EnSkyCubeType SKYCUBE_TYPE = enSkyCubeType_Day;//スカイキューブのタイプ

	const Vector3 SKYCUBE_POSITION(0.0f, -1000.0f, 0.0f);//スカイキューブの座標

	const Vector3 SKYCUBE_SCALE(2000.0f, 2000.0f, 2000.0f);//スカイキューブの大きさ

	const float SKYCUBE_LUMINANCE = 1.0f;//スカイキューブの輝度
}

//開始処理
bool BackGround::Start()
{
	//スカイキューブの初期化
	InitSkyCube();

	//レベルの読み込み処理
	LoadLevel();

	return true;
}

//更新処理
void BackGround::Update()
{

}

//スカイキューブの初期化
void BackGround::InitSkyCube()
{
	DeleteGO(m_skyCube);
	//スカイキューブの生成
	m_skyCube = NewGO<SkyCube>(0, "skycube");

	//スカイキューブの初期化
	m_skyCube->SetLuminance(SKYCUBE_LUMINANCE);

	//スカイキューブのテクスチャを設定
	m_skyCube->SetType(SKYCUBE_TYPE);

	//スカイキューブの座標の設定
	m_skyCube->SetPosition(SKYCUBE_POSITION);

	//スカイキューブの大きさの設定
	m_skyCube->SetScale(SKYCUBE_SCALE);

	//スカイキューブの更新
	m_skyCube->Update();
}

//レベルの読み込み処理
void BackGround::LoadLevel()
{
	m_level3dRender.Init("Assets/level/stage.tkl", [&](LevelObjectData& objData)
	{
		//オブジェの名前がstreetだったら
		if (objData.EqualObjectName(L"street") == true)
		{
			//道クラスのインスタンスの生成
			auto street = NewGO<Street>(0, "street");

			//レベルのデータに保存されている座標を設定
			street->SetPosition(objData.position);

			//レベルのデータに保存されている回転を設定
			street->SetRotation(objData.rotation);

			//レベルのデータに保存されている大きさを設定
			street->SetScale(objData.scale);

			return true;
		}

		//オブジェの名前がbuilding001だったら
		else if (objData.EqualObjectName(L"building001") == true)
		{
			//建物クラスのインスタンスの生成
			auto buildings = NewGO<Buildings>(0, "buildings");

			//建物のタイプを設定
			buildings->SetBuildingModelType(buildings->enBuildingsType_Building);

			//建物モデルの名前を設定
			buildings->SetBuildingsModelName("building001");

			//レベルのデータに保存されている座標を設定
			buildings->SetPosition(objData.position);

			//レベルのデータに保存されている回転を設定
			buildings->SetRotation(objData.rotation);

			//レベルのデータに保存されている大きさを設定
			buildings->SetScale(objData.scale);

			return true;
		}

		//オブジェの名前がbuilding002だったら
		else if (objData.EqualObjectName(L"building002") == true)
		{
			auto buildings = NewGO<Buildings>(0, "buildings");

			//建物のタイプを設定
			buildings->SetBuildingModelType(buildings->enBuildingsType_Building);

			//建物モデルの名前を設定
			buildings->SetBuildingsModelName("building002");

			//レベルのデータに保存されている座標を設定
			buildings->SetPosition(objData.position);

			//レベルのデータに保存されている回転を設定
			buildings->SetRotation(objData.rotation);

			//レベルのデータに保存されている大きさを設定
			buildings->SetScale(objData.scale);

			return true;
		}

		//オブジェの名前がbuilding003だったら
		else if (objData.EqualObjectName(L"building003") == true)
		{
			auto buildings = NewGO<Buildings>(0, "buildings");

			//建物のタイプを設定
			buildings->SetBuildingModelType(buildings->enBuildingsType_Building);

			//建物モデルの名前を設定
			buildings->SetBuildingsModelName("building003");

			//レベルのデータに保存されている座標を設定
			buildings->SetPosition(objData.position);

			//レベルのデータに保存されている回転を設定
			buildings->SetRotation(objData.rotation);

			//レベルのデータに保存されている大きさを設定
			buildings->SetScale(objData.scale);

			return true;
		}

		//オブジェの名前がbuilding004だったら
		else if (objData.EqualObjectName(L"building004") == true)
		{
			auto buildings = NewGO<Buildings>(0, "buildings");

			//建物のタイプを設定
			buildings->SetBuildingModelType(buildings->enBuildingsType_Building);

			//建物モデルの名前を設定
			buildings->SetBuildingsModelName("building004");

			//レベルのデータに保存されている座標を設定
			buildings->SetPosition(objData.position);

			//レベルのデータに保存されている回転を設定
			buildings->SetRotation(objData.rotation);

			//レベルのデータに保存されている大きさを設定
			buildings->SetScale(objData.scale);

			return true;
		}

		//オブジェの名前がtowerだったら
		else if (objData.EqualObjectName(L"tower") == true)
		{
			auto buildings = NewGO<Buildings>(0, "buildings");

			//建物のタイプを設定
			buildings->SetBuildingModelType(buildings->enBuildingsType_Tower);

			//建物モデルの名前を設定
			buildings->SetBuildingsModelName("tower");

			//レベルのデータに保存されている座標を設定
			buildings->SetPosition(objData.position);

			//レベルのデータに保存されている回転を設定
			buildings->SetRotation(objData.rotation);

			//レベルのデータに保存されている大きさを設定
			buildings->SetScale(objData.scale);

			return true;
		}

		return false;
	});
}