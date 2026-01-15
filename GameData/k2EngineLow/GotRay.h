#pragma once
namespace nsK2EngineLow {
	/// <summary>
	/// ゴッドレイ
	/// </summary>
	class GotRay : public Noncopyable
	{
	private://定数バッファ

		//ゴッドレイ用定数バッファ
		struct GodRayConstantBuffer
		{
			float strength;//ブラーの強さ
			Vector2 center;//ブラーの中心座標
			float pad0;//パディング
		};

	private://メンバ関数
		/// <summary>
		/// レンダリングターゲットの初期化
		/// </summary>
		/// <param name="rt">レンダリングターゲット</param>
		void InitRenderTarget(RenderTarget& rt);

		/// <summary>
		/// ゴッドレイ用スプライトの設定
		/// </summary>
		/// <param name="rt">レンダリングターゲット</param>
		void SetGodRaySprite(RenderTarget& rt);

		/// <summary>
		/// 最終合成用スプライトの設定
		/// </summary>
		/// <param name="rt">レンダリングターゲット</param>
		void SetFinalSprite(RenderTarget& rt);

	public://メンバ関数

		/// <summary>
		/// 初期化
		/// </summary>
		void Init(RenderTarget& mainRt, RenderTarget& luminnceRt);

		/// <summary>
		/// 実行処理
		/// </summary>
		void Execute(RenderContext& rc, RenderTarget& rt);

		/// <summary>
		/// ゴッドレイ用ブラーの強さの設定
		/// </summary>
		/// <param name="strength">ブラーの強さ</param>
		void SetGodRayBlurStrength(float strength)
		{
			m_godRayConstantBuffer.strength = strength;
		}

		/// <summary>
		/// ゴッドレイ用ブラーの中心の設定
		/// </summary>
		/// <param name="center">ズームブラーの中心座標</param>
		void SetGodRayBlurCenter(Vector2 center)
		{
			m_godRayConstantBuffer.center = center;
		}

		/// <summary>
		/// ゴッドレイ用定数バッファの取得
		/// </summary>
		/// <returns>ゴッドレイ用定数バッファ</returns>
		GodRayConstantBuffer& GetGodRayConstantBuffer()
		{
			return m_godRayConstantBuffer;
		}

	private://メンバ変数
		RenderTarget m_godRayRenderTarget;//ゴッドレイ用レンダリングターゲット
		Sprite m_godRaySprite;//ゴッドレイ用スプライト
		Sprite m_finalSprite;//最終合成用スプライト
		GodRayConstantBuffer m_godRayConstantBuffer;//ゴッドレイ用の定数バッファ
	};
}

