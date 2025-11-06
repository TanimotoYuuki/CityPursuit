#include "stdafx.h"
#include "GameTimeLimit.h"

namespace {
	//制限時間UI
	const float TIME_LIMIT_UI_WIDTH = 1024.0f;//制限時間UIの横幅

	const float TIME_LIMIT_UI_HEIGHT = 128.0f;//制限時間UIの縦幅

	const Vector3 TIME_LIMIT_UI_INIT_POSITION{ 0.0f,350.0f,0.0f };//制限時間UIの初期位置

	const Vector3 TIME_LIMIT_UI_SCALE{ 0.7f,0.7f,1.0f };//制限時間UIの大きさ

	const float TIME_LIMIT_UI_INTERVAL_POSITION = 60.0f;//制限時間UIの間隔位置

	const Vector4 TIME_LIMIT_UI_NORMAL_MULCOLOR{ 1.0f,1.0f,1.0f,1.0f };//制限時間UIの乗算カラー

	const Vector4 TIME_LIMIT_UI_HALF_TRANSPARENT_MULCOLOR{ 1.0f,1.0f,1.0f,0.5f };//制限時間UIを半透明にするための乗算カラー
}

//開始処理
bool GameTimeLimit::Start()
{
	//制限時間UIの初期化
	for (int i = 0; i < enTime_Num; i++)
	{
		for (int j = 0; j < enTimeLimitDisplayUI_Num; j++)
		{
			InitTimeLimitUI((EnTime)i, (EnTimeLimitDisplayUI)j);
		}
	}

	m_timeLimitManage.reserve(6);


	//制限時間UIの更新処理
	TimeLimitUIUpdate();

	return true;
}

//制限時間の実行処理
void GameTimeLimit::Execute()
{
	if (m_isTimeStop)
	{
		for (int i = 0; i < m_timeLimitManage.size(); i++)
		{
			//QTEイベントで成功した回数を表示するUIの乗算カラーの設定
			m_timeLimitUI[i][m_timeLimitManage[i]].SetMulColor(TIME_LIMIT_UI_HALF_TRANSPARENT_MULCOLOR);
			//QTEイベントで成功した回数を表示するUIの更新処理
			m_timeLimitUI[i][m_timeLimitManage[i]].Update();
		}
		return;
	}

	//毎フレーム制限時間を管理する変数で格納しているデータを削除する
	m_timeLimitManage.clear();

	m_timeLimit -= g_gameTime->GetFrameDeltaTime();

	if (m_timeLimit < 0.0f)
	{
		m_timeLimit = 0.0f;
		m_isTimeUp = true;
	}

	//制限時間UIの更新処理
	TimeLimitUIUpdate();
}

//描画処理
void GameTimeLimit::Render(RenderContext& rc)
{
	//UIを描画しないなら処理しない
	if (!m_isDrawingUI)
	{
		return;
	}

	for (int i = 0; i < m_timeLimitManage.size(); i++)
	{
		//QTEイベントで成功した回数を表示するUIの描画
		m_timeLimitUI[i][m_timeLimitManage[i]].Draw(rc);
	}
}

//制限時間UIの初期化
void GameTimeLimit::InitTimeLimitUI(EnTime enTime, EnTimeLimitDisplayUI enTimeLimitDisplayUI)
{
	//制限時間UIの初期化
	m_timeLimitUI[enTime][enTimeLimitDisplayUI].Init(m_timeLimitDisplayUIFilePath[enTimeLimitDisplayUI].c_str(), TIME_LIMIT_UI_WIDTH, TIME_LIMIT_UI_HEIGHT);
	//制限時間UIの座標の設定
	m_timeLimitUI[enTime][enTimeLimitDisplayUI].SetPosition(TIME_LIMIT_UI_INIT_POSITION);
	//制限時間UIの大きさの設定
	m_timeLimitUI[enTime][enTimeLimitDisplayUI].SetScale(TIME_LIMIT_UI_SCALE);
	//制限時間UIの更新処理
	m_timeLimitUI[enTime][enTimeLimitDisplayUI].Update();
}

//制限時間UIの更新処理
void GameTimeLimit::TimeLimitUIUpdate()
{
	//制限時間UIを表示するための計算
	CalcDrawingTimeLimitUI();

	for (int i = 0; i < m_timeLimitManage.size(); i++)
	{
		//リザルトで描画するUIの位置の更新処理
		TimeLimitUIPositionUpdate(m_timeLimitManage, i, m_timeLimitUI[(EnTime)i][m_timeLimitManage[i]]);
	}
}

//制限時間UIを表示するための計算
void GameTimeLimit::CalcDrawingTimeLimitUI()
{
	//残り時間の〇分の部分を求める
	int minute = m_timeLimit / 60;

	//残り時間の〇秒の部分を求める
	int second = (int)m_timeLimit % 60;

	//○秒の十の位に変換
	int secondTen = second / 10;

	//〇秒の一の位に変換
	float secondOne = second % 10;

	//リザルトのデータの格納
	m_timeLimitManage.push_back((EnTimeLimitDisplayUI)minute);
	m_timeLimitManage.push_back(enTimeLimitDisplayUI_Colon);
	m_timeLimitManage.push_back((EnTimeLimitDisplayUI)secondTen);
	m_timeLimitManage.push_back((EnTimeLimitDisplayUI)secondOne);
}

//リザルトで描画するUIの位置の更新処理
void GameTimeLimit::TimeLimitUIPositionUpdate(std::vector<EnTimeLimitDisplayUI> timeLimitData, int dataNum, SpriteRender& timeLimitUI)
{
	//制限時間データの総数を取得
	const int resultDataNum = timeLimitData.size();

	//UI全体の幅を計算
	const float totalWidth = (resultDataNum - 1) * TIME_LIMIT_UI_INTERVAL_POSITION;

	//最初のUIを配置すべきX座標を計算
	const float startX = -totalWidth / 2.0f;

	//現在の描画するUIのX座標を計算
	const float posX = startX + dataNum * TIME_LIMIT_UI_INTERVAL_POSITION;

	//現在のUIの位置の取得
	Vector3 pos = timeLimitUI.GetPosition();
	pos.x = posX; //計算したX座標を適用

	//制限時間UIの位置の設定
	timeLimitUI.SetPosition(pos);
	//制限時間UIの乗算カラーの設定
	timeLimitUI.SetMulColor(TIME_LIMIT_UI_NORMAL_MULCOLOR);
	//制限時間UIの更新処理
	timeLimitUI.Update();
}

