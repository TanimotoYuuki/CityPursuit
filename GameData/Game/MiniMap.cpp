#include "stdafx.h"
#include "MiniMap.h"
#include "Player.h"
#include "Enemy.h"

namespace{
	const Vector3 MAP_CENTER_POSITION = Vector3(534.0f, -224.0f, 0.0f);
	const Vector3 MAP_SCALE = Vector3{0.5f,0.5f,0.5f};

	const float MAP_RADIUS = 120.0f;
	const float LIMITED_RANGE_IMAGE = 10000.0f;

	const int ENEMY_MAX = 3;
}

//開始処理
bool MiniMap::Start()
{
	m_miniMapUI.Init("Assets/sprite/miniMap/map.dds", 512, 512);
	m_miniMapUI.SetPosition(MAP_CENTER_POSITION);
	m_miniMapUI.SetScale(MAP_SCALE);

	m_currentPlayerUI.Init("Assets/sprite/miniMap/player.dds", 50, 50);
	m_currentPlayerUI.SetPosition(MAP_CENTER_POSITION);
	m_currentPlayerUI.SetScale(MAP_SCALE);

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		m_currentEnemyUI[i].Init("Assets/sprite/miniMap/enemy.dds", 35, 35);
		m_currentEnemyUI[i].SetScale(MAP_SCALE);
	}

	m_enemy.reserve(ENEMY_MAX);
	return true;
}

//実行処理
void MiniMap::Execute()
{
	Vector3 playerPosition = m_player->GetModelData().GetPosition();
	Vector3 mapPosition;

	for (int i = 0; i < m_enemy.size(); i++)
	{
		Vector3 enemyPosition = m_enemy[i]->GetPosition();

		//ワールド座標系からマップ座標系に変換
		WorldPositionConvertToMapPosition(playerPosition, enemyPosition, mapPosition);
		
		//敵の現在地UIに座標の設定
		m_currentEnemyUI[i].SetPosition(mapPosition);
		m_currentEnemyUI[i].Update();
	}

	m_miniMapUI.Update();
	m_currentPlayerUI.Update();
}

//ワールド座標系からマップ座標系に変換
void MiniMap::WorldPositionConvertToMapPosition(Vector3 worldCenterPosition, Vector3 worldPosition, Vector3& mapPosition)
{
	//Y座標はマップの座標とは関係ないので、0にする
	worldCenterPosition.y = 0.0f;
	worldPosition.y = 0.0f;
	Vector3 diff = worldPosition - worldCenterPosition;

	//マップを中心とするオブジェクトとの距離が一定以上離れていたら
	if (diff.LengthSq() >= LIMITED_RANGE_IMAGE * LIMITED_RANGE_IMAGE)
	{
		//これ以上Z軸方向に離れないようにする
		diff.Normalize();
		diff *= LIMITED_RANGE_IMAGE;
	}

	float length = diff.Length();

	Vector3 forward = g_camera3D->GetForward();
	Quaternion rot;
	rot.SetRotationY(atan2(-forward.x, forward.z));

	//ベクトルにカメラの回転を適用
	rot.Apply(diff);

	//ベクトルを正規化
	diff.Normalize();

	//マップの大きさ/距離制限で。
	//ベクトルをマップ座標系に変換する。
	diff *= length * MAP_RADIUS / LIMITED_RANGE_IMAGE;

	//マップの中央座標と上記ベクトルを加算する。
	mapPosition = Vector3(MAP_CENTER_POSITION.x + diff.x, MAP_CENTER_POSITION.y + diff.z, 0.0f);
}

//描画処理
void MiniMap::Render(RenderContext& rc)
{
	//UIを描画しないなら処理しない
	if (!m_isDrawingUI)
	{
		return;
	}

	m_miniMapUI.Draw(rc);
	m_currentPlayerUI.Draw(rc);

	for (int i = 0; i < m_enemy.size(); i++)
	{
		m_currentEnemyUI[i].Draw(rc);
	}
}
