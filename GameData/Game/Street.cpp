#include "stdafx.h"
#include "Street.h"

//開始処理
bool Street::Start()
{
	//道モデルの初期化
	m_streetModel.Init("Assets/modelData/street/street.tkm");

	//道モデルの位置設定
	m_streetModel.SetPosition(m_position);

	//道モデルの回転設定
	m_streetModel.SetRotation(m_rotation);

	//道モデルの大きさ設定
	m_streetModel.SetScale(m_scale);

	//道モデルの更新
	m_streetModel.Update();
	return true;
}

//更新処理
void Street::Update()
{
	//道モデルの更新
	m_streetModel.Update();
}

//描画処理
void Street::Render(RenderContext& rc)
{
	//道モデルの描画
	m_streetModel.Draw(rc);
}