#include "stdafx.h"
#include "QteEvent.h"
#include "QteEventInput.h"
#include "Game.h"
#include "GameMission.h"
#include "MiniMap.h"
#include "Player.h"
#include "PlayerCatchEnemy.h"
#include "PlayerAnimation.h"
#include "Enemy.h"
#include "EnemyAI.h"
#include "EnemyEffect.h"

namespace {
	//ゲームパッドUI
	const float GAMEPAD_UI_WIDTH_AND_HEIGHT = 1024;//ゲームパッドUIの横幅と縦幅

	const Vector3 GAMEPAD_INPUT_BEFORE_UI_INIT_POSITION{ 0.0f,0.0f,0.0f };//ゲームパッドのボタンや方向キーを入力前のUIの初期位置

	const Vector3 GAMEPAD_INPUT_AFTER_UI_INIT_POSITION{ 0.0f,-25.0f,0.0f };//ゲームパッドのボタンや方向キーを入力後のUIの初期位置

	const Vector3 GAMEPAD_UI_SCALE{ 0.1f,0.1f,0.1f };//ゲームパッドUIの大きさ

	const float GAMEPAD_UI_INTERVAL_POSITION = 125.0f;//ゲームパッドUIの間隔位置

	//制限時間UI
	const float TIME_LIMIT_UI_WIDTH = 1024;//制限時間UIの横幅

	const float TIME_LIMIT_UI_HEIGHT = 128;//制限時間UIの縦幅

	const Vector3 TIME_LIMIT_UI_INIT_POSITION{ -255.0f,250.0f,0.0f };//制限時間UIの初期位置

	const Vector3 TIME_LIMIT_BER_UI_INIT_POSITION{ 0.0f,250.0f,0.0f };//制限時間バーUIの初期位置

	const Vector3 TIME_LIMIT_UI_SCALE{ 0.4956f,0.45f,0.45f };//制限時間UIの大きさ

	const Vector3 TIME_LIMIT_BER_UI_SCALE{ 0.5f,0.5f,0.5f };//制限時間バーUIの大きさ

	const Vector2 TIME_LIMIT_UI_PIVOT{ 0.0f,0.5f };//制限時間UIのピボット

	const Vector4 TIME_LIMIT_UI_MULCOLOR_WHITE{ 1.0f,1.0f,1.0f,1.0f };//制限時間UIの乗算カラー(白色)

	const Vector4 TIME_LIMIT_UI_MULCOLOR_YELLOW{ 1.0f,1.0f,0.0f,1.0f };//制限時間UIの乗算カラー(黄色)

	const Vector4 TIME_LIMIT_UI_MULCOLOR_RED{ 1.0f,0.0f,0.0f,1.0f };//制限時間UIの乗算カラー(赤色)

	//QTEイベントの結果UI
	const float QTE_EVENT_RESULT_UI_WIDTH = 1024;//QTEイベントの結果UIの横幅

	const float QTE_EVENT_RESULT_UI_HEIGHT = 128;//QTEイベントの結果UIの縦幅

	const Vector3 QTE_EVENT_RESULT_UI_INIT_POSITION{ -150.0f,100.0f,0.0f };//QTEイベントの結果UIの初期位置

	const Vector3 QTE_EVENT_RESULT_UI_SCALE{ 0.5f,0.5f,0.5f };//QTEイベントの結果UIの大きさ

	const Vector4 QTE_EVENT_RESULT_UI_MULCOLOR{ 1.0f,1.0f,1.0f,0.0f };//QTEイベントの結果の乗算カラー

	//時間
	const float DELAY_TIME = 1.0f;//待機時間

	//ボタン
	const int A_BUTTON_SELECT_ID = 0;//AボタンID

	const int B_BUTTON_SELECT_ID = 1;//BボタンID

	const int X_BUTTON_SELECT_ID = 2;//XボタンID

	const int Y_BUTTON_SELECT_ID = 3;//YボタンID

	//QTEイベント
	const int SUCCESS_INPUT_COMMAND_COUNT_MAX = 3;//QTEイベントでコマンド入力成功数の上限

	//スプライトのアニメーション
	const Vector3 AFTER_START_DIRECTION_ANIMATION_POSITION{ 0.0f,100.0f,0.0f };//QTEイベントの結果UIのアニメーション後の座標(開始演出)

	const Vector3 QTE_RESULT_SUCCESS_AFTER_END_DIRECTION_ANIMATION_POSITION{ 150.0f,100.0f,0.0f };//QTEイベントの結果UI(成功版)のアニメーション後の座標(終了演出)

	const Vector3 QTE_RESULT_FAILED_AFTER_END_DIRECTION_ANIMATION_POSITION{0.0f,0.0f,0.0f};//QTEイベントの結果UI(失敗版)のアニメーション後の座標(終了演出)

	const float AFTER_END_DIRECTION_ANIMATION_ANGLE = -10.0f;//QTEイベントの結果UIのアニメーション後の角度(終了演出)

	const float AFTER_START_DIRECTION_ANIMATION_ALPHA = 1.0f;//QTEイベントの結果UIのアニメーション後の透明度(開始演出)

	const float AFTER_END_DIRECTION_ANIMATION_ALPHA = 0.0f;//QTEイベントの結果UIのアニメーション後の透明度(終了演出)

	const float ANIMATION_PLAY_SPEED = 3.0f;//透明度を変えるアニメーションの再生速度
}

//デストラクタ
QteEvent::~QteEvent()
{
	DeleteGO(m_qteEventInput);
}

//開始処理
bool QteEvent::Start()
{
	//乱数の初期化
	srand(time(nullptr));

	//初期設定
	Init();

	//入力するコマンドの設定
	SetInputCommand();

	//QTEイベントでプレイヤー側が入力するクラスのインスタンスの生成
	m_qteEventInput = NewGO<QteEventInput>(0, "qteeventInput");

	//QTEイベントの結果の終了演出の後の座標
	const Vector3 qteEventResultUIEndAnimationPosition[enQteEventResultUIDirection_Num] = {
		QTE_RESULT_SUCCESS_AFTER_END_DIRECTION_ANIMATION_POSITION,
		QTE_RESULT_FAILED_AFTER_END_DIRECTION_ANIMATION_POSITION
	};

	for (int i = 0; i < enQteEventResult_Num; i++)
	{
		//スプライトのアニメーションクラスのインスタンスの生成
		
		//座標を変えるアニメーションクラス
		m_positionSpriteAnimation[i][enQteEventResultUIDirection_Start] = std::make_unique<PositionSpriteAnimation>(
			&m_qteEventResultUI[i], //アニメーションをさせるスプライト
			1.0f, //ターゲットの割合
			ANIMATION_PLAY_SPEED,//アニメーションの再生速度
			false, //ループするか?
			m_qteEventResultUI[i].GetPosition(), //元の位置
			AFTER_START_DIRECTION_ANIMATION_POSITION//ターゲットの位置
		);

		m_positionSpriteAnimation[i][enQteEventResultUIDirection_End] = std::make_unique<PositionSpriteAnimation>(
			&m_qteEventResultUI[i], //アニメーションをさせるスプライト
			1.0f, //ターゲットの割合
			ANIMATION_PLAY_SPEED,//アニメーションの再生速度
			false, //ループするか?
			AFTER_START_DIRECTION_ANIMATION_POSITION, //元の位置
			qteEventResultUIEndAnimationPosition[i]//ターゲットの位置
		);


		//透明度を変えるアニメーションクラス
		m_alphaSpriteAnimation[i][enQteEventResultUIDirection_Start] = std::make_unique<AlphaSpriteAnimation>(
			&m_qteEventResultUI[i],  //アニメーションをさせるスプライト
			1.0f,  //ターゲットの割合
			ANIMATION_PLAY_SPEED,//アニメーションの再生速度
			false, //ループするか?
			m_qteEventResultUI[i].GetMulColor().a, //元の透明度
			AFTER_START_DIRECTION_ANIMATION_ALPHA//ターゲットの透明度
		);

		m_alphaSpriteAnimation[i][enQteEventResultUIDirection_End] = std::make_unique<AlphaSpriteAnimation>(
			&m_qteEventResultUI[i],  //アニメーションをさせるスプライト
			1.0f,  //ターゲットの割合
			ANIMATION_PLAY_SPEED,//アニメーションの再生速度
			false, //ループするか?
			AFTER_START_DIRECTION_ANIMATION_ALPHA, //元の透明度
			AFTER_END_DIRECTION_ANIMATION_ALPHA//ターゲットの透明度
		);
	}

	//QTEイベントの結果UIのアニメーション後の回転(終了演出)の設定
	Quaternion afterEndDirectionAnimationRotation;
	afterEndDirectionAnimationRotation.SetRotationDegZ(AFTER_END_DIRECTION_ANIMATION_ANGLE);

	//回転を変えるアニメーションクラス
	m_rotationSpriteAnimation = std::make_unique<RotationSpriteAnimation>(
		&m_qteEventResultUI[enQteEventResult_Failed],  //アニメーションをさせるスプライト
		1.0f,  //ターゲットの割合
		ANIMATION_PLAY_SPEED,//アニメーションの再生速度
		false, //ループするか?
		m_qteEventResultUI[enQteEventResult_Failed].GetRotation(), //元の回転
		afterEndDirectionAnimationRotation//ターゲットの回転
	);

	return true;
}

//更新処理
void QteEvent::Update()
{
	if (!m_player->GetPlayerCatchEnemy()->IsQteEvent() || !m_player->GetGamePtr())
	{
		return;
	}

	//QTEイベントでプレイヤー側が入力する処理の実行
	m_qteEventInput->Execute();

	//ゲームパッドの入力前後のUIの更新処理
	GamePadInputUIUpdate();

	//QTEイベントでコマンド入力に失敗したら処理する
	if (m_qteEventInput->IsInputFailed())
	{
		//入力に失敗したときの処理
		InputFailed();
	}

	//制限時間が止まっていなかったら かつ
	//プレイヤー側が最後のコマンドを入力していないときに処理する
	if (!m_isStopTimeLimit && !(m_qteEventInput->IsInputLastCommand() && m_succesInputCommandCount >= SUCCESS_INPUT_COMMAND_COUNT_MAX))
	{
		//制限時間の更新処理
		TimeLimitUpdate();
	}

	//QTEイベントの結果が出たら処理する
	if (m_isQteEventResult[enQteEventResult_Success] == true || m_isQteEventResult[enQteEventResult_Failed] == true)
	{
		//演出が流れている
		m_isResultDirection = true;
		//スプライトのアニメーションの更新処理
		m_positionSpriteAnimation[m_qteEventResult][m_qteEventResultUIDirection]->Update();
		m_alphaSpriteAnimation[m_qteEventResult][m_qteEventResultUIDirection]->Update();
		
		//QTEイベントで成功したとき
		if (m_qteEventResult == enQteEventResult_Success)
		{
			if (!m_isPlayQteEventResultSe)
			{
				GameSoundEngine::GetInstance()->PlaySE(GameSoundList_SE_QteEventSuccess, 1.0f);
				m_isPlayQteEventResultSe = true;
			}
			m_player->GetPlayerCatchEnemy()->QteEventSuccessMove();
		}
		//QTEイベントで失敗したとき
		else if (m_qteEventResult == enQteEventResult_Failed)
		{
			if (!m_isPlayQteEventResultSe)
			{
				GameSoundEngine::GetInstance()->PlaySE(GameSoundList_SE_QteEventFailed, 2.0f);
				m_isPlayQteEventResultSe = true;
			}
			if (m_targetEnemy->GetEnemyAIPtr()->IsSpeedUp())
			{
				m_player->GetPlayerCatchEnemy()->QteEventFailedMove();
			}
		}

		//QTEイベントの結果が失敗　かつ
		//QTEイベントの演出が終了演出なら処理する
		if (m_qteEventResult == enQteEventResult_Failed && m_qteEventResultUIDirection == enQteEventResultUIDirection_End)
		{
			m_rotationSpriteAnimation->Update();
		}

		if (m_positionSpriteAnimation[m_qteEventResult][enQteEventResultUIDirection_End]->IsCompleted())//アニメーションが終わったか?
		{
			if (m_player->GetPlayerAnimation()->IsPlayAnimation() == PlayerAnimation::enAnimationList_Idle ||
				(m_player->GetPlayerAnimation()->IsPlayAnimation() == PlayerAnimation::enAnimationList_StandUp &&
				!m_player->GetModelData().IsPlayingAnimation()))
			{
				if (!m_targetEnemy->GetEnemyEffectPtr()->IsPlayEffect())
				{
					return;
				}

				//QTEイベントを終了する処理
				if (m_isQteEventResult[enQteEventResult_Success] == true)//QTEイベントで成功したときの処理
				{
					m_game->GetMiniMapPtr()->DeleteEnemyPtr(m_targetEnemy);
					DeleteGO(m_targetEnemy);
					m_game->QteEventSuccessCountUp();
					m_game->GetGameMissionPtr()->AddCurrentCaptureEnemyNum();
				}
				else//QTEイベントで失敗したときの処理
				{
					m_game->QteEventFailedCountUp();
				}
				m_player->GetPlayerCatchEnemy()->Reset();
			}
		}
		else if (m_positionSpriteAnimation[m_qteEventResult][enQteEventResultUIDirection_Start]->IsCompleted())//アニメーションが終わったか?
		{
			//結果を表示して特定のエフェクトを再生したら終了演出のアニメーションを再生する
			if (m_targetEnemy->GetEnemyEffectPtr()->IsPlayEffect() == EnemyEffect::enEnemyEffectList_EngineSmoke_Large ||
				m_targetEnemy->GetEnemyEffectPtr()->IsPlayEffect() == EnemyEffect::enEnemyEffectList_Explosion)
			{
				m_qteEventResultUIDirection = enQteEventResultUIDirection_End;
			}
		}
	}
}

//初期設定
void QteEvent::Init()
{
	//UI関連の初期化
	for (int i = 0; i < enGamePadInputList_Num; i++)
	{
		InitGamePadInputBeforeUI((EnGamePadInputList)i);//ゲームパッドのボタンや方向キーを入力前のUIの初期設定

		InitGamePadInputAfterUI((EnGamePadInputList)i);//ゲームパッドのボタンや方向キーを入力後のUIの初期設定
	}

	InitTimeLimitUI();//制限時間UI関連の初期化

	for (int j = 0; j < enQteEventResult_Num; j++)
	{
		InitQteEventResultUI((EnQteEventResult)j);//QTEイベントの結果UIの初期化
	}

	//コマンド入力リストのコンテナのメモリ容量の確保
	m_inputCommandList.reserve(15);
}

//ゲームパッドのボタンや方向キーを入力前のUIの初期設定
void QteEvent::InitGamePadInputBeforeUI(EnGamePadInputList enGamePadInputList)
{
	//ゲームパッドのボタンや方向キーを入力前のUIの初期化
	m_gamePadInputBeforeUI[enGamePadInputList].Init(m_gamepadInputBeforeUIFilePath[enGamePadInputList].c_str(), GAMEPAD_UI_WIDTH_AND_HEIGHT, GAMEPAD_UI_WIDTH_AND_HEIGHT);
	//ゲームパッドのボタンや方向キーを入力前のUIの座標の設定
	m_gamePadInputBeforeUI[enGamePadInputList].SetPosition(GAMEPAD_INPUT_BEFORE_UI_INIT_POSITION);
	//ゲームパッドのボタンや方向キーを入力前のUIの大きさの設定
	m_gamePadInputBeforeUI[enGamePadInputList].SetScale(GAMEPAD_UI_SCALE);
	//ゲームパッドのボタンや方向キーを入力前のUIの更新処理
	m_gamePadInputBeforeUI[enGamePadInputList].Update();
}

//ゲームパッドのボタンや方向キーを入力後のUIの初期設定
void QteEvent::InitGamePadInputAfterUI(EnGamePadInputList enGamePadInputList)
{
	//QTEイベント成功用
	//ゲームパッドのボタンや方向キーを入力後のUIの初期化
	m_gamePadInputAfterUI[enQteEventResult_Success][enGamePadInputList].Init(m_gamepadInputAfterUIFilePath[enGamePadInputList].c_str(), GAMEPAD_UI_WIDTH_AND_HEIGHT, GAMEPAD_UI_WIDTH_AND_HEIGHT);
	//ゲームパッドのボタンや方向キーを入力後のUIの座標の設定
	m_gamePadInputAfterUI[enQteEventResult_Success][enGamePadInputList].SetPosition(GAMEPAD_INPUT_AFTER_UI_INIT_POSITION);
	//ゲームパッドのボタンや方向キーを入力後のUIの大きさの設定
	m_gamePadInputAfterUI[enQteEventResult_Success][enGamePadInputList].SetScale(GAMEPAD_UI_SCALE);
	//ゲームパッドのボタンや方向キーを入力後のUIの更新処理
	m_gamePadInputAfterUI[enQteEventResult_Success][enGamePadInputList].Update();

	//QTEイベント終了用
	//ゲームパッドのボタンや方向キーを入力後のUIの初期化
	m_gamePadInputAfterUI[enQteEventResult_Failed][enGamePadInputList].Init(m_gamepadInputFailedUIFilePath[enGamePadInputList].c_str(), GAMEPAD_UI_WIDTH_AND_HEIGHT, GAMEPAD_UI_WIDTH_AND_HEIGHT);
	//ゲームパッドのボタンや方向キーを入力後のUIの座標の設定
	m_gamePadInputAfterUI[enQteEventResult_Failed][enGamePadInputList].SetPosition(GAMEPAD_INPUT_AFTER_UI_INIT_POSITION);
	//ゲームパッドのボタンや方向キーを入力後のUIの大きさの設定
	m_gamePadInputAfterUI[enQteEventResult_Failed][enGamePadInputList].SetScale(GAMEPAD_UI_SCALE);
	//ゲームパッドのボタンや方向キーを入力後のUIの更新処理
	m_gamePadInputAfterUI[enQteEventResult_Failed][enGamePadInputList].Update();
}

//制限時間UI関連の初期化
void QteEvent::InitTimeLimitUI()
{
	//制限時間UI
	m_timeLimitUI.Init("Assets/sprite/timeLimit/timeLimit.dds", TIME_LIMIT_UI_WIDTH, TIME_LIMIT_UI_HEIGHT);
	m_timeLimitUI.SetPosition(TIME_LIMIT_UI_INIT_POSITION);
	m_timeLimitUI.SetScale(TIME_LIMIT_UI_SCALE);
	m_timeLimitUI.SetPivot(TIME_LIMIT_UI_PIVOT);
	m_timeLimitUI.Update();

	//制限時間バーUI
	m_timeLimitBerUI.Init("Assets/sprite/timeLimit/timeLimitBer.dds", TIME_LIMIT_UI_WIDTH, TIME_LIMIT_UI_HEIGHT);
	m_timeLimitBerUI.SetPosition(TIME_LIMIT_BER_UI_INIT_POSITION);
	m_timeLimitBerUI.SetScale(TIME_LIMIT_BER_UI_SCALE);
	m_timeLimitBerUI.Update();

	//制限時間の上限の設定
	m_timeLimitMax = m_timeLimit;
}

//QTEイベントの結果UIの初期化
void QteEvent::InitQteEventResultUI(EnQteEventResult enQteEventResult)
{
	m_qteEventResultUI[enQteEventResult].Init(m_qteEventResultUIFilePath[enQteEventResult].c_str(), QTE_EVENT_RESULT_UI_WIDTH, QTE_EVENT_RESULT_UI_HEIGHT);
	m_qteEventResultUI[enQteEventResult].SetPosition(QTE_EVENT_RESULT_UI_INIT_POSITION);
	m_qteEventResultUI[enQteEventResult].SetScale(QTE_EVENT_RESULT_UI_SCALE);
	m_qteEventResultUI[enQteEventResult].SetMulColor(QTE_EVENT_RESULT_UI_MULCOLOR);
	m_qteEventResultUI[enQteEventResult].Update();
}

//入力するコマンドの設定
void QteEvent::SetInputCommand()
{
	//入力するコマンドをランダムで決める
	m_nextInputCommand = EnCommandList(rand() % enCommandList_Num);
	m_nowInputCommand = m_nextInputCommand;

	//現在入力するコマンド
	switch (m_nowInputCommand)
	{
	case QteEvent::enCommandList_Hadouken://波動拳
		m_inputCommandList.push_back(enGamePadInputList_DownArrow);
		m_inputCommandList.push_back(enGamePadInputList_RightDownArrow);
		m_inputCommandList.push_back(enGamePadInputList_RightArrow);
		break;
	case QteEvent::enCommandList_SyakunetsuHadouken://灼熱波動拳
		m_inputCommandList.push_back(enGamePadInputList_LeftArrow);
		m_inputCommandList.push_back(enGamePadInputList_LeftDownArrow);
		m_inputCommandList.push_back(enGamePadInputList_DownArrow);
		m_inputCommandList.push_back(enGamePadInputList_RightDownArrow);
		m_inputCommandList.push_back(enGamePadInputList_RightArrow);
		break;
	case QteEvent::enCommandList_Tatsumaki://竜巻旋風脚
		m_inputCommandList.push_back(enGamePadInputList_DownArrow);
		m_inputCommandList.push_back(enGamePadInputList_LeftDownArrow);
		m_inputCommandList.push_back(enGamePadInputList_LeftArrow);
		break;
	case QteEvent::enCommandList_Shoryuken://昇龍拳
		m_inputCommandList.push_back(enGamePadInputList_RightArrow);
		m_inputCommandList.push_back(enGamePadInputList_DownArrow);
		m_inputCommandList.push_back(enGamePadInputList_RightDownArrow);
		break;
	case QteEvent::enCommandList_SomersaultKick://サマーソルトキック
		m_inputCommandList.push_back(enGamePadInputList_DownArrow);
		m_inputCommandList.push_back(enGamePadInputList_UpArrow);
		break;
	case QteEvent::enCommandList_SonicBoom://ソニックブーム
		m_inputCommandList.push_back(enGamePadInputList_LeftArrow);
		m_inputCommandList.push_back(enGamePadInputList_RightArrow);
		break;
	case QteEvent::enCommandList_YogaFlame://ヨガフレイム
		m_inputCommandList.push_back(enGamePadInputList_RightArrow);
		m_inputCommandList.push_back(enGamePadInputList_RightDownArrow);
		m_inputCommandList.push_back(enGamePadInputList_DownArrow);
		m_inputCommandList.push_back(enGamePadInputList_LeftDownArrow);
		m_inputCommandList.push_back(enGamePadInputList_LeftArrow);
		break;
	case QteEvent::enCommandList_ScrewPiledriver://スクリューパイルドライバー
		m_inputCommandList.push_back(enGamePadInputList_RightArrow);
		m_inputCommandList.push_back(enGamePadInputList_RightDownArrow);
		m_inputCommandList.push_back(enGamePadInputList_DownArrow);
		m_inputCommandList.push_back(enGamePadInputList_LeftDownArrow);
		m_inputCommandList.push_back(enGamePadInputList_LeftArrow);
		m_inputCommandList.push_back(enGamePadInputList_LeftUpArrow);
		m_inputCommandList.push_back(enGamePadInputList_UpArrow);
		break;
	default:
		break;
	}

	//入力するボタンをランダムで決める
	int selectInputButton = rand() % 4;

	//入力するボタン
	switch (selectInputButton)
	{
	case A_BUTTON_SELECT_ID://Aボタン
		m_inputCommandList.push_back(enGamePadInputList_AButton);
		break;
	case B_BUTTON_SELECT_ID://Bボタン
		m_inputCommandList.push_back(enGamePadInputList_BButton);
		break;
	case X_BUTTON_SELECT_ID://Xボタン
		m_inputCommandList.push_back(enGamePadInputList_XButton);
		break;
	case Y_BUTTON_SELECT_ID://Yボタン
		m_inputCommandList.push_back(enGamePadInputList_YButton);
		break;
	default:
		break;
	}

	//入力するコマンドに応じてUIの設定をする
	for (int i = 0; i < m_inputCommandList.size(); i++)
	{
		SetGamePadInputUIPosition(i, m_inputCommandList[i]);//ゲームパッドのボタンや方向キーを入力前後のUIの位置設定
	}
}

//ゲームパッドのボタンや方向キーを入力前のUIの位置設定
void QteEvent::SetGamePadInputUIPosition(int commandNum, EnGamePadInputList enGamePadInputList)
{
	//表示するコマンドの総数を取得
	const int commandCount = m_inputCommandList.size();

	//UI全体の幅を計算
	const float totalWidth = (commandCount - 1) * GAMEPAD_UI_INTERVAL_POSITION;

	//最初のUIを配置すべきX座標を計算 (全体の幅の半分だけ左にずらす)
	const float startX = -totalWidth / 2.0f;

	//現在のコマンドのX座標を計算
	const float posX = startX + commandNum * GAMEPAD_UI_INTERVAL_POSITION;

	//コマンド入力の順番に応じてゲームパッドUIの座標を設定する
	Vector3 beforeUIPos = m_gamePadInputBeforeUI[enGamePadInputList].GetPosition();
	beforeUIPos.x = posX; //計算したX座標を適用

	//ゲームパッドのボタンや方向キーを入力前のUIの座標の設定
	m_gamePadInputBeforeUI[enGamePadInputList].SetPosition(beforeUIPos);
	//ゲームパッドのボタンや方向キーを入力前のUIの更新処理
	m_gamePadInputBeforeUI[enGamePadInputList].Update();

	//コマンド入力の順番に応じてゲームパッドUIの座標を設定する
	Vector3 afterUIPos = m_gamePadInputAfterUI[enQteEventResult_Success][enGamePadInputList].GetPosition();
	afterUIPos.x = posX; //計算したX座標を適用

	//ゲームパッドのボタンや方向キーを入力後のUIの座標の設定
	m_gamePadInputAfterUI[enQteEventResult_Success][enGamePadInputList].SetPosition(afterUIPos);
	//ゲームパッドのボタンや方向キーを入力後のUIの更新処理
	m_gamePadInputAfterUI[enQteEventResult_Success][enGamePadInputList].Update();
}

//ゲームパッドの入力前後のUIのイージング設定
void QteEvent::SetGamePadInputUIEasing(EnQteEventResult enQteEventResult, EnGamePadInputList enGamePadInputList, SpriteRender& easingBeforeUI, SpriteRender& easingAfterUI)
{
	//イージング前の位置の設定
	m_gamePadInputUIBeforeEasingPosition[enQteEventResult][enGamePadInputList] = easingAfterUI.GetPosition();

	//イージング後の位置の設定
	m_gamePadInputUIAfterEasingPosition[enQteEventResult][enGamePadInputList] = easingBeforeUI.GetPosition();

	//イージング中の位置の設定
	m_gamePadInputUIEasingPosition[enQteEventResult][enGamePadInputList] = easingAfterUI.GetPosition();

	//イージングの割合の設定
	m_gamePadInputUIEasingTime[enQteEventResult][enGamePadInputList] = 0.0f;

	//イージング実行可能な状態にする
	m_isGamePadUIEasingStart[enQteEventResult][enGamePadInputList] = true;
}

//ゲームパッドの入力前後のUIのイージング更新処理
void QteEvent::GamePadInputUIEasingUpdate(EnQteEventResult enQteEventResult, EnGamePadInputList enGamePadInputList, SpriteRender& afterInputUI)
{
	m_gamePadInputUIEasingTime[enQteEventResult][enGamePadInputList] += 2.5f * g_gameTime->GetFrameDeltaTime();

	m_gamePadInputUIBeforeEasingPosition[enQteEventResult][enGamePadInputList].y += 1.0f;

	//割合が1.0fになったら演出終了する
	if (m_gamePadInputUIEasingTime[enQteEventResult][enGamePadInputList] > 1.0f)
	{
		m_gamePadInputUIEasingTime[enQteEventResult][enGamePadInputList] = 1.0f;

		m_isGamePadUIEasingEnd[enQteEventResult][enGamePadInputList] = true;
	}

	//イージング処理
	m_gamePadInputUIEasingPosition[enQteEventResult][enGamePadInputList].Lerp(
			m_gamePadInputUIEasingTime[enQteEventResult][enGamePadInputList], //イージング用の割合
			m_gamePadInputUIBeforeEasingPosition[enQteEventResult][enGamePadInputList],//イージング前の位置
			m_gamePadInputUIAfterEasingPosition[enQteEventResult][enGamePadInputList]//イージング後の位置
			);

	//イージングした位置を設定
	afterInputUI.SetPosition(m_gamePadInputUIEasingPosition[enQteEventResult][enGamePadInputList]);

	//入力後のUIの更新処理
	afterInputUI.Update();
}

//ゲームパッドUIのリセット処理
void QteEvent::GamePadPushUIReset()
{
	//ゲームパッドの位置を初期位置に戻す
	for (int i = 0; i < m_inputCommandList.size(); i++)
	{
		//ゲームパッドのボタンや方向キーを入力前のUIの座標の設定
		m_gamePadInputBeforeUI[m_inputCommandList[i]].SetPosition(GAMEPAD_INPUT_BEFORE_UI_INIT_POSITION);
		//ゲームパッドのボタンや方向キーを入力前のUIの更新処理
		m_gamePadInputBeforeUI[m_inputCommandList[i]].Update();

		//ゲームパッドのボタンや方向キーを入力後のUIの座標の設定
		m_gamePadInputAfterUI[enQteEventResult_Success][m_inputCommandList[i]].SetPosition(GAMEPAD_INPUT_AFTER_UI_INIT_POSITION);
		//ゲームパッドのボタンや方向キーを入力後のUIの更新処理
		m_gamePadInputAfterUI[enQteEventResult_Success][m_inputCommandList[i]].Update();

		//イージング実行不可能な状態にする
		m_isGamePadUIEasingStart[enQteEventResult_Success][m_inputCommandList[i]] = false;
		m_isGamePadUIEasingEnd[enQteEventResult_Success][m_inputCommandList[i]] = false;

		//コマンド入力が成功しているか判断するフラグをリセット
		m_isInputCommandSuccess[m_inputCommandList[i]] = false;
	}

	//プレイヤー側の入力でのリセット処理
	m_qteEventInput->Reset();

	//コマンドリストの要素をすべて削除
	m_inputCommandList.clear();

	//入力するコマンドの設定
	SetInputCommand();
}

//ゲームパッドの入力前後のUIの更新処理
void QteEvent::GamePadInputUIUpdate()
{
	//入力したコマンドが最後のとき
	if (m_qteEventInput->IsInputLastCommand())
	{
		if (!m_isAddSuccessInputCommandCount)//カウントアップしていないとき
		{
			m_succesInputCommandCount++;//コマンド入力成功数をカウントアップ
			m_isAddSuccessInputCommandCount = true;//カウントアップ完了
		}
	}

	for (int i = 0; i < m_inputCommandList.size(); i++)
	{
		//コマンド入力が成功した時
		if (m_isInputCommandSuccess[m_inputCommandList[i]] == true)
		{
			//イージング設定ができていないとき
			if (m_isGamePadUIEasingStart[enQteEventResult_Success][m_inputCommandList[i]] != true)
			{
				//イージング設定
				SetGamePadInputUIEasing(
					enQteEventResult_Success,//QTEイベントの結果
					m_inputCommandList[i],//コマンドリスト
					m_gamePadInputBeforeUI[m_inputCommandList[i]],//イージング前の設定用のUI
					m_gamePadInputAfterUI[enQteEventResult_Success][m_inputCommandList[i]]//イージング後の設定用のUI
				);
			}
			//イージング設定ができているとき
			else
			{
				//イージング更新処理
				GamePadInputUIEasingUpdate(
					enQteEventResult_Success,//QTEイベントの結果
					m_inputCommandList[i],//コマンドリスト
					m_gamePadInputAfterUI[enQteEventResult_Success][m_inputCommandList[i]]//描画するUI
				);
			}

			//全てのコマンドの入力成功時の演出が終わったらリセット処理をする
			if (m_isGamePadUIEasingEnd[enQteEventResult_Success][m_inputCommandList[m_inputCommandList.size() - 1]] == true)
			{
				//コマンド入力が3回成功したらQTEイベントを終了する
				if (m_succesInputCommandCount >= SUCCESS_INPUT_COMMAND_COUNT_MAX)
				{
					m_qteEventResult = enQteEventResult_Success;
					m_isQteEventResult[enQteEventResult_Success] = true;
					StopTimeLimit();
					return;
				}

				GamePadPushUIReset();

				m_isAddSuccessInputCommandCount = false;

				m_qteEventInput->ResetInputLastCommand();
			}
		}
	}
}

//制限時間の更新処理
void QteEvent::TimeLimitUpdate()
{
	m_timeLimit -= g_gameTime->GetFrameDeltaTime();

	//制限時間が0秒になったらQTEイベントを終了する
	if (m_timeLimit <= 0.0f)
	{
		m_qteEventResult = enQteEventResult_Failed;
		m_isQteEventResult[enQteEventResult_Failed] = true;
		m_timeLimit = 0.0f;
		StopTimeLimit();
	}

	//制限時間UIの色を変える処理
	ChangeTimeLimitUIColor(m_timeLimit);

	float timeLimitMax = m_timeLimitMax;
	float timeLimitNow = m_timeLimit;
	float timeLimitRemnanth = timeLimitNow / timeLimitMax;
	Vector3 timeLimitUIScale = {0.5f,0.5f,0.5f};
	timeLimitUIScale.x *= timeLimitRemnanth;
	m_timeLimitUI.SetScale(timeLimitUIScale);
	m_timeLimitUI.Update();
}

//制限時間UIの色を変える処理
void QteEvent::ChangeTimeLimitUIColor(float timeLimit)
{
	//制限時間が上限の4分の1になったら制限時間UIのカラーを赤色に変える
	if (timeLimit < m_timeLimitMax / 4)
	{
		m_timeLimitUI.SetMulColor(TIME_LIMIT_UI_MULCOLOR_RED);
	}
	//制限時間が上限の半分になったら制限時間UIのカラーを黄色に変える
	else if (timeLimit < m_timeLimitMax / 2)
	{
		m_timeLimitUI.SetMulColor(TIME_LIMIT_UI_MULCOLOR_YELLOW);
	}
	else
	{
		m_timeLimitUI.SetMulColor(TIME_LIMIT_UI_MULCOLOR_WHITE);
	}
}

//入力に失敗したときの処理
void QteEvent::InputFailed()
{
	//イージング設定ができていないとき
	if (m_isGamePadUIEasingStart[enQteEventResult_Failed][m_qteEventInput->GetInputCommand()] != true)
	{
		//イージング設定
		SetGamePadInputUIEasing(
			enQteEventResult_Failed,//QTEイベントの結果
			(EnGamePadInputList)m_qteEventInput->GetInputCommand(),//コマンドリスト
			m_gamePadInputBeforeUI[m_inputCommandList[m_qteEventInput->GetNowInputCommandOrder()]],//イージング前の設定用のUI
			m_gamePadInputAfterUI[enQteEventResult_Success][m_inputCommandList[m_qteEventInput->GetNowInputCommandOrder()]]//イージング後の設定用のUI
		);

		//ゲームパッドのボタンや方向キーを入力後のUI(QTEイベント失敗用)の位置の設定
		m_gamePadInputAfterUI[enQteEventResult_Failed][m_qteEventInput->GetInputCommand()]
			.SetPosition(m_gamePadInputUIEasingPosition[enQteEventResult_Failed][m_qteEventInput->GetInputCommand()]);

		//ゲームパッドのボタンや方向キーを入力後のUI(QTEイベント失敗用)の更新処理
		m_gamePadInputAfterUI[enQteEventResult_Failed][m_qteEventInput->GetInputCommand()].Update();
	}
	//イージング設定ができているとき
	else
	{
		//イージング更新処理
		GamePadInputUIEasingUpdate(
			enQteEventResult_Failed,//QTEイベントの結果
			(EnGamePadInputList)m_qteEventInput->GetInputCommand(),//コマンドリスト
			m_gamePadInputAfterUI[enQteEventResult_Failed][m_qteEventInput->GetInputCommand()]//描画するUI
		);
	}

	//イージングが終わったら入力に失敗したときのリセット処理をする
	if (m_isGamePadUIEasingEnd[enQteEventResult_Failed][m_qteEventInput->GetInputCommand()] == true)
	{
		m_isGamePadUIEasingStart[enQteEventResult_Failed][m_qteEventInput->GetInputCommand()] = false;
		m_isGamePadUIEasingEnd[enQteEventResult_Failed][m_qteEventInput->GetInputCommand()] = false;
		m_qteEventInput->ResetInputFailed();
	}
}

//リセット処理
void QteEvent::Reset()
{
	m_succesInputCommandCount = 0;
	m_timeLimit = 15.0f;
	m_isStopTimeLimit = false;

	m_positionSpriteAnimation[m_qteEventResult][enQteEventResultUIDirection_Start]->Reset();
	m_positionSpriteAnimation[m_qteEventResult][enQteEventResultUIDirection_End]->Reset();

	m_alphaSpriteAnimation[m_qteEventResult][enQteEventResultUIDirection_Start]->Reset();
	m_alphaSpriteAnimation[m_qteEventResult][enQteEventResultUIDirection_End]->Reset();

	m_rotationSpriteAnimation->Reset();

	m_qteEventResult = enQteEventResult_Num;

	m_isQteEventResult[enQteEventResult_Success] = false;
	m_isQteEventResult[enQteEventResult_Failed] = false;

	m_qteEventResultUIDirection = enQteEventResultUIDirection_Start;

	m_isPlayQteEventResultSe = false;

	for (int i = 0; i < enQteEventResult_Num; i++)
	{
		m_qteEventResultUI[i].SetPosition(QTE_EVENT_RESULT_UI_INIT_POSITION);
		m_qteEventResultUI[i].SetMulColor(QTE_EVENT_RESULT_UI_MULCOLOR);
		m_qteEventResultUI[i].Update();
	}
}

//描画処理
void QteEvent::Render(RenderContext& rc)
{
	if (!m_player->GetPlayerCatchEnemy()->IsQteEvent())
	{
		return;
	}

	for (int i = 0; i < enQteEventResult_Num; i++)
	{
		//QTEの結果が出ているかときに描画する
		if (m_isQteEventResult[i] == true)
		{
			//QTE結果UIの描画処理
			m_qteEventResultUI[i].Draw(rc);
		}
	}

	if (m_isResultDirection)
	{
		return;
	}

	for (int i = 0; i < m_inputCommandList.size(); i++)
	{
		//ゲームパッドのボタンや方向キーを入力前のUIの描画処理
		m_gamePadInputBeforeUI[m_inputCommandList[i]].Draw(rc);

		//コマンド入力が成功しているときに描画する
		if (m_isInputCommandSuccess[m_inputCommandList[i]] == true)
		{
			//ゲームパッドのボタンや方向キーを入力後のUIの描画処理(QTEイベント成功用)
			m_gamePadInputAfterUI[enQteEventResult_Success][m_inputCommandList[i]].Draw(rc);
		}

		//コマンド入力が失敗しているときに描画する
		if (m_qteEventInput->IsInputFailed())
		{
			//ゲームパッドのボタンや方向キーを入力後のUIの描画処理(QTEイベント失敗用)
			m_gamePadInputAfterUI[enQteEventResult_Failed][m_qteEventInput->GetInputCommand()].Draw(rc);
		}
	}

	//制限時間バーUIの描画
	m_timeLimitBerUI.Draw(rc);

	//制限時間UIの描画
	m_timeLimitUI.Draw(rc);
}