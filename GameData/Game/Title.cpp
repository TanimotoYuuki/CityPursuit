#include "stdafx.h"
#include "Title.h"
#include "FadeManager.h"
#include "TitleSprite.h"
#include "TitleSelect.h"

namespace {
	const float DELAY_TIME = 1.0f;//待機時間
}

//デストラクタ
Title::~Title()
{
	DeleteGO(m_titleSprite);
	DeleteGO(m_titleSelect);
}

//開始処理
bool Title::Start()
{
	m_titleSprite = NewGO<TitleSprite>(0, "titlesprite");
	m_titleSprite->EnableDrawingUI();

	m_titleSelect = NewGO<TitleSelect>(0, "titleselect");
	return true;
}

//更新処理
void Title::Update()
{
	//タイトルの状態
	switch (m_titleState)
	{
	case enTitleState_Fade://フェード
		FadeUpdate();
		break;
	case enTitleState_TitleText://タイトルのテキスト
		TitleTextUpdate();
		break;
	case enTitleState_Select://選択
		SelectUpdate();
		break;
	default:
		break;
	}
}

//フェードの更新処理
void Title::FadeUpdate()
{
	if (g_gameTime->StopWatch(DELAY_TIME))
	{
		FadeManager::GetInstance()->SetFadeState(FadeManager::enFadeState_FadeIn);
	}

	//フェード処理が終わったら次のステートに移行する
	if (FadeManager::GetInstance()->IsFinishFade())
	{
		m_titleState = enTitleState_TitleText;
	}
}

//タイトルのテキストの更新処理
void Title::TitleTextUpdate()
{
	//Aボタンを押したら次のステートに移行する
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_titleSprite->DisableDrawingUI();
		m_titleSelect->EnableDrawingUI();
		m_titleState = enTitleState_Select;
	}
}

//選択の更新処理
void Title::SelectUpdate()
{
	m_titleSelect->Execute();

	//シーンを遷移するならタイトルの削除処理する
	if (m_titleSelect->IsTransitionScene())
	{
		DeleteGO(this);
	}
}