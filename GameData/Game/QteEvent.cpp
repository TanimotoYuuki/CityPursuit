#include "stdafx.h"
#include "QteEvent.h"
#include "QteEventInput.h"

namespace {
	//ゲームパッドUI
	const float GAMEPAD_UI_WIDTH_AND_HEIGHT = 1024;//ゲームパッドUIの横幅と縦幅

	const Vector3 GAMEPAD_INPUT_BEFORE_UI_INIT_POSITION{ 0.0f,0.0f,0.0f };//ゲームパッドのボタンや方向キーを入力前のUIの初期位置

	const Vector3 GAMEPAD_INPUT_AFTER_UI_INIT_POSITION{ 0.0f,-25.0f,0.0f };//ゲームパッドのボタンや方向キーを入力後のUIの初期位置

	const Vector3 GAMEPAD_UI_SCALE{ 0.1f,0.1f,0.1f };//ゲームパッドUIの大きさ

	const float GAMEPAD_UI_INTERVAL_POSITION = 125.0f;//ゲームパッドUIの間隔位置

	//ボタン
	const int A_BUTTON_SELECT_ID = 0;//AボタンID

	const int B_BUTTON_SELECT_ID = 1;//BボタンID

	const int X_BUTTON_SELECT_ID = 2;//XボタンID

	const int Y_BUTTON_SELECT_ID = 3;//YボタンID
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

	m_qteEventInput = NewGO<QteEventInput>(0, "qteeventInput");

	return true;
}

//更新処理
void QteEvent::Update()
{
	m_qteEventInput->Execute();

	for (int i = 0; i < m_inputCommandList.size(); i++)
	{
		//コマンド入力が成功した時
		if (m_isInputCommandSuccess[m_inputCommandList[i]] == true)
		{
			//イージング設定ができていないとき
			if (m_isGamePadUIEasingStart[m_inputCommandList[i]] != true)
			{
				//イージング設定
				SetGamePadInputUIEasing(m_inputCommandList[i], m_gamePadInputBeforeUI[m_inputCommandList[i]], m_gamePadInputAfterUI[m_inputCommandList[i]]);
			}
			//イージング設定ができているとき
			else
			{
				//イージング更新処理
				GamePadInputUIEasingUpdate(m_inputCommandList[i], m_gamePadInputAfterUI[m_inputCommandList[i]]);
			}

			//全てのコマンドの入力成功時の演出が終わったらリセット処理をする
			if (m_isGamePadUIEasingEnd[m_inputCommandList[m_inputCommandList.size() - 1]] == true)
			{
				GamePadPushUIReset();
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
	//ゲームパッドのボタンや方向キーを入力後のUIの初期化
	m_gamePadInputAfterUI[enGamePadInputList].Init(m_gamepadInputAfterUIFilePath[enGamePadInputList].c_str(), GAMEPAD_UI_WIDTH_AND_HEIGHT, GAMEPAD_UI_WIDTH_AND_HEIGHT);
	//ゲームパッドのボタンや方向キーを入力後のUIの座標の設定
	m_gamePadInputAfterUI[enGamePadInputList].SetPosition(GAMEPAD_INPUT_AFTER_UI_INIT_POSITION);
	//ゲームパッドのボタンや方向キーを入力後のUIの大きさの設定
	m_gamePadInputAfterUI[enGamePadInputList].SetScale(GAMEPAD_UI_SCALE);
	//ゲームパッドのボタンや方向キーを入力後のUIの更新処理
	m_gamePadInputAfterUI[enGamePadInputList].Update();
}

//入力するコマンドの設定
void QteEvent::SetInputCommand()
{
	//入力するコマンドをランダムで決める
	m_nextInputCommand = EnCommandList(rand() % enCommandList_Num);
	m_nowInputCommand = enCommandList_ScrewPiledriver;

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
	Vector3 afterUIPos = m_gamePadInputAfterUI[enGamePadInputList].GetPosition();
	afterUIPos.x = posX; //計算したX座標を適用

	//ゲームパッドのボタンや方向キーを入力後のUIの座標の設定
	m_gamePadInputAfterUI[enGamePadInputList].SetPosition(afterUIPos);
	//ゲームパッドのボタンや方向キーを入力後のUIの更新処理
	m_gamePadInputAfterUI[enGamePadInputList].Update();
}

//ゲームパッドの入力前後のUIのイージング設定
void QteEvent::SetGamePadInputUIEasing(EnGamePadInputList enGamePadInputList, SpriteRender& easingBeforeUI, SpriteRender& easingAfterUI)
{
	//イージング前の位置の設定
	m_gamePadInputUIBeforeEasingPosition[enGamePadInputList] = easingAfterUI.GetPosition();

	//イージング後の位置の設定
	m_gamePadInputUIAfterEasingPosition[enGamePadInputList] = easingBeforeUI.GetPosition();

	//イージング中の位置の設定
	m_gamePadInputUIEasingPosition[enGamePadInputList] = easingAfterUI.GetPosition();

	//イージングの割合の設定
	m_gamePadInputUIEasingTime[enGamePadInputList] = 0.0f;

	//イージング実行可能な状態にする
	m_isGamePadUIEasingStart[enGamePadInputList] = true;
}

//ゲームパッドの入力前後のUIのイージング更新処理
void QteEvent::GamePadInputUIEasingUpdate(EnGamePadInputList enGamePadInputList, SpriteRender& afterInputUI)
{
	m_gamePadInputUIEasingTime[enGamePadInputList] += 2.5f * g_gameTime->GetFrameDeltaTime();

	m_gamePadInputUIBeforeEasingPosition[enGamePadInputList].y += 1.0f;

	//割合が1.0fになったら演出終了する
	if (m_gamePadInputUIEasingTime[enGamePadInputList] > 1.0f)
	{
		m_gamePadInputUIEasingTime[enGamePadInputList] = 1.0f;
		m_isGamePadUIEasingEnd[enGamePadInputList] = true;
	}

	//イージング処理
	m_gamePadInputUIEasingPosition[enGamePadInputList].Lerp(
			m_gamePadInputUIEasingTime[enGamePadInputList], //イージング用の割合
			m_gamePadInputUIBeforeEasingPosition[enGamePadInputList],//イージング前の位置
			m_gamePadInputUIAfterEasingPosition[enGamePadInputList]//イージング後の位置
			);

	//イージングした位置を設定
	afterInputUI.SetPosition(m_gamePadInputUIEasingPosition[enGamePadInputList]);

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
		m_gamePadInputAfterUI[m_inputCommandList[i]].SetPosition(GAMEPAD_INPUT_AFTER_UI_INIT_POSITION);
		//ゲームパッドのボタンや方向キーを入力後のUIの更新処理
		m_gamePadInputAfterUI[m_inputCommandList[i]].Update();

		//イージング実行不可能な状態にする
		m_isGamePadUIEasingStart[m_inputCommandList[i]] = false;
		m_isGamePadUIEasingEnd[m_inputCommandList[i]] = false;

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

//描画処理
void QteEvent::Render(RenderContext& rc)
{
	for (int i = 0; i < m_inputCommandList.size(); i++)
	{
		//ゲームパッドのボタンや方向キーを入力前のUIの描画処理
		m_gamePadInputBeforeUI[m_inputCommandList[i]].Draw(rc);

		//コマンド入力が成功しているときに描画する
		if (m_isInputCommandSuccess[m_inputCommandList[i]] == true)
		{
			{
				//ゲームパッドのボタンや方向キーを入力後のUIの描画処理
				m_gamePadInputAfterUI[m_inputCommandList[i]].Draw(rc);
			}
		}
	}
}