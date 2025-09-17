#include "k2EngineLowPreCompile.h"
#include "Shadow.h"

namespace nsK2EngineLow
{
	//初期化
	void Shadow::Init()
	{
		//シャドウマップ用のレンダリングターゲットの作成
		float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
		m_shadowMap.Create(
			1024,
			1024,
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}

	//描画処理を実行
	void Shadow::Execute(RenderContext& rc, std::vector<IRenderer*>& ro)
	{
		//レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
		//レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_shadowMap);
		//レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_shadowMap);

		for (auto& renderObj : ro)
		{
			//シャドウマップの描画
			renderObj->OnRenderShadowMap(rc, g_renderingEngine->GetLightCamera());
		}

		//レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);
	}
}