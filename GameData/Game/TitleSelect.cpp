#include "stdafx.h"
#include "TitleSelect.h"

namespace {
	//選択テキストUI
	const float SELECT_TEXT_UI_WIDTH = 1024.0f;//選択テキストUIの横幅

	const float SELECT_TEXT_UI_HEIGHT = 128.0f;//選択テキストUIの縦幅

	const Vector3 GAME_START_TEXT_UI_POSITION{ 0.0f,150.0f,0.0f };//選択テキストUIの座標(ゲームスタート)

	const Vector3 HOW_TO_PLAY_TEXT_UI_POSITION{ 0.0f,0.0f,0.0f };//選択テキストUIの座標(遊び方)

	const Vector3 GAME_FINISH_TEXT_UI_POSITION{ 0.0f,-150.0f,0.0f };//選択テキストUIの座標(ゲーム終了)

	const Vector3 SELECT_TEXT_UI_SCALE{ 0.6f,0.6f,0.6f };//選択テキストUIの大きさ

	//現在何を選択しているかを表すUI
	const float CURRENT_SELECT_UI_WIDTH = 1024.0f;//現在何を選択しているかを表すUIの横幅

	const float CURRENT_SELECT_UI_HEIGHT = 1024.0f;//現在何を選択しているかを表すUIの縦幅

	const float CURRENT_SELECT_UI_POSITION_Y_OFFSET = 310.0f;//現在何を選択しているかを表すUIのY座標のオフセット

	const Vector3 CURRENT_SELECT_UI_SCALE{ 0.15f,0.15f,0.15f };//現在何を選択しているかを表すUIの大きさ

	//選択
	const int SELECT_UP = 1;//上に選択

	const int SELECT_DOWN = 1;//下に選択

	//アニメーション
	const float ANIMATION_SPEED_NORMAL = 5.0f;//通常のアニメーション再生速度

	const float ANIMATION_SPEED_SELECT = 3.5f;//選択したときのアニメーション再生速度

	//スプライトのアニメーション
	const Vector2 AFTER_SCALE_DOWN_SCALE{ 0.115f,0.115f };//スプライトのアニメーション後の大きさ

	const Vector3 AFTER_PUSH_DOWN_COLOR{ 0.82f,0.82f,0.82f };//スプライトの押されたときのアニメーション後の色

	//時間
	const float DELAY_TIME = 0.2f;//待機時間
}

//開始処理
bool TitleSelect::Start()
{
	//選択テキストUI用の座標の設定
	Vector3 selectTextUIPosition[enTitleSelect_Num] = {
		GAME_START_TEXT_UI_POSITION,
		HOW_TO_PLAY_TEXT_UI_POSITION,
		GAME_FINISH_TEXT_UI_POSITION
	};

	for (int i = 0; i < enTitleSelect_Num; i++)
	{
		//選択テキストUIの初期化
		m_selectTextUIPosition[i] = selectTextUIPosition[i];
		InitSelectTextUI((EnTitleSelect)i);

		//選択テキストUI用の座標の設定
		m_currentSelectUIPosition[i] = m_selectTextUIPosition[i];
		m_currentSelectUIPosition[i].x -= CURRENT_SELECT_UI_POSITION_Y_OFFSET;
		m_defaultCurrentSelectUIPosition[i] = m_currentSelectUIPosition[i];
	}

	//現在何を選択しているかを表すUIの初期化
	InitCurrentSelectUI();

	Vector2 currentSelectUIScale = {
		m_currentSelectUI.GetScale().x,
		m_currentSelectUI.GetScale().y
	};

	//大きさを変えるアニメーションクラス
	m_currentSelectUIScaleDownAnimation = std::make_unique<ScaleSpriteAnimation>(
		&m_currentSelectUI,//アニメーションをさせるスプライト
		1.0f,//ターゲットの割合
		ANIMATION_SPEED_SELECT,//アニメーションの再生速度
		false,//ループするか？
		currentSelectUIScale,//元の大きさ
		AFTER_SCALE_DOWN_SCALE//ターゲットの大きさ
	);

	m_currentSelectUIScaleUpAnimation = std::make_unique<ScaleSpriteAnimation>(
		&m_currentSelectUI,//アニメーションをさせるスプライト
		1.0f,//ターゲットの割合
		ANIMATION_SPEED_SELECT,//アニメーションの再生速度
		false,//ループするか？
		AFTER_SCALE_DOWN_SCALE,//元の大きさ
		currentSelectUIScale//ターゲットの大きさ
	);

	Vector3 currentSelectUIColor = {
		m_currentSelectUI.GetMulColor().x,
		m_currentSelectUI.GetMulColor().y,
		m_currentSelectUI.GetMulColor().z
	};

	//色を変えるアニメーションクラス
	m_currentSelectUIPushDownAnimation = std::make_unique<ColorSpriteAnimation>(
		&m_currentSelectUI,//アニメーションをさせるスプライト
		1.0f,//ターゲットの割合
		ANIMATION_SPEED_SELECT,//アニメーションの再生速度
		false,//ループするか？
		currentSelectUIColor,//元の色
		AFTER_PUSH_DOWN_COLOR//ターゲットの色
	);

	m_currentSelectUIPushUpAnimation = std::make_unique<ColorSpriteAnimation>(
		&m_currentSelectUI,//アニメーションをさせるスプライト
		1.0f,//ターゲットの割合
		ANIMATION_SPEED_SELECT,//アニメーションの再生速度
		false,//ループするか？
		AFTER_PUSH_DOWN_COLOR,//元の色
		currentSelectUIColor//ターゲットの色
	);

	return true;
}

//実行処理
void TitleSelect::Execute()
{
	if (m_currentSelectUIScaleUpAnimation->IsCompleted())
	{
		if (g_gameTime->StopWatch(DELAY_TIME))
		{
			m_isFinishSelectDecisionDirection = true;
		}
		return;
	}

	//選択できていないとき処理する
	if (!m_isSelect)
	{
		//入力の更新処理
		InputUpdate();
	}

	//現在何を選択しているかを表すUIの更新処理
	CurrentSelectUIUpdate((EnTitleSelect)m_currentSelect);
}

//描画処理
void TitleSelect::Render(RenderContext& rc)
{
	//UIを描画しないなら処理しない
	if (!m_isDrawingUI)
	{
		return;
	}

	for (int i = 0; i < enTitleSelect_Num; i++)
	{
		//選択テキストUIの描画
		m_selectTextUI[i].Draw(rc);
	}

	//現在何を選択しているかを表すUIの描画
	m_currentSelectUI.Draw(rc);
}

//選択テキストUIの初期化
void TitleSelect::InitSelectTextUI(EnTitleSelect enTitleSelect)
{
	//選択テキストUIのの初期化
	m_selectTextUI[enTitleSelect].Init(m_selectTextUIFilePath[enTitleSelect].c_str(), SELECT_TEXT_UI_WIDTH, SELECT_TEXT_UI_HEIGHT);
	//選択テキストUIの座標の設定
	m_selectTextUI[enTitleSelect].SetPosition(m_selectTextUIPosition[enTitleSelect]);
	//選択テキストUIの大きさの設定
	m_selectTextUI[enTitleSelect].SetScale(SELECT_TEXT_UI_SCALE);
	//選択テキストUIの更新処理
	m_selectTextUI[enTitleSelect].Update();
}

//現在何を選択しているかを表すUIの初期化
void TitleSelect::InitCurrentSelectUI()
{
	//現在何を選択しているかを表すUIの初期化
	m_currentSelectUI.Init(m_currentSelectUIFilePath.c_str(), CURRENT_SELECT_UI_WIDTH, CURRENT_SELECT_UI_HEIGHT);
	//現在何を選択しているかを表すUIの座標の設定(デフォルトの選択はゲームスタート)
	m_currentSelectUI.SetPosition(m_currentSelectUIPosition[enTitleSelect_GameStart]);
	//現在何を選択しているかを表すUIの大きさの設定
	m_currentSelectUI.SetScale(CURRENT_SELECT_UI_SCALE);
	//現在何を選択しているかを表すUIの更新処理
	m_currentSelectUI.Update();
}

//現在何を選択しているかを表すUIの更新処理
void TitleSelect::CurrentSelectUIUpdate(EnTitleSelect enTitleSelect)
{
	//前に選択したところの位置をデフォルトに戻す
	m_currentSelectUIPosition[m_previousSelect] = m_defaultCurrentSelectUIPosition[m_previousSelect];

	//現在何を選択しているかを表すUIのアニメーションの更新処理
	CurrentSelectUIAnimationUpdate();

	//現在何を選択しているかを表すUIの座標の設定
	m_currentSelectUI.SetPosition(m_currentSelectUIPosition[enTitleSelect]);
	//現在何を選択しているかを表すUIの更新処理
	m_currentSelectUI.Update();
}

//現在何を選択しているかを表すUIのアニメーションの更新処理
void TitleSelect::CurrentSelectUIAnimationUpdate()
{
	if (m_isSelect)
	{
		//大きさを変えるアニメーション
		//大きさを縮小するアニメーションの再生が終わっているか?
		m_currentSelectUIScaleDownAnimation->IsCompleted() != true ?
			m_currentSelectUIScaleDownAnimation->Update() ://終わっていなければ引き継ぎ、縮小するアニメーションの再生をする
			m_currentSelectUIScaleUpAnimation->Update();//終わっていたら拡大するアニメーションの再生をする

		
		//色を変えるアニメーション
		//押されたときのアニメーションの再生が終わっているか?
		m_currentSelectUIPushDownAnimation->IsCompleted() != true ?
			m_currentSelectUIPushDownAnimation->Update() ://終わっていなければ引き継ぎ、押されたときのアニメーションの再生をする
			m_currentSelectUIPushUpAnimation->Update();//終わっていたら離したときのアニメーションの再生をする

		return;
	}

	m_time += ANIMATION_SPEED_NORMAL * g_gameTime->GetFrameDeltaTime();
	m_currentSelectUIPosition[m_currentSelect].x += sin(m_time) * 0.5f;
}

//入力の更新処理
void TitleSelect::InputUpdate()
{
	//十字キーを上に倒す または
	//左スティックを上に倒したとき
	if (g_pad[0]->IsTrigger(enButtonUp) || g_pad[0]->IsTriggerLStickUp())
	{
		m_time = 0.0f;
		m_previousSelect = m_currentSelect;
		GameSoundEngine::GetInstance()->PlaySE(GameSoundList_SE_Select, 1.2f);

		if (m_currentSelect == enTitleSelect_GameStart)
		{
			m_currentSelect = enTitleSelect_GameFinish;
			return;
		}

		m_currentSelect -= SELECT_UP;//上に選択
	}
	//十字キーを下に倒す または
	//左スティックを下に倒したとき
	else if (g_pad[0]->IsTrigger(enButtonDown) || g_pad[0]->IsTriggerLStickDown())
	{
		m_time = 0.0f;
		m_previousSelect = m_currentSelect;
		GameSoundEngine::GetInstance()->PlaySE(GameSoundList_SE_Select, 1.2f);

		if (m_currentSelect == enTitleSelect_GameFinish)
		{
			m_currentSelect = enTitleSelect_GameStart;
			return;
		}

		m_currentSelect += SELECT_UP;//下に選択
	}

	//Aボタンを押したとき
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_time = 0.0f;
		GameSoundEngine::GetInstance()->PlaySE(GameSoundList_SE_Decision, 1.2f);
		m_isSelect = true;
	}
}

//リセット処理
void TitleSelect::Reset()
{
	m_time = 0.0f;

	m_currentSelectUIPosition[m_currentSelect] = m_defaultCurrentSelectUIPosition[m_currentSelect];
	m_currentSelectUI.SetPosition(m_currentSelectUIPosition[m_currentSelect]);
	m_currentSelectUI.Update();

	ResetScaleAnimation();
	ResetColorAnimation();

	m_isSelect = false;
	m_isFinishSelectDecisionDirection = false;
}
