#include "stdafx.h"
#include "Loading.h"

Loading* Loading::m_instance = nullptr;//初期化

namespace {
	//ローディングUI
	const float LOADING_UI_WIDTH_AND_HEIGHT = 1024;//ローディングUIの横幅と縦幅

	const Vector3 LOADING_UI_POSITION{ 700.0f,-375.0f,0.0f };//ローディングUIの位置

	const Vector3 ICON_SCALE{ 0.09,0.09,0.09f };//アイコンの大きさ

	const Vector3 RING_SCALE{ 0.1f,0.1f,0.1f };//リングの大きさ

	const Quaternion LOADING_UI_INIT_ROTAITON = { 0.0f,0.0f,0.0f,1.0f };//ローディングUIの初期回転
}

bool Loading::Start()
{
	for (int i = 0; i < enLoadingStructure_Num; i++)
	{
		InitLoadingUI((EnLoadingStructure)i);
	}

	return true;
}

void Loading::Update()
{
	//ローディング中ではなければ処理しない
	if (!m_isLoading)
	{
		m_elapsedTime = 0.0f;
		m_loadingUI[enLoadingStructure_RingTwo].SetMulColor(Vector4{ 1.0f,1.0f,1.0f,1.0f });
		m_loadingUI[enLoadingStructure_RingTwo].Update();
		return;
	}

	//ローディングUIの回転の更新処理
	LoadingUIRotationUpdate();

	//ローディングUIの乗算カラーの更新処理
	LoadingUIMulColorUpdate();
}

void Loading::Render(RenderContext& rc)
{
	//ローディング中ではなければ描画しない
	if (!m_isLoading)
	{
		return;
	}

	for (int i = 0; i < enLoadingStructure_Num; i++)
	{
		m_loadingUI[i].Draw(rc);
	}
}

//ローディングUIの初期化
void Loading::InitLoadingUI(EnLoadingStructure enLoadingStructure)
{
	//ローディングUIの初期化
	m_loadingUI[enLoadingStructure].Init(m_loadingUIFilePath[enLoadingStructure].c_str(), LOADING_UI_WIDTH_AND_HEIGHT, LOADING_UI_WIDTH_AND_HEIGHT);

	//ローディングUIの位置の設定
	m_loadingUI[enLoadingStructure].SetPosition(LOADING_UI_POSITION);

	//ローディングUIの回転の設定
	m_loadingUI[enLoadingStructure].SetRotation(LOADING_UI_INIT_ROTAITON);

	if (enLoadingStructure != enLoadingStructure_Icon)
	{
		//ローディングUIの大きさの設定(リング)
		m_loadingUI[enLoadingStructure].SetScale(RING_SCALE);
	}
	else
	{
		//ローディングUIの大きさの設定(アイコン)
		m_loadingUI[enLoadingStructure].SetScale(ICON_SCALE);
	}

	//ローディングUIの更新処理
	m_loadingUI[enLoadingStructure].Update();
}

//ローディングUIの回転の更新処理
void Loading::LoadingUIRotationUpdate()
{
	//アイコンから一つ目と三つ目のリングを時計回りに回転
	m_rightRotation.AddRotationDegZ(-3.0f);

	m_loadingUI[enLoadingStructure_RingOne].SetRotation(m_rightRotation);
	m_loadingUI[enLoadingStructure_RingOne].Update();
	m_loadingUI[enLoadingStructure_RingThree].SetRotation(m_rightRotation);
	m_loadingUI[enLoadingStructure_RingThree].Update();

	//アイコンから三つ目のリングを反時計回りに回転
	m_leftRotation.AddRotationDegZ(4.0f);

	m_loadingUI[enLoadingStructure_RingTwo].SetRotation(m_leftRotation);
	m_loadingUI[enLoadingStructure_RingTwo].Update();
}

//ローディングUIの乗算カラーの更新処理
void Loading::LoadingUIMulColorUpdate()
{
	//cos波でアイコンから三つ目のリングを不透明から透明に変化
	m_elapsedTime += 3.0f * g_gameTime->GetFrameDeltaTime();
	float alpha = 1.2f + cos(m_elapsedTime);
	m_loadingUI[enLoadingStructure_RingTwo].SetMulColor(Vector4{ 1.0f,1.0f,1.0f,alpha });
	m_loadingUI[enLoadingStructure_RingTwo].Update();
}