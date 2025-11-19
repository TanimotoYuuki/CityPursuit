#include "stdafx.h"
#include "GameEndSelect.h"
#include "SceneManager.h"
#include "FadeManager.h"

namespace {
	//選択テキストUI
	const float SELECT_TEXT_UI_WIDTH = 1024.0f;//選択テキストUIの横幅

	const float SELECT_TEXT_UI_HEIGHT = 128.0f;//選択テキストUIの縦幅

	const Vector3 RETRY_TEXT_UI_POSITION{ 0.0f,75.0f,0.0f };//選択テキストUIの座標(リトライ)

	const Vector3 RETURN_TITLE_TEXT_UI_POSITION{ 0.0f,-75.0f,0.0f };//選択テキストUIの座標(タイトルへ戻る)

	const Vector3 SELECT_TEXT_UI_SCALE{ 0.6f,0.6f,0.6f };//選択テキストUIの大きさ

	//現在何を選択しているかを表すUI
	const float CURRENT_SELECT_UI_WIDTH = 1024.0f;//現在何を選択しているかを表すUIの横幅

	const float CURRENT_SELECT_UI_HEIGHT = 1024.0f;//現在何を選択しているかを表すUIの縦幅

	const float CURRENT_SELECT_UI_POSITION_Y_OFFSET = 300.0f;//現在何を選択しているかを表すUIのY座標のオフセット

	const Vector3 CURRENT_SELECT_UI_SCALE{ 0.15f,0.15f,0.15f };//現在何を選択しているかを表すUIの大きさ

	//選択
	const int SELECT_UP = 1;//上に選択

	const int SELECT_DOWN = 1;//下に選択

	//アニメーション
	const float ANIMATION_SPEED_NORMAL = 1.5f;//通常のアニメーション再生速度

	const float ANIMATION_SPEED_SELECT = 5.0f;//選択したときのアニメーション再生速度

	//カウント
	const int NO_DRAWING_CURRENT_SELECT_UI_COUNT_MAX = 5;//現在何を選択しているかを表すUIを描画していない回数の上限
}

//開始処理
bool GameEndSelect::Start()
{
	//選択テキストUI用の座標の設定
	Vector3 selectTextUIPosition[enGameEndSelect_Num] = {
		RETRY_TEXT_UI_POSITION,
		RETURN_TITLE_TEXT_UI_POSITION
	};

	for (int i = 0; i < enGameEndSelect_Num; i++)
	{
		//選択テキストUIの初期化
		m_selectTextUIPosition[i] = selectTextUIPosition[i];
		InitSelectTextUI((EnGameEndSelect)i);

		//選択テキストUI用の座標の設定
		m_currentSelectUIPosition[i] = m_selectTextUIPosition[i];
		m_currentSelectUIPosition[i].x -= CURRENT_SELECT_UI_POSITION_Y_OFFSET;
	}

	//現在何を選択しているかを表すUIの初期化
	InitCurrentSelectUI();

	return true;
}

//実行処理
void GameEndSelect::Execute()
{
	//一定の回数までカウントしたら処理しない
	if (m_noDrawingCurrentSelectUICount == NO_DRAWING_CURRENT_SELECT_UI_COUNT_MAX)
	{
		//シーンの遷移処理
		TransitionScene();
		return;
	}

	//現在何を選択しているかを表すUIの更新処理
	CurrentSelectUIUpdate((EnGameEndSelect)m_currentSelect);

	//選択できていないとき処理する
	if (!m_isSelect)
	{
		//入力の更新処理
		InputUpdate();
	}
}

//描画処理
void GameEndSelect::Render(RenderContext& rc)
{
	//UIを描画しないなら処理しない
	if (!m_isDrawingUI)
	{
		return;
	}

	for (int i = 0; i < enGameEndSelect_Num; i++)
	{
		//選択テキストUIの描画
		m_selectTextUI[i].Draw(rc);
	}

	if (m_isDrawingCurrentSelectUI)
	{
		//現在何を選択しているかを表すUIの描画
		m_currentSelectUI.Draw(rc);
	}
}

//選択テキストUIの初期化
void GameEndSelect::InitSelectTextUI(EnGameEndSelect enGameEndSelect)
{
	//選択テキストUIのの初期化
	m_selectTextUI[enGameEndSelect].Init(m_selectTextUIFilePath[enGameEndSelect].c_str(), SELECT_TEXT_UI_WIDTH, SELECT_TEXT_UI_HEIGHT);
	//選択テキストUIの座標の設定
	m_selectTextUI[enGameEndSelect].SetPosition(m_selectTextUIPosition[enGameEndSelect]);
	//選択テキストUIの大きさの設定
	m_selectTextUI[enGameEndSelect].SetScale(SELECT_TEXT_UI_SCALE);
	//選択テキストUIの更新処理
	m_selectTextUI[enGameEndSelect].Update();
}

//現在何を選択しているかを表すUIの初期化
void GameEndSelect::InitCurrentSelectUI()
{
	//現在何を選択しているかを表すUIの初期化
	m_currentSelectUI.Init(m_currentSelectUIFilePath.c_str(), CURRENT_SELECT_UI_WIDTH, CURRENT_SELECT_UI_HEIGHT);
	//現在何を選択しているかを表すUIの座標の設定(デフォルトの選択はリトライ)
	m_currentSelectUI.SetPosition(m_currentSelectUIPosition[enGameEndSelect_Retry]);
	//現在何を選択しているかを表すUIの大きさの設定
	m_currentSelectUI.SetScale(CURRENT_SELECT_UI_SCALE);
	//現在何を選択しているかを表すUIの更新処理
	m_currentSelectUI.Update();
}

//現在何を選択しているかを表すUIの更新処理
void GameEndSelect::CurrentSelectUIUpdate(EnGameEndSelect enGameEndSelect)
{
	if (m_isSelect)
	{
		//現在何を選択しているかを表すUIのアニメーションの更新処理
		CurrentSelectUIAnimationUpdate(ANIMATION_SPEED_SELECT);
	}
	else
	{
		//現在何を選択しているかを表すUIのアニメーションの更新処理
		CurrentSelectUIAnimationUpdate(ANIMATION_SPEED_NORMAL);
	}
	//現在何を選択しているかを表すUIの座標の設定
	m_currentSelectUI.SetPosition(m_currentSelectUIPosition[enGameEndSelect]);
	//現在何を選択しているかを表すUIの更新処理
	m_currentSelectUI.Update();
}

//現在何を選択しているかを表すUIのアニメーションの更新処理
void GameEndSelect::CurrentSelectUIAnimationUpdate(float speed)
{
	m_time += speed * g_gameTime->GetFrameDeltaTime();

	if (int(m_time) % 2 == 0)
	{
		//選択している　かつ
		//現在何を選択しているかを表すUIを描画していないとき
		if (m_isSelect)
		{
			if (!m_isDrawingCurrentSelectUI)
			{
				m_noDrawingCurrentSelectUICount++;
			}
		}

		m_isDrawingCurrentSelectUI = true;
		return;
	}
	m_isDrawingCurrentSelectUI = false;
}

//入力の更新処理
void GameEndSelect::InputUpdate()
{
	//十字キーを上に倒す または
	//左スティックを上に倒したとき
	if (g_pad[0]->IsTrigger(enButtonUp) || g_pad[0]->IsTriggerLStickUp())
	{
		if (m_currentSelect == enGameEndSelect_Retry)
		{
			m_currentSelect = enGameEndSelect_ReturnTitle;
			return;
		}

		m_currentSelect -= SELECT_UP;//上に選択
	}
	//十字キーを下に倒す または
	//左スティックを下に倒したとき
	else if (g_pad[0]->IsTrigger(enButtonDown) || g_pad[0]->IsTriggerLStickDown())
	{
		if (m_currentSelect == enGameEndSelect_ReturnTitle)
		{
			m_currentSelect = enGameEndSelect_Retry;
			return;
		}

		m_currentSelect += SELECT_UP;//下に選択
	}

	//Aボタンを押したとき
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_isSelect = true;
	}
}

//シーンの遷移処理
void GameEndSelect::TransitionScene()
{
	//現在の選択
	FadeManager::GetInstance()->SetFadeState(FadeManager::enFadeState_FadeOut);
	switch (m_currentSelect)
	{
	case EnGameEndSelect::enGameEndSelect_Retry:
		if (FadeManager::GetInstance()->IsFinishFade())
		{
			SceneManager::GetInstance()->CreateScene(SceneManager::enSceneID_InGame);//インゲームシーンの生成
			m_isTransitionScene = true;//シーンの遷移をする
		}
		break;
	case EnGameEndSelect::enGameEndSelect_ReturnTitle:
		if (FadeManager::GetInstance()->IsFinishFade())
		{
			SceneManager::GetInstance()->CreateScene(SceneManager::enSceneID_Title);//タイトルシーンの生成
			m_isTransitionScene = true;//シーンの遷移をする
		}
		break;
	default:
		break;
	}
}