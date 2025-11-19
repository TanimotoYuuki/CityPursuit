#include "stdafx.h"
#include "GameResult.h"

namespace {
	//リザルトで表示するUI
	const float RESULT_DISPLAY_UI_WIDTH = 1024.0f;//リザルトで表示するUIの横幅

	const float RESULT_DISPLAY_UI_HEIGHT = 128.0f;//リザルトで表示するUIの縦幅

	const Vector3 RESULT_DISPLAY_UI_SCALE = Vector3(0.25f, 0.25f, 1.0f);//リザルトで表示するUIの大きさ

	const float RESULT_DISPLAY_UI_CENTER_POSITION = 385.0f;//リザルトで表示するUIの中央の位置

	//リザルトUI
	const float RESULT_UI_WIDTH = 1024.0f;//リザルトUIの横幅

	const float RESULT_UI_HEIGHT = 1536.0f;//リザルトUIの縦幅

	const Vector3 RESULT_UI_INIT_POSITION{ 300.0f,-450.0f,0.0f };//リザルトUIの初期位置

	const Vector3 RESULT_UI_SCALE{ 0.45f,0.45f,1.0f };//リザルトUIの大きさ

	const Vector4 RESULT_UI_MULCOLOR{ 1.0f,1.0f,1.0f,0.0f };//リザルトUIの乗算カラー

	//QTEイベントで成功と失敗した回数を表示するUI
	const Vector3 QTE_EVENT_SUCCESS_COUNT_UI_INIT_POSITION{ 0.0f,42.5f,0.0f };//QTEイベントで成功した回数を表示するUIの初期位置 

	const Vector3 QTE_EVENT_FAILED_COUNT_UI_INIT_POSITION{ 0.0f,-80.0f,0.0f };//QTEイベントで失敗した回数を表示するの初期位置

	const float QTE_COUNT_UI_INTERVAL_POSITION = 27.0f;//QTEイベントで成功と失敗した回数を表示するUIの間隔位置

	//ゲームクリアしたときの残り時間を表示するUI
	const Vector3 GAME_CLEAR_TIME_LIMIT_UI_INIT_POSITION{ 0.0f,-202.5f,0.0f };//ゲームクリアしたときの残り時間を表示するUIの初期位置

	const float GAME_CLEAR_TIME_LIMIT_UI_INTERVAL_POSITION = 22.0f;//ゲームクリアしたときの残り時間を表示するUIの間隔位置

	//アニメーション
	const float ANIMATION_PLAY_SPEED = 2.5f;//リザルトUIのアニメーションの再生速度

	const Vector3 RESULT_UI_AFTER_ANIMATION_POSITION{ 300.0f,-25.0f,0.0f };//リザルトUIのアニメーション後の位置

	const float RESULT_UI_AFTER_ANIMATION_ALPHA = 1.0f;//リザルトUIのアニメーション後の透明度

	const float RESULT_ALL_UI_BEFORE_ANIMATION_ALPHA = 1.0f;//リザルトの全てのUIのアニメーション前の透明度

	const float RESULT_ALL_UI_AFTER_ANIMATION_ALPHA = 0.0f;//リザルトの全てのUIのアニメーション後の透明度
}

//開始処理
bool GameResult::Start()
{
	//リザルトUIの初期化
	InitResultUI();

	//リザルトUIでアニメーションさせるユニークポインタの初期化

	//移動
	m_resultUIPositionAnimation = std::make_unique<PositionSpriteAnimation>(
		&m_resultUI, //アニメーションをさせるスプライト
		1.0f, //ターゲットの割合
		ANIMATION_PLAY_SPEED,//アニメーションの再生速度
		false, //ループするか?
		m_resultUI.GetPosition(), //元の位置
		RESULT_UI_AFTER_ANIMATION_POSITION//ターゲットの位置
	);

	//透明度(透明から不透明)
	m_resultUIFullAlphaAnimation = std::make_unique<AlphaSpriteAnimation>(
		&m_resultUI, //アニメーションをさせるスプライト
		1.0f, //ターゲットの割合
		ANIMATION_PLAY_SPEED,//アニメーションの再生速度
		false, //ループするか?
		m_resultUI.GetMulColor().a, //元の透明度
		RESULT_UI_AFTER_ANIMATION_ALPHA//ターゲットの透明度
	);

	//透明(不透明から透明)
	m_resultUINoneAlphaAnimation = std::make_unique<AlphaSpriteAnimation>(
		&m_resultUI, //アニメーションをさせるスプライト
		1.0f, //ターゲットの割合
		ANIMATION_PLAY_SPEED,//アニメーションの再生速度
		false, //ループするか?
		RESULT_ALL_UI_BEFORE_ANIMATION_ALPHA, //元の透明度
		RESULT_ALL_UI_AFTER_ANIMATION_ALPHA//ターゲットの透明度
	);

	//リザルトで描画するUIの準備処理
	PreResultDrawingUI();

	//QTEイベントで失敗した回数を表示するUIの初期化と
	//アニメーションさせるユニークポインタの初期化
	for (int i = 0; i < m_qteEventSuccessCountManage.size(); i++)
	{
		InitQteEventSuccessCountUI((EnPlace)i, m_qteEventSuccessCountManage[i]);//QTEイベントで成功した回数を表示するUIの初期化

		//リザルトで描画するUIの位置の更新処理
		ResultDrawingUIPositionUpdate(m_qteEventSuccessCountManage, i, m_qteEventSuccessCountUI[(EnPlace)i][m_qteEventSuccessCountManage[i]], QTE_COUNT_UI_INTERVAL_POSITION);

		//透明度(不透明から透明)
		m_qteEventSuccessCountUINoneAlphaAnimation[(EnPlace)i][m_qteEventSuccessCountManage[i]] = std::make_unique<AlphaSpriteAnimation>(
			&m_qteEventSuccessCountUI[(EnPlace)i][m_qteEventSuccessCountManage[i]], //アニメーションをさせるスプライト
			1.0f, //ターゲットの割合
			ANIMATION_PLAY_SPEED,//アニメーションの再生速度
			false, //ループするか?
			RESULT_ALL_UI_BEFORE_ANIMATION_ALPHA, //元の透明度
			RESULT_ALL_UI_AFTER_ANIMATION_ALPHA//ターゲットの透明度
		);
	}

	//QTEイベントで成功した回数を表示するUIの初期化と
	//アニメーションさせるユニークポインタの初期化
	for (int j = 0; j < m_qteEventFailedCountManage.size(); j++)
	{
		InitQteEventFailedCountUI((EnPlace)j, m_qteEventFailedCountManage[j]);//QTEイベントで失敗した回数を表示するUIの初期化

		//リザルトで描画するUIの位置の更新処理
		ResultDrawingUIPositionUpdate(m_qteEventFailedCountManage, j, m_qteEventFailedCountUI[(EnPlace)j][m_qteEventFailedCountManage[j]], QTE_COUNT_UI_INTERVAL_POSITION);

		//透明度(不透明から透明)
		m_qteEventFailedCountUINoneAlphaAnimation[(EnPlace)j][m_qteEventFailedCountManage[j]] = std::make_unique<AlphaSpriteAnimation>(
			&m_qteEventFailedCountUI[(EnPlace)j][m_qteEventFailedCountManage[j]], //アニメーションをさせるスプライト
			1.0f, //ターゲットの割合
			ANIMATION_PLAY_SPEED,//アニメーションの再生速度
			false, //ループするか?
			RESULT_ALL_UI_BEFORE_ANIMATION_ALPHA, //元の透明度
			RESULT_ALL_UI_AFTER_ANIMATION_ALPHA//ターゲットの透明度
		);
	}

	//ゲームクリアしたときの残り時間を表示するUIの初期化と
	//アニメーションさせるユニークポインタの初期化
	for (int k = 0; k < m_gameClearTimeLimitManage.size(); k++)
	{
		InitGameClearTimeLimitUI((EnTime)k, m_gameClearTimeLimitManage[k]);//ゲームクリアしたときの残り時間を表示するUIの初期化

		//リザルトで描画するUIの位置の更新処理
		ResultDrawingUIPositionUpdate(m_gameClearTimeLimitManage, k, m_gameClearTimeLimitUI[(EnTime)k][m_gameClearTimeLimitManage[k]], GAME_CLEAR_TIME_LIMIT_UI_INTERVAL_POSITION);

		//透明度(不透明から透明)
		m_gameClearTimeLimitUINoneAlphaAnimation[(EnTime)k][m_gameClearTimeLimitManage[k]] = std::make_unique<AlphaSpriteAnimation>(
			&m_gameClearTimeLimitUI[(EnTime)k][m_gameClearTimeLimitManage[k]], //アニメーションをさせるスプライト
			1.0f, //ターゲットの割合
			ANIMATION_PLAY_SPEED,//アニメーションの再生速度
			false, //ループするか?
			RESULT_ALL_UI_BEFORE_ANIMATION_ALPHA, //元の透明度
			RESULT_ALL_UI_AFTER_ANIMATION_ALPHA//ターゲットの透明度
		);
	}

	return true;
}

//リザルトの実行処理
void GameResult::Execute()
{
	//透明度(不透明から透明)を変えるアニメーションを再生終了していたら処理しない
	if (m_isFinishNoneAlphaAnimation)
	{
		return;
	}

	//透明度(不透明から透明)を変えるアニメーションを再生するなら処理をする
	if (m_isPlayNoneAlphaAnimation)
	{
		//透明度(不透明から透明)を変えるアニメーションを再生
		PlayNoneAlphaAnimation();

		if (m_resultUINoneAlphaAnimation->IsCompleted())
		{
			m_isFinishNoneAlphaAnimation = true;
		}

		return;
	}

	//全てのリザルトの演出が終わっていなかったら
	if (!m_isFinishAllResultDirection)
	{
		//リザルトの演出
		ResultDirection();
	}

	//入力の更新処理
	InputUpdate();
}

//描画処理
void GameResult::Render(RenderContext& rc)
{
	//透明度(不透明から透明)を変えるアニメーションを再生終了していたら描画しない
	if (m_isFinishNoneAlphaAnimation)
	{
		return;
	}

	//リザルトUIの描画
	m_resultUI.Draw(rc);

	if (m_isStartResultUIDirection[enResultDirection_QteSuccessCountUI])
	{
		for (int i = 0; i < m_qteEventSuccessCountManage.size(); i++)
		{
			//QTEイベントで成功した回数を表示するUIの描画
			m_qteEventSuccessCountUI[i][m_qteEventSuccessCountManage[i]].Draw(rc);
		}
	}

	if (m_isStartResultUIDirection[enResultDirection_QteFailedCountUI])
	{
		for (int j = 0; j < m_qteEventFailedCountManage.size(); j++)
		{
			//QTEイベントで失敗した回数を表示するUIの描画
			m_qteEventFailedCountUI[j][m_qteEventFailedCountManage[j]].Draw(rc);
		}
	}

	if (m_isStartResultUIDirection[enResultDirection_GameClearTimeLimitUI])
	{
		for (int k = 0; k < m_gameClearTimeLimitManage.size(); k++)
		{
			//ゲームクリアしたときの残り時間を表示するUIの描画
			m_gameClearTimeLimitUI[k][m_gameClearTimeLimitManage[k]].Draw(rc);
		}
	}
}

//リザルトUIの初期化
void GameResult::InitResultUI()
{
	//リザルトUIの初期化
	m_resultUI.Init(m_resultUIFilePath.c_str(), RESULT_UI_WIDTH, RESULT_UI_HEIGHT);
	//リザルトUIの座標の設定
	m_resultUI.SetPosition(RESULT_UI_INIT_POSITION);
	//リザルトUIの大きさの設定
	m_resultUI.SetScale(RESULT_UI_SCALE);
	//リザルトUIの乗算カラーの設定
	m_resultUI.SetMulColor(RESULT_UI_MULCOLOR);
	//リザルトUIの更新処理
	m_resultUI.Update();
}

//QTEイベントで成功した回数を表示するUIの初期化
void GameResult::InitQteEventSuccessCountUI(EnPlace enPlace, EnResultDisplayUI enResultDisplayUI)
{
	//QTEイベントで成功した回数を表示するUIの初期化
	m_qteEventSuccessCountUI[enPlace][enResultDisplayUI].Init(m_resultDisplayUIFilePath[enResultDisplayUI].c_str(), RESULT_DISPLAY_UI_WIDTH, RESULT_DISPLAY_UI_HEIGHT);
	//QTEイベントで成功した回数を表示するUIの座標の設定
	m_qteEventSuccessCountUI[enPlace][enResultDisplayUI].SetPosition(QTE_EVENT_SUCCESS_COUNT_UI_INIT_POSITION);
	//QTEイベントで成功した回数を表示するUIの大きさの設定
	m_qteEventSuccessCountUI[enPlace][enResultDisplayUI].SetScale(RESULT_DISPLAY_UI_SCALE);
	//QTEイベントで成功した回数を表示するUIの更新処理
	m_qteEventSuccessCountUI[enPlace][enResultDisplayUI].Update();
}

//QTEイベントで失敗した回数を表示するUIの初期化
void GameResult::InitQteEventFailedCountUI(EnPlace enPlace, EnResultDisplayUI enResultDisplayUI)
{
	//QTEイベントで失敗した回数を表示するUIの初期化
	m_qteEventFailedCountUI[enPlace][enResultDisplayUI].Init(m_resultDisplayUIFilePath[enResultDisplayUI].c_str(), RESULT_DISPLAY_UI_WIDTH, RESULT_DISPLAY_UI_HEIGHT);
	//QTEイベントで失敗した回数を表示するUIの座標の設定
	m_qteEventFailedCountUI[enPlace][enResultDisplayUI].SetPosition(QTE_EVENT_FAILED_COUNT_UI_INIT_POSITION);
	//QTEイベントで失敗した回数を表示するUIの大きさの設定
	m_qteEventFailedCountUI[enPlace][enResultDisplayUI].SetScale(RESULT_DISPLAY_UI_SCALE);
	//QTEイベントで失敗した回数を表示するUIの更新処理
	m_qteEventFailedCountUI[enPlace][enResultDisplayUI].Update();
}

//ゲームクリアしたときの残り時間を表示するUIの初期化
void GameResult::InitGameClearTimeLimitUI(EnTime enTime, EnResultDisplayUI enResultDisplayUI)
{
	//ゲームクリアしたときの残り時間を表示するUIの初期化
	m_gameClearTimeLimitUI[enTime][enResultDisplayUI].Init(m_resultDisplayUIFilePath[enResultDisplayUI].c_str(), RESULT_DISPLAY_UI_WIDTH, RESULT_DISPLAY_UI_HEIGHT);
	//ゲームクリアしたときの残り時間を表示するUIの座標の設定
	m_gameClearTimeLimitUI[enTime][enResultDisplayUI].SetPosition(GAME_CLEAR_TIME_LIMIT_UI_INIT_POSITION);
	//ゲームクリアしたときの残り時間を表示するUIの大きさの設定
	m_gameClearTimeLimitUI[enTime][enResultDisplayUI].SetScale(RESULT_DISPLAY_UI_SCALE);
	//ゲームクリアしたときの残り時間を表示するUIの更新処理
	m_gameClearTimeLimitUI[enTime][enResultDisplayUI].Update();
}

//リザルトで描画するUIの準備処理
void GameResult::PreResultDrawingUI()
{
	m_qteEventSuccessCountManage.reserve(6);
	m_qteEventFailedCountManage.reserve(6);
	m_gameClearTimeLimitManage.reserve(6);

	PreQteEventResultUI();//リザルトでQTEイベントの成功と失敗の回数を表示するUIの準備処理

	PreGameClearTimeLimitUI();//リザルトでクリアしたときの残り時間を表示するUIの準備処理
}

//リザルトでQTEイベントの成功と失敗の回数を表示するUIの準備処理
void GameResult::PreQteEventResultUI()
{
	//QTEイベントで成功と失敗の回数のUIを表示するための計算
	CalcDrawingQteEventResultUI(m_qteEventSuccessCount, m_qteEventSuccessCountManage);

	//QTEイベントで成功と失敗の回数のUIを表示するための計算
	CalcDrawingQteEventResultUI(m_qteEventFaileCount, m_qteEventFailedCountManage);
}

//リザルトでクリアしたときの残り時間を表示するUIの準備処理
void GameResult::PreGameClearTimeLimitUI()
{
	//ゲームクリアしたときの残り時間UIを表示するための計算
	CalcDrawingGameClearTimeLimitUI();
}

//QTEイベントで成功と失敗の回数のUIを表示するための計算
void GameResult::CalcDrawingQteEventResultUI(int resultData, std::vector<EnResultDisplayUI>& resultDataManage)
{
	int result = resultData;

	//十の位を求める
	int placeTen = result / 10;

	//一の位を求める
	float placeOne = result % 10;

	//結果が10以上なら十の位のUIも表示する
	if (result >= 10) {
		resultDataManage.push_back((EnResultDisplayUI)placeTen);
	}

	resultDataManage.push_back((EnResultDisplayUI)placeOne);
	resultDataManage.push_back(enResultDisplayUI_Count);
}

//ゲームクリアしたときの残り時間UIを表示するための計算
void GameResult::CalcDrawingGameClearTimeLimitUI()
{
	//残り時間の〇分の部分を求める
	int minute = m_gameClearTimeLimit / 60;

	//残り時間の〇秒の部分を求める
	int second = (int)m_gameClearTimeLimit % 60;

	//○秒の十の位に変換
	int secondTen = second / 10;

	//〇秒の一の位に変換
	int secondOne = second % 10;

	//リザルトのデータの格納
	m_gameClearTimeLimitManage.push_back((EnResultDisplayUI)minute);
	m_gameClearTimeLimitManage.push_back(enResultDisplayUI_Colon);
	m_gameClearTimeLimitManage.push_back((EnResultDisplayUI)secondTen);
	m_gameClearTimeLimitManage.push_back((EnResultDisplayUI)secondOne);
}

//リザルトで描画するUIの位置の更新処理
void GameResult::ResultDrawingUIPositionUpdate(std::vector<EnResultDisplayUI> resultData, int dataNum, SpriteRender& resultUI, float intervalPosition)
{
	//リザルトデータの総数を取得
	const int resultDataNum = resultData.size();

	//UI全体の幅を計算
	const float totalWidth = (resultDataNum - 1) * intervalPosition;

	//最初のUIを配置すべきX座標を計算
	const float startX = -totalWidth / 2.0f;

	//現在の描画するUIのX座標を計算
	const float posX = (startX + (dataNum * intervalPosition)) + RESULT_DISPLAY_UI_CENTER_POSITION;

	//現在のUIの位置の取得
	Vector3 pos = resultUI.GetPosition();
	pos.x = posX; //計算したX座標を適用

	//リザルトで描画するUIの位置の設定
	resultUI.SetPosition(pos);
	//リザルトで描画するUIの更新処理
	resultUI.Update();
}

//入力の更新処理
void GameResult::InputUpdate()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		//全てのリザルトの演出が終わっていなかったら
		if (!m_isFinishAllResultDirection)
		{
			//演出のスキップ処理
			DirectionSkip();
			return;
		}
		//透明度(不透明から透明)を変えるアニメーションを再生させる
		m_isPlayNoneAlphaAnimation = true;
	}
}

//リザルトの演出
void GameResult::ResultDirection()
{
	//リザルトUIの演出処理
	switch (m_resultDirectionState)
	{
	case GameResult::enResultDirection_ResultUI://リザルトUIの演出
		ResultUIDirection();
		break;
	case GameResult::enResultDirection_QteSuccessCountUI://QTEイベントで成功した回数のUIの演出
		QteSuccessCountUIDirection();
		break;
	case GameResult::enResultDirection_QteFailedCountUI://QTEイベントで失敗した回数のUIの演出
		QteFailedCountUIDirection();
		break;
	case GameResult::enResultDirection_GameClearTimeLimitUI://ゲームクリアしたときの残り時間のUIの演出
		GameClearTimeLimitUIDirection();
		break;
	default:
		break;
	}
}

//演出のスキップ処理
void GameResult::DirectionSkip()
{
	//現在のリザルトUIの演出
	switch (m_resultDirectionState)
	{
	case GameResult::enResultDirection_QteSuccessCountUI://QTEイベントで成功した回数のUIの演出
		m_isStartResultUIDirection[enResultDirection_QteFailedCountUI] = true;
		m_resultDirectionState = enResultDirection_QteFailedCountUI;
		break;
	case GameResult::enResultDirection_QteFailedCountUI://QTEイベントで失敗した回数のUIの演出
		m_isStartResultUIDirection[enResultDirection_GameClearTimeLimitUI] = true;
		m_resultDirectionState = enResultDirection_GameClearTimeLimitUI;
		break;
	case GameResult::enResultDirection_GameClearTimeLimitUI://ゲームクリアしたときの残り時間のUIの演出
		m_isFinishAllResultDirection = true;
		break;
	default:
		break;
	}
}

//リザルトUIの演出
void GameResult::ResultUIDirection()
{
	m_resultUIPositionAnimation->Update();
	m_resultUIFullAlphaAnimation->Update();
	if (m_resultUIPositionAnimation->IsCompleted())
	{
		if (g_gameTime->StopWatch(0.5f))
		{
			m_isStartResultUIDirection[enResultDirection_QteSuccessCountUI] = true;
			m_resultDirectionState = enResultDirection_QteSuccessCountUI;
		}
	}
}

//QTEイベントで成功した回数のUIの演出
void GameResult::QteSuccessCountUIDirection()
{
	if (g_gameTime->StopWatch(1.0f))
	{
		m_isStartResultUIDirection[enResultDirection_QteFailedCountUI] = true;
		m_resultDirectionState = enResultDirection_QteFailedCountUI;
	}
}

//QTEイベントで失敗した回数のUIの演出
void GameResult::QteFailedCountUIDirection()
{
	if (g_gameTime->StopWatch(1.0f))
	{
		m_isStartResultUIDirection[enResultDirection_GameClearTimeLimitUI] = true;
		m_resultDirectionState = enResultDirection_GameClearTimeLimitUI;
	}
}

//ゲームクリアしたときの残り時間のUIの演出
void GameResult::GameClearTimeLimitUIDirection()
{
	if (g_gameTime->StopWatch(1.0f))
	{
		m_isFinishAllResultDirection = true;
	}
}

//透明度(不透明から透明)を変えるアニメーションを再生
void GameResult::PlayNoneAlphaAnimation()
{
	m_resultUINoneAlphaAnimation->Update();

	if (m_isStartResultUIDirection[enResultDirection_QteSuccessCountUI])
	{
		for (int i = 0; i < m_qteEventSuccessCountManage.size(); i++)
		{
			m_qteEventSuccessCountUINoneAlphaAnimation[i][m_qteEventSuccessCountManage[i]]->Update();
		}
	}

	if (m_isStartResultUIDirection[enResultDirection_QteFailedCountUI])
	{
		for (int j = 0; j < m_qteEventFailedCountManage.size(); j++)
		{
			m_qteEventFailedCountUINoneAlphaAnimation[j][m_qteEventFailedCountManage[j]]->Update();
		}
	}

	if (m_isStartResultUIDirection[enResultDirection_GameClearTimeLimitUI])
	{
		for (int k = 0; k < m_gameClearTimeLimitManage.size(); k++)
		{
			m_gameClearTimeLimitUINoneAlphaAnimation[k][m_gameClearTimeLimitManage[k]]->Update();
		}
	}
}