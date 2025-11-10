#include "stdafx.h"
#include "GameMission.h"

namespace {
	//敵の捕獲数を表示するUI
	const float CAPTURE_ENEMY_NUM_UI_WIDTH = 1024.0f;//敵の捕獲数を表示するUIの横幅

	const float CAPTURE_ENEMY_NUM_UI_HEIGHT = 128.0f;//敵の捕獲数を表示するUIの縦幅

	const Vector3 CAPTURE_ENEMY_NUM_UI_INIT_POSITION{ -205.0f,350.0f,0.0f };//敵の捕獲数を表示するUIの初期位置

	const Vector3 CAPTURE_ENEMY_NUM_UI_SCALE{ 0.3f,0.3f,1.0f };//敵の捕獲数を表示するUIの大きさ

	const float CAPTURE_ENEMY_NUM_UI_INTERVAL_POSITION = 25.0f;//敵の捕獲数を表示するUIの間隔位置

	//ミッションテキストUI
	const float MISSION_TEXT_UI_WIDTH = 1024.0f;//ミッションテキストUIの横幅

	const float MISSION_TEXT_UI_HEIGHT = 128.0f;//ミッションテキストUIの縦幅

	const Vector3 MISSION_TEXT_UI_INIT_POSITION{ -687.5f,400.0f,0.0f };//ミッションテキストUIの初期位置

	const Vector3 MISSION_TEXT_UI_SCALE{ 0.3f,0.3f,1.0f };//ミッションテキストUIの大きさ

	//ミッション内容テキストUI
	const float MISSION_CONTENT_TEXT_UI_WIDTH = 2048.0f;//ミッション内容テキストUIの横幅

	const float MISSION_CONTENT_TEXT_UI_HEIGHT = 128.0f;//ミッション内容テキストUIの縦幅

	const Vector3 MISSION_CONTENT_TEXT_UI_INIT_POSITION{ -520.0f,350.0f,0.0f };//ミッション内容テキストUIの初期位置

	const Vector3 MISSION_CONTENT_TEXT_UI_SCALE{ 0.3f,0.3f,1.0f };//ミッション内容テキストUIの大きさ

}

bool GameMission::Start()
{
	//敵の捕獲数を表示するUIの初期化
	for (int i = 0; i < enCaptureEnemyNumDrawingOrder_Num; i++)
	{
		for (int j = 0; j < enCaptureEnemyNumDisplayUI_Num; j++)
		{
			InitCaptureEnemyNumUI((EnCaptureEnemyNumDrawingOrder)i, (EnCaptureEnemyNumDisplayUI)j);
		}
	}

	m_captureEnemyNumManage.reserve(6);

	//敵の捕獲数を表示するUIの更新処理
	CaptureEnemyNumUIUpdate();

	//ミッションテキストUIの初期化
	InitMissionTextUI();

	//ミッション内容テキストUIの初期化
	InitMissionContentTextUI();

	return true;
}

//実行処理
void GameMission::Execute()
{
	//現在捕獲している敵の数が変わったら処理する
	if (m_currentCaptureEnemyNum != m_captureEnemyNumManage[0])
	{
		//毎フレーム敵の捕獲数を管理する変数で格納しているデータを削除する
		m_captureEnemyNumManage.clear();

		//敵の捕獲数を表示するUIの更新処理
		CaptureEnemyNumUIUpdate();
	}
}

//描画処理
void GameMission::Render(RenderContext& rc)
{
	//UIを描画しないなら処理しない
	if (!m_isDrawingUI)
	{
		return;
	}

	//制限時間UIの描画
	for (int i = 0; i < m_captureEnemyNumManage.size(); i++)
	{
		m_captureEnemyNumUI[i][m_captureEnemyNumManage[i]].Draw(rc);
	}

	//ミッションテキストUIの描画
	m_missionTextUI.Draw(rc);

	//ミッション内容テキストUIの描画
	m_missionContentTextUI.Draw(rc);
}

//敵の捕獲数を表示するUIの初期化
void GameMission::InitCaptureEnemyNumUI(EnCaptureEnemyNumDrawingOrder order, EnCaptureEnemyNumDisplayUI displayUI)
{
	//敵の捕獲数を表示するUIの初期化
	m_captureEnemyNumUI[order][displayUI].Init(m_captureEnemyNumUIFilePath[displayUI].c_str(), CAPTURE_ENEMY_NUM_UI_WIDTH, CAPTURE_ENEMY_NUM_UI_HEIGHT);
	//敵の捕獲数を表示するUIの座標の設定
	m_captureEnemyNumUI[order][displayUI].SetPosition(CAPTURE_ENEMY_NUM_UI_INIT_POSITION);
	//敵の捕獲数を表示するUIの大きさの設定
	m_captureEnemyNumUI[order][displayUI].SetScale(CAPTURE_ENEMY_NUM_UI_SCALE);
	//敵の捕獲数を表示するUIの更新処理
	m_captureEnemyNumUI[order][displayUI].Update();
}

//ミッションテキストUIの初期化
void GameMission::InitMissionTextUI()
{
	//ミッションテキストUIの初期化
	m_missionTextUI.Init(m_missionTextUIFilePath.c_str(), MISSION_TEXT_UI_WIDTH, MISSION_TEXT_UI_HEIGHT);
	//ミッションテキストUIの座標の設定
	m_missionTextUI.SetPosition(MISSION_TEXT_UI_INIT_POSITION);
	//ミッションテキストUIの大きさの設定
	m_missionTextUI.SetScale(MISSION_TEXT_UI_SCALE);
	//ミッションテキストUIの更新処理
	m_missionTextUI.Update();
}

//ミッション内容テキストUIの初期化
void GameMission::InitMissionContentTextUI()
{
	//ミッション内容テキストUIの初期化
	m_missionContentTextUI.Init(m_missionContextTextUIFilePath.c_str(), MISSION_CONTENT_TEXT_UI_WIDTH, MISSION_CONTENT_TEXT_UI_HEIGHT);
	//ミッション内容テキストUIの座標の設定
	m_missionContentTextUI.SetPosition(MISSION_CONTENT_TEXT_UI_INIT_POSITION);
	//ミッション内容テキストUIの大きさの設定
	m_missionContentTextUI.SetScale(MISSION_CONTENT_TEXT_UI_SCALE);
	//ミッション内容テキストUIの更新処理
	m_missionContentTextUI.Update();
}

//敵の捕獲数を表示するUIの更新処理
void GameMission::CaptureEnemyNumUIUpdate()
{
	//敵の捕獲数のデータの格納
	m_captureEnemyNumManage.push_back((EnCaptureEnemyNumDisplayUI)m_currentCaptureEnemyNum);
	m_captureEnemyNumManage.push_back(enCaptureEnemyNumDisplayUI_Slash);
	m_captureEnemyNumManage.push_back((EnCaptureEnemyNumDisplayUI)m_quotaCaptureEnemyNum);

	for (int i = 0; i < m_captureEnemyNumManage.size(); i++)
	{
		//敵の捕獲数を表示するUIの位置の更新処理
		CaptureEnemyNumUIPositionUpdate(m_captureEnemyNumManage, i, m_captureEnemyNumUI[(EnCaptureEnemyNumDrawingOrder)i][m_captureEnemyNumManage[i]]);
	}
}

//敵の捕獲数を表示するUIの位置の更新処理
void GameMission::CaptureEnemyNumUIPositionUpdate(std::vector<EnCaptureEnemyNumDisplayUI> captureEnemyNumData, int dataNum, SpriteRender& captureEnemyNumUI)
{
	//敵の捕獲数のデータの総数を取得
	const int captureEnemyNum = captureEnemyNumData.size();

	//UI全体の幅を計算
	const float totalWidth = (captureEnemyNum - 1) * CAPTURE_ENEMY_NUM_UI_INTERVAL_POSITION;

	//最初のUIを配置すべきX座標を計算
	const float startX = -totalWidth / 2.0f;

	//現在の描画するUIのX座標を計算
	const float posX = startX + dataNum * CAPTURE_ENEMY_NUM_UI_INTERVAL_POSITION;

	//UIの初期位置の取得
	Vector3 pos = CAPTURE_ENEMY_NUM_UI_INIT_POSITION;
	pos.x += posX; //計算したX座標を適用

	//敵の捕獲数を表示するUIの位置の設定
	captureEnemyNumUI.SetPosition(pos);
	//敵の捕獲数を表示するUIの更新処理
	captureEnemyNumUI.Update();
}