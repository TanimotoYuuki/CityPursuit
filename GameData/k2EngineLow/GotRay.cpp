#include "k2EngineLowPreCompile.h"
#include "GotRay.h"

namespace nsK2EngineLow
{
	//初期化
	void GotRay::Init(RenderTarget& mainRt, RenderTarget& luminnceRt)
	{
		//レンダリングターゲットの初期化
		InitRenderTarget(mainRt);

		//ゴッドレイ用スプライトの設定
		SetGodRaySprite(luminnceRt);

		//最終合成用スプライトの設定
		SetFinalSprite(mainRt);
	}

	//レンダリングターゲットの初期化
	void GotRay::InitRenderTarget(RenderTarget& rt)
	{
		//レンダリングターゲットの作成
		m_godRayRenderTarget.Create(
			rt.GetWidth(),
			rt.GetHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}

	//ゴッドレイ用スプライトの設定
	void GotRay::SetGodRaySprite(RenderTarget& rt)
	{
		//スプライトの初期化
		SpriteInitData bokeSpriteInitData;
		//ポストエフェクト用のシェーダーを使用する
		bokeSpriteInitData.m_fxFilePath = "Assets/shader/godRay.fx";
		bokeSpriteInitData.m_vsEntryPointFunc = "VSMain";
		bokeSpriteInitData.m_psEntryPoinFunc = "PSMain";
		//解像度はメインレンダリングターゲットの幅と高さ
		bokeSpriteInitData.m_width = rt.GetWidth();
		bokeSpriteInitData.m_height = rt.GetHeight();
		//テクスチャはメインレンダリングターゲット
		bokeSpriteInitData.m_textures[0] = &rt.GetRenderTargetTexture();
		//レンダリングターゲットのフォーマット
		bokeSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		//定数バッファの設定
		bokeSpriteInitData.m_expandConstantBuffer = &(GetGodRayConstantBuffer());
		bokeSpriteInitData.m_expandConstantBufferSize = sizeof(GetGodRayConstantBuffer());
		//初期化
		m_godRaySprite.Init(bokeSpriteInitData);
	}

	//最終合成用スプライトの設定
	void GotRay::SetFinalSprite(RenderTarget& rt)
	{
		//スプライトの初期化
		SpriteInitData finalSpriteInitData;
		//テクスチャはガウシアンブラー
		finalSpriteInitData.m_textures[0] = &m_godRayRenderTarget.GetRenderTargetTexture();
		//解像度はメインレンダリングターゲットの幅と高さ
		finalSpriteInitData.m_width = rt.GetWidth();
		finalSpriteInitData.m_height = rt.GetHeight();
		//2D用のシェーダーを使用する
		finalSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		finalSpriteInitData.m_vsEntryPointFunc = "VSMain";
		finalSpriteInitData.m_psEntryPoinFunc = "PSMain";
		//加算描画
		finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
		//レンダリングターゲットのフォーマット
		finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		//初期化
		m_finalSprite.Init(finalSpriteInitData);
	}

	//実行処理
	void GotRay::Execute(RenderContext& rc, RenderTarget& rt)
	{
		//レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_godRayRenderTarget);
		//レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_godRayRenderTarget);
		//レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_godRayRenderTarget);

		//ポストエフェクトの描画
		m_godRaySprite.Draw(rc);

		//レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_godRayRenderTarget);

		//ターゲットをメインに戻す
		rc.WaitUntilToPossibleSetRenderTarget(rt);
		//レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(rt);

		//最終合成用スプライトの描画
		m_finalSprite.Draw(rc);

		//レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(rt);
	}
}