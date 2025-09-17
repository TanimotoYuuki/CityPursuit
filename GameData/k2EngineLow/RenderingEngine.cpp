#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow
{
	//初期化
	void RenderingEngine::Init()
	{
		//メインレンダリングターゲットの初期化
		InitMainRenderTarget();

		//背景の初期化
		InitBackGround();

		//2D(フォントとスプライト)の初期化
		Init2DSprite();

		//メインレンダリングターゲットのカラーバッファの内容を
		//フレームバッファにコピーするためのスプライトを初期化する
		InitCopyMainRenderTargetToFrameBuffer();

		//ブルームの初期化
		m_bloom.Init(m_mainRenderTarget);

		//シャドウの初期化
		m_shadow.Init();

		//シーンライトの初期化
		m_sceneLight.Init();
	}

	//メインレンダリングターゲットの初期化
	void RenderingEngine::InitMainRenderTarget()
	{
		//メインレンダリングターゲットの作成
		float clearColor[4] = { 0.5f,0.5f,0.5f,1.0f };
		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}

	//背景の初期化
	void RenderingEngine::InitBackGround()
	{
		//背景用レンダリングターゲットの作成
		float clearColor[4] = { 0.0f,0.0f,0.0f,1.0f };
		m_backGroundRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN,
			clearColor
		);

		//最終合成用のスプライトを初期化する
		SpriteInitData spriteInitData;
		//テクスチャは背景用レンダリングターゲット
		spriteInitData.m_textures[0] = &m_backGroundRenderTarget.GetRenderTargetTexture();
		//解像度はメインレンダリングターゲットの幅と高さ
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();
		//背景用のシェーダーを使用する
		spriteInitData.m_fxFilePath = "Assets/shader/backGround.fx";
		spriteInitData.m_vsEntryPointFunc = "VSMain";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		//上書き
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
		//レンダリングターゲットのフォーマット
		spriteInitData.m_colorBufferFormat[0] = m_mainRenderTarget.GetColorBufferFormat();
		//初期化
		m_backGroundSprite.Init(spriteInitData);
	}

	//2D(フォントとスプライト)の初期化
	void RenderingEngine::Init2DSprite()
	{
		//2D用レンダリングターゲットの作成
		float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
		m_2DRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN,
			clearColor
		);

		//最終合成用のスプライトを初期化する
		SpriteInitData spriteInitData;
		//テクスチャは2Dレンダリングターゲット
		spriteInitData.m_textures[0] = &m_2DRenderTarget.GetRenderTargetTexture();
		//解像度はメインレンダリングターゲットの幅と高さ
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();
		//2D用のシェーダーを使用する
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_vsEntryPointFunc = "VSMain";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		//上書き
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
		//レンダリングターゲットのフォーマット
		spriteInitData.m_colorBufferFormat[0] = m_mainRenderTarget.GetColorBufferFormat();
		//初期化
		m_2DSprite.Init(spriteInitData);

		//テクスチャはメインレンダリングターゲット
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		//解像度は2Dレンダリングターゲットの幅と高さ
		spriteInitData.m_width = m_2DRenderTarget.GetWidth();
		spriteInitData.m_height = m_2DRenderTarget.GetHeight();
		//2D用のシェーダーを使用する
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		//上書き
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
		//レンダリングターゲットのフォーマット
		spriteInitData.m_colorBufferFormat[0] = m_2DRenderTarget.GetColorBufferFormat();
		//初期化
		m_mainSprite.Init(spriteInitData);
	}

	//メインレンダリングターゲットのカラーバッファの内容を
	//フレームバッファにコピーするためのスプライトを初期化する
	void RenderingEngine::InitCopyMainRenderTargetToFrameBuffer()
	{
		//スプライトの初期化
		SpriteInitData spriteInitData;
		//テクスチャはメインレンダリングターゲット
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		//解像度はメインレンダリングターゲットの幅と高さ
		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
		//2D用のシェーダーを使用する
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		//初期化
		m_copyToFrameBufferSprite.Init(spriteInitData);
	}

	//描画処理を実行
	void RenderingEngine::Execute(RenderContext& rc)
	{
		//ライトカメラの更新処理
		m_sceneLight.LightCameraUpdate();

		//シャドウの描画処理を実行
		m_shadow.Execute(rc, m_renderObjects);

		//背景の描画
		BackGroundDraw(rc);

		//モデルの描画
		ModelDraw(rc);

		//ブルームの描画処理を実行
		m_bloom.Execute(rc, m_mainRenderTarget);

		//エフェクトの描画
		EffectEngine::GetInstance()->Draw();

		//2D(フォントとスプライト)の描画
		SpriteFontDraw(rc);
		
		//メインレンダリングターゲットの内容をフレームバッファにコピーする
		CopyMainRenderTargetToFrameBuffer(rc);

		//レンダリングオブジェクトのクリア
		m_renderObjects.clear();
	}

	//背景の描画
	void RenderingEngine::BackGroundDraw(RenderContext& rc)
	{
		//レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_backGroundRenderTarget);
		//レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_backGroundRenderTarget);
		//レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_backGroundRenderTarget);

		for (auto renderObj : m_renderObjects)
		{
			//背景の描画
			renderObj->OnRenderBackGround(rc);
		}

		//レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_backGroundRenderTarget);

		//ターゲットをメインに戻す
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		//レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);
		//レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_mainRenderTarget);

		//背景用スプライトの描画
		m_backGroundSprite.Draw(rc);

		//メインレンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	}

	//モデルの描画
	void RenderingEngine::ModelDraw(RenderContext& rc)
	{
		//レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		//レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		for (auto renderObj : m_renderObjects)
		{
			//モデルの描画
			renderObj->OnRenderModel(rc);
		}

		//レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	}

	//2D(フォントとスプライト)の描画
	void RenderingEngine::SpriteFontDraw(RenderContext& rc)
	{
		//レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_2DRenderTarget);
		//レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_2DRenderTarget);
		//レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_2DRenderTarget);

		//メイン(モデル)用のスプライトの描画
		m_mainSprite.Draw(rc);

		for (auto renderObj : m_renderObjects)
		{
			//2D(フォントとスプライト)の描画
			renderObj->OnRender2D(rc);
		}

		//レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_2DRenderTarget);

		//ターゲットをメインに戻す
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		//レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		//2D(フォントとスプライト)の描画
		m_2DSprite.Draw(rc);

		//メインレンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	}

	//メインレンダリングターゲットの内容をフレームバッファにコピーする
	void RenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
	{
		//レンダリングターゲットの設定
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		//ビューポートとシザリング短形の設定
		rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

		//メインレンダリングターゲットをフレームバッファにコピーするためのスプライトの描画
		m_copyToFrameBufferSprite.Draw(rc);
	}
}