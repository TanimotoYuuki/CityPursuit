#include "k2EngineLowPreCompile.h"
#include "GameEndPostEffect.h"

namespace nsK2EngineLow
{
	//初期化
	void GameEndPostEffect::Init(RenderTarget& rt)
	{
		//ゲーム終了のときにするポストエフェクト用レンダリングターゲットの初期化
		InitGameEndPostEffectRenderTarget(rt);

		//ガウシアンブラーを初期化
		InitGaussianBlur();

		//ゲーム終了したときにするポストエフェクト用スプライトの設定
		SetGameEndPostEffectSprire(rt);

		//最終合成用スプライトの設定
		SetFinalSprite(rt);
	}

	//ゲーム終了のときにするポストエフェクト用のレンダリングターゲットの初期化
	void GameEndPostEffect::InitGameEndPostEffectRenderTarget(RenderTarget& rt)
	{
		//ゲーム終了のときにするポストエフェクト用レンダリングターゲットの作成
		m_gameEndPostEffectRenderTarget.Create(
			rt.GetWidth(),
			rt.GetHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}

	//ガウシアンブラーを初期化
	void GameEndPostEffect::InitGaussianBlur()
	{
		//初期化
		m_gaussianBlur.Init(&m_gameEndPostEffectRenderTarget.GetRenderTargetTexture());
	}

	//ボケ化スプライトの設定
	void GameEndPostEffect::SetGameEndPostEffectSprire(RenderTarget& rt)
	{
		//スプライトの初期化
		SpriteInitData bokeSpriteInitData;
		//ポストエフェクト用のシェーダーを使用する
		bokeSpriteInitData.m_fxFilePath = "Assets/shader/gameEndPostEffect.fx";
		bokeSpriteInitData.m_vsEntryPointFunc = "VSMain";
		bokeSpriteInitData.m_psEntryPoinFunc = "PSMain";
		//解像度はメインレンダリングターゲットの幅と高さ
		bokeSpriteInitData.m_width = rt.GetWidth();
		bokeSpriteInitData.m_height = rt.GetHeight();
		//テクスチャはメインレンダリングターゲット
		bokeSpriteInitData.m_textures[0] = &rt.GetRenderTargetTexture();
		bokeSpriteInitData.m_textures[1] = &m_gaussianBlur.GetBokeTexture();
		//レンダリングターゲットのフォーマット
		bokeSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		//定数バッファの設定
		bokeSpriteInitData.m_expandConstantBuffer = &GetGameEndPostEffectConstantBuffer();
		bokeSpriteInitData.m_expandConstantBufferSize = sizeof(GetGameEndPostEffectConstantBuffer());
		//初期化
		m_gameEndPostEffectSprite.Init(bokeSpriteInitData);
	}

	//最終合成用スプライトの設定
	void GameEndPostEffect::SetFinalSprite(RenderTarget& rt)
	{
		//スプライトの初期化
		SpriteInitData finalSpriteInitData;
		//テクスチャはガウシアンブラー
		finalSpriteInitData.m_textures[0] = &m_gameEndPostEffectRenderTarget.GetRenderTargetTexture();
		//解像度はメインレンダリングターゲットの幅と高さ
		finalSpriteInitData.m_width = rt.GetWidth();
		finalSpriteInitData.m_height = rt.GetHeight();
		//2D用のシェーダーを使用する
		finalSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		finalSpriteInitData.m_vsEntryPointFunc = "VSMain";
		finalSpriteInitData.m_psEntryPoinFunc = "PSMain";
		//加算描画
		finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
		//レンダリングターゲットのフォーマット
		finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		//初期化
		m_finalSprite.Init(finalSpriteInitData);
	}

	//描画処理を実行
	void GameEndPostEffect::Execute(RenderContext& rc, RenderTarget& rt)
	{
		//ポストエフェクトを描画しないなら処理しない
		if (m_gameEndPostEffectConstantBuffer.drawingPostEffect == enGameEndPostEffect_None)
		{
			m_gameEndPostEffectConstantBuffer.drawingRate = 0.0f;
			m_isFinishDrawingGameEndPostEffect = false;
			return;
		}

		if (!m_isFinishDrawingGameEndPostEffect)
		{
			m_gameEndPostEffectConstantBuffer.drawingRate += 2.0f * g_gameTime->GetFrameDeltaTime();

			if (m_gameEndPostEffectConstantBuffer.drawingRate > 0.99f)
			{
				m_gameEndPostEffectConstantBuffer.drawingRate = 0.99f;
				m_isFinishDrawingGameEndPostEffect = true;
			};
		}

		//レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_gameEndPostEffectRenderTarget);
		//レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_gameEndPostEffectRenderTarget);
		//レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_gameEndPostEffectRenderTarget);

		//ポストエフェクトの描画
		m_gameEndPostEffectSprite.Draw(rc);

		//レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_gameEndPostEffectRenderTarget);

		m_gaussianBlur.ExecuteOnGPU(rc, 50.0f);

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