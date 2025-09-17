#include "k2EngineLowPreCompile.h"
#include "BackGroundRender.h"

namespace nsK2EngineLow
{
	//初期化
	void BackGroundRender::Init(const char* filePath, const float w, const float h, bool isScroll)
	{
		//スプライトの初期化
		SpriteInitData spriteInitData;
		//ddsファイルのファイルパスを指定
		spriteInitData.m_ddsFilePath[0] = filePath;
		//シェーダーファイルのファイルパスを指定
		spriteInitData.m_fxFilePath = "Assets/shader/backGround.fx";
		//幅と高さの設定
		spriteInitData.m_width = w;
		spriteInitData.m_height = h;

		//定数バッファの設定
		spriteInitData.m_expandConstantBuffer = &GetBackGroundRenderConstantBuffer();
		spriteInitData.m_expandConstantBufferSize = sizeof(GetBackGroundRenderConstantBuffer());

		//描画モードの指定
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

		//背景をスクロールするかの指定
		m_backGroundConstantBuffer.scrollFlag = isScroll;

		//背景をスクロールするなら
		if (m_backGroundConstantBuffer.scrollFlag == true)
		{
			//幅がフレームバッファ(1600)より大きく設定していたら
			if (w > FRAME_BUFFER_W)
			{
				//画面端の補正を行う
				m_backGroundConstantBuffer.screenEdgeCorrection = w / FRAME_BUFFER_W - 1;
			}
		}

		//初期化
		m_sprite.Init(spriteInitData);
	}

	//更新処理
	void BackGroundRender::Update()
	{
		//背景をスクロールするなら
		if (m_backGroundConstantBuffer.scrollFlag == true)
		{
			//背景スクロールの更新処理
			UpdateBackGroundScroll();
		}

		//スプライトの更新処理
		m_sprite.Update(m_position, m_rotation, m_scale, m_pivot);
	}

	//描画処理
	void BackGroundRender::Draw(RenderContext& rc)
	{
		//レンダリングオブジェクトの追加
		g_renderingEngine->AddRenderObject(this);
	}
}