#pragma once

#include "SceneLight.h"
#include "IRenderer.h"
#include "Bloom.h"
#include "Shadow.h"

namespace nsK2EngineLow
{
	/// <summary>
	/// レンダリングエンジン
	/// </summary>
	class RenderingEngine : public Noncopyable
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		void Init();

		/// <summary>
		/// 描画処理を実行
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void Execute(RenderContext& rc);

		/// <summary>
		/// レンダリングオブジェクトの追加
		/// </summary>
		/// <param name="renderObject">レンダリングオブジェクト</param>
		void AddRenderObject(IRenderer* renderObject)
		{
			m_renderObjects.push_back(renderObject);
		}

		/// <summary>
		/// ディレクションライトを設定する
		/// </summary>
		/// <param name="direction">ライトの方向</param>
		/// <param name="color">ライトの色</param>
		void SetDirectionLight(const Vector3& direction, const Vector3& color)
		{
			m_sceneLight.SetDirectionLight(direction, color);
		}

		/// <summary>
		/// 環境光を設定する
		/// </summary>
		/// <param name="ambient">環境光</param>
		void SetAmbient(const Vector3& ambient)
		{
			m_sceneLight.SetAmbient(ambient);
		}

		/// <summary>
		/// ポイントライトを設定する
		/// </summary>
		/// <param name="num">ライト番号</param>
		/// <param name="position">ライトの位置</param>
		/// <param name="color">ライトのカラー</param>
		/// <param name="range">ライトの影響範囲</param>
		void SetPointLight(int num, const Vector3& position, const Vector3& color, float range)
		{
			m_sceneLight.SetPointLight(num, position, color, range);
		}

		/// <summary>
		/// スポットライトを設定する
		/// </summary>
		/// <param name="num">ライト番号</param>
		/// <param name="position">ライトの位置</param>
		/// <param name="color">ライトのカラー</param>
		/// <param name="range">ライトの影響範囲</param>
		/// <param name="direction">ライトの放射方向</param>
		/// <param name="angle">ライトの放射角度</param>
		void SetSpotLight(int num, const Vector3& position, const Vector3& color, float range, const Vector3& direction, float angle)
		{
			m_sceneLight.SetSpotLight(num, position, color, range, direction, angle);
		}

		/// <summary>
		/// 半球ライトを設定する
		/// </summary>
		/// <param name="groundColor">地面色</param>
		/// <param name="skyColor">天球色</param>
		/// <param name="groundNormal">地面の法線</param>
		void SetHemLight(const Vector3& groundColor, const Vector3& skyColor, const Vector3& groundNormal)
		{
			m_sceneLight.SetHemLight(groundColor, skyColor, groundNormal);
		}

		/// <summary>
		/// ライトカメラの注視点を設定する
		/// </summary>
		/// <param name="target">注視点</param>
		void SetLightCameraTarget(const Vector3& target)
		{
			m_sceneLight.SetLightCameraTarget(target);
		}

		/// <summary>
		/// シャドウマップを取得
		/// </summary>
		/// <returns></returns>
		RenderTarget& GetShadow()
		{
			return m_shadow.GetRenderTarget();
		}

		/// <summary>
		/// ライトを取得
		/// </summary>
		/// <returns></returns>
		Light& GetLight()
		{
			return m_sceneLight.GetLight();
		}

		/// <summary>
		/// ライトカメラを取得
		/// </summary>
		Camera& GetLightCamera()
		{
			return m_sceneLight.GetLightCamera();
		}

	private:

		/// <summary>
		/// メインレンダリングターゲットの初期化
		/// </summary>
		void InitMainRenderTarget();

		/// <summary>
		/// 背景の初期化
		/// </summary>
		void InitBackGround();

		/// <summary>
		/// 2D(フォントとスプライト)の初期化
		/// </summary>
		void Init2DSprite();

		/// <summary>
		/// メインレンダリングターゲットのカラーバッファの内容を
		/// フレームバッファにコピーするためのスプライトを初期化する
		/// </summary>
		void InitCopyMainRenderTargetToFrameBuffer();

		/// <summary>
		/// 背景の描画
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void BackGroundDraw(RenderContext& rc);

		/// <summary>
		/// モデルの描画
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void ModelDraw(RenderContext& rc);

		/// <summary>
		/// 2D(フォントとスプライト)の描画
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void SpriteFontDraw(RenderContext& rc);

		/// <summary>
		/// メインレンダリングターゲットの内容をフレームバッファにコピーする
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);

		SceneLight m_sceneLight;						//シーンライト
		RenderTarget m_mainRenderTarget;				//メインレンダリングターゲット
		RenderTarget m_backGroundRenderTarget;			//背景用レンダリングターゲット
		RenderTarget m_2DRenderTarget;					//2D用レンダリングターゲット
		Sprite m_backGroundSprite;						//背景用スプライト
		Sprite m_2DSprite;								//2D(フォントとスプライト)用スクリプト
		Sprite m_mainSprite;							//メイン(モデル)用スプライト
		Sprite m_copyToFrameBufferSprite;				//メインレンダリングターゲットをフレームバッファにコピーするためのスプライト
		Bloom m_bloom;									//ブルーム
		Shadow m_shadow;								//シャドウマップ
		std::vector< IRenderer* > m_renderObjects;		//レンダリングオブジェクトの格納
	};
}

