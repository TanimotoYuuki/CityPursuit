#include "k2EngineLowPreCompile.h"
#include "SpriteRender.h"

namespace nsK2EngineLow
{
	//初期化
	void SpriteRender::Init(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode)
	{
		//スプライトの初期化
		SpriteInitData spriteInitData;
		//ddsファイルのファイルパスの指定
		spriteInitData.m_ddsFilePath[0] = filePath;
		//シェーダーファイルのファイルパスの指定
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		//幅と高さの設定
		spriteInitData.m_width = w;
		spriteInitData.m_height = h;
		//定数バッファの設定
		spriteInitData.m_expandConstantBuffer = &GetSpriteRenderConstantBuffer();
		spriteInitData.m_expandConstantBufferSize = sizeof(GetSpriteRenderConstantBuffer());
		//描画モードを指定
		spriteInitData.m_alphaBlendMode = alphaBlendMode;
		//初期化
		m_sprite.Init(spriteInitData);
	}

	//更新処理
	void SpriteRender::Update()
	{
		//リニアワイプの描画モードでLinearWipeDrawingMode_None以外選択されていたら処理する
		if (m_spriteRenderConstantBuffer.linearWipeDrawingMode != LinearWipeDrawingMode_None)
		{
			//リニアワイプの更新処理
			LinearWipeUpdate();
		}

		//画像加工の描画モードでScreenDrawingMode_None以外選択されていたら処理する
		if (m_spriteRenderConstantBuffer.screenDrawingMode != ScreenDrawingMode_None)
		{
			//画像処理の更新処理
			ScreenDrawingUpdate();
		}

		//スプライトの更新処理
		m_sprite.Update(m_position, m_rotation, m_scale, m_pivot);
	}

	//描画処理
	void SpriteRender::Draw(RenderContext& rc)
	{
		//レンダリングオブジェクトの追加
		g_renderingEngine->AddRenderObject(this);
	}
}