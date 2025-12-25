#include "stdafx.h"
#include "GameBackGround.h"
#include "Game.h"
#include "GameMission.h"
#include "MiniMap.h"
#include "Enemy.h"

namespace {
	// スカイキューブ
	const EnSkyCubeType SKYCUBE_TYPE = enSkyCubeType_Day;//スカイキューブのタイプ

	const Vector3 SKYCUBE_POSITION(0.0f, -1000.0f, 0.0f);//スカイキューブの座標

	const Vector3 SKYCUBE_SCALE(2000.0f, 2000.0f, 2000.0f);//スカイキューブの大きさ

	const float SKYCUBE_LUMINANCE = 1.0f;//スカイキューブの輝度
}

//デストラクタ
GameBackGround::~GameBackGround()
{
	auto& enemys = FindGOs<Enemy>("enemy");
	for (const auto& enemy : enemys)
	{
		DeleteGO(enemy);
	}
}

//開始処理
bool GameBackGround::Start()
{
	//レベルの読み込み処理
	LoadLevel();

	return true;
}

//更新処理
void GameBackGround::Update()
{

}

//レベルの読み込み処理
void GameBackGround::LoadLevel()
{
	m_level3dRender.Init("Assets/level/stage.tkl", [&](LevelObjectData& objData)
	{
		//オブジェの名前がstreetだったら
		if (objData.EqualObjectName(L"street") == true)
		{
			return true;
		}

		//オブジェの名前がbuilding001だったら
		else if (objData.EqualObjectName(L"building001") == true)
		{
			return true;
		}

		//オブジェの名前がbuilding002だったら
		else if (objData.EqualObjectName(L"building002") == true)
		{
			return true;
		}

		//オブジェの名前がbuilding003だったら
		else if (objData.EqualObjectName(L"building003") == true)
		{
			return true;
		}

		//オブジェの名前がbuilding004だったら
		else if (objData.EqualObjectName(L"building004") == true)
		{
			return true;
		}

		//オブジェの名前がtowerだったら
		else if (objData.EqualObjectName(L"tower") == true)
		{
			return true;
		}

		//オブジェの名前がcarだったら
		else if (objData.EqualObjectName(L"car") == true)
		{
			auto car = NewGO<Enemy>(0, "enemy");

			//レベルのデータに保存されている座標を設定
			car->SetPosition(objData.position);

			//レベルのデータに保存されている回転を設定
			car->SetRotation(objData.rotation);

			//レベルのデータに保存されている大きさを設定
			car->SetScale(objData.scale);

			//ゲーム全体を管理するクラスのポインタの設定
			car->SetGamePtr(m_game);

			//ノルマで捕獲する敵の数の加算
			m_game->GetGameMissionPtr()->AddQuotaCaptureEnemyNum();

			//ミニマップに表示する用の敵クラスのポインタの設定
			m_game->GetMiniMapPtr()->SetEnemyPtr(car);

			return true;
		}

		return false;
	});
}