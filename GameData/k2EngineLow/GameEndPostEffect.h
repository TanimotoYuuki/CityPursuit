#pragma once
namespace nsK2EngineLow {
	/// <summary>
	/// ゲーム終了のときにするポストエフェクト
	/// </summary>
	class GameEndPostEffect : public Noncopyable
	{
	private://定数バッファ

		//ゲーム終了のときにするポストエフェクト用の定数バッファ
		struct GameEndPostEffectConstantBuffer
		{
			float drawingRate = 0.0f;//ポストエフェクト用イージング割合
			int drawingPostEffect = enGameEndPostEffect_None;//描画するポストエフェクト
		};

	public://列挙型

		//ゲーム終了のときにするポストエフェクト
		enum EnGameEndPostEffect
		{
			enGameEndPostEffect_Monochrome,//モノクロ化
			enGameEndPostEffect_Boke,//ボケ化
			enGameEndPostEffect_None//ポストエフェクト数
		};

	public:
		/// <summary>
		/// 初期化
		/// </summary>
		void Init(RenderTarget& rt);

		/// <summary>
		/// 描画処理を実行
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		/// <param name="rt">レンダリングターゲット</param>
		void Execute(RenderContext& rc, RenderTarget& rt);

		/// <summary>
		/// ゲーム終了のときに描画するポストエフェクトの設定
		/// </summary>
		/// <param name="enGameEndPostEffect">ゲーム終了のときに描画するポストエフェクト</param>
		void SetDrawingGameEndPostEffect(EnGameEndPostEffect enGameEndPostEffect)
		{
			m_gameEndPostEffectConstantBuffer.drawingPostEffect = enGameEndPostEffect;
		}

		/// <summary>
		/// ゲーム終了のときにするポストエフェクトの描画が終わったか?
		/// </summary>
		/// <returns>trueならゲーム終了のときにするポストエフェクトの描画が終わっている</returns>
		bool IsFinishDrawingGameEndPostEffect() const
		{
			return m_isFinishDrawingGameEndPostEffect;
		}

	private://メンバ関数

		/// <summary>
		/// ゲーム終了のときにするポストエフェクト用のレンダリングターゲットの初期化
		/// </summary>
		/// <param name="rc">レンダリングターゲット</param>
		void InitGameEndPostEffectRenderTarget(RenderTarget& rt);

		/// <summary>
		/// ゲーム終了したときにするポストエフェクト用スプライトの設定
		/// </summary>
		/// <param name="rt">レンダリングターゲット</param>
		void SetGameEndPostEffectSprire(RenderTarget& rt);

		/// <summary>
		/// ガウシアンブラーを初期化
		/// </summary>
		void InitGaussianBlur();

		/// <summary>
		/// 最終合成用スプライトの設定
		/// </summary>
		/// <param name="rt">レンダリングターゲット</param>
		void SetFinalSprite(RenderTarget& rt);

		/// <summary>
		/// ゲーム終了のときにするポストエフェクトの定数バッファの取得
		/// </summary>
		/// <returns>ゲーム終了のときにするポストエフェクトの定数バッファ</returns>
		GameEndPostEffectConstantBuffer& GetGameEndPostEffectConstantBuffer()
		{
			return m_gameEndPostEffectConstantBuffer;
		}

	private://メンバ変数
		GameEndPostEffectConstantBuffer m_gameEndPostEffectConstantBuffer;//ゲーム終了のときに描画するポストエフェクトの定数バッファ
		RenderTarget m_gameEndPostEffectRenderTarget;//ゲーム終了のときに描画するポストエフェクト用レンダリングターゲット
		GaussianBlur m_gaussianBlur;//ガウシアンブラー
		Sprite m_gameEndPostEffectSprite;//ゲーム終了したときにするポストエフェクト用スプライト
		Sprite m_finalSprite;//最終合成用スプライト
		bool m_isFinishDrawingGameEndPostEffect = false;//ゲーム終了のときにするポストエフェクトの描画が終わったか?
	};
}

