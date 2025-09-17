#pragma once
namespace nsK2EngineLow
{
	//リニアワイプ描画モード
	enum LinearWipeDrawingMode
	{
		LinearWipeDrawingMode_Normal,		//通常ワイプ
		LinearWipeDrawingMode_Direction,	//方向ワイプ
		LinearWipeDrawingMode_Round,		//円形ワイプ
		LinearWipeDrawingMode_Vertical,		//縦じまワイプ
		LinearWipeDrawingMode_Horizontal,	//横じまワイプ
		LinearWipeDrawingMode_CheckerBoard, //チェッカーボードワイプ
		LinearWipeDrawingMode_None			//描画しない
	};

	//画像加工描画モード
	enum ScreenDrawingMode
	{
		ScreenDrawingMode_Monochrome,	//モノクロ
		ScreenDrawingMode_Sepia,		//セピア
		ScreenDrawingMode_Nega,			//ネガポジ反転
		ScreenDrawingMode_Noise,		//ノイズ
		ScreenDrawingMode_None,			//描画しない
	};

	//フェードステート
	enum EnFadeState
	{
		enFadeState_FadeIn,		//フェードイン
		enFadeState_Loading,	//ローディング
		enFadeState_FadeOut,	//フェードアウト
		enFadeState_Num			//フェード数
	};

	/// <summary>
	/// スプライトレンダー
	/// </summary>
	class SpriteRender : public IRenderer
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="w">画像の横幅</param>
		/// <param name="h">画像の縦幅</param>
		void Init(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode = AlphaBlendMode_Trans);

		/// <summary>
		/// 座標を設定
		/// </summary>
		/// <param name="pos">座標</param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}

		/// <summary>
		/// 座標を取得
		/// </summary>
		/// <returns>座標</returns>
		const Vector3& GetPosition() const
		{
			return m_position;
		}

		/// <summary>
		/// 大きさを設定
		/// </summary>
		/// <param name="scale">大きさ</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}

		/// <summary>
		/// 大きさを取得
		/// </summary>
		/// <returns>大きさ</returns>
		const Vector3& GetScale() const
		{
			return m_scale;
		}

		/// <summary>
		/// 回転を設定
		/// </summary>
		/// <param name="rot">回転</param>
		void SetRotation(const Quaternion& rot)
		{
			m_rotation = rot;
		}

		/// <summary>
		/// 回転を取得
		/// </summary>
		/// <returns>回転</returns>
		const Quaternion& GetRotation() const
		{
			return m_rotation;
		}

		/// <summary>
		/// ピボットを設定
		/// </summary>
		/// <param name="pivot">ピボット</param>
		void SetPivot(const Vector2& pivot)
		{
			m_pivot = pivot;
		}

		/// <summary>
		/// ピボットを取得
		/// </summary>
		/// <returns>ピボット</returns>
		const Vector2& GetPivot() const
		{
			return m_pivot;
		}

		/// <summary>
		/// 乗算カラーを設定
		/// </summary>
		/// <param name="mulColor">乗算カラー</param>
		void SetMulColor(const Vector4& mulColor)
		{
			m_sprite.SetMulColor(mulColor);
		}

		/// <summary>
		/// 乗算カラーを取得
		/// </summary>
		/// <returns>乗算カラー</returns>
		const Vector4& GetMulColor() const
		{
			return m_sprite.GetMulColor();
		}

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();
		
		/// <summary>
		/// 描画処理
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void Draw(RenderContext& rc);
		
		struct LinearWipe
		{
			Vector2 direction; //方向
			float size = 0.0f; //ワイプサイズ
		};

		//スプライトレンダー用の定数バッファ
		struct SpriteRenderConstantBuffer
		{
			LinearWipe linearWipe;									//リニアワイプ
			int linearWipeDrawingMode = LinearWipeDrawingMode_None; //リニアワイプの描画モード
			float drawingRate = 0.0f;								//画像加工用イージング割合
			int screenDrawingMode = ScreenDrawingMode_None;			//画像加工の描画モード
		};

		/// <summary>
		///	リニアワイプの描画モードを設定
		/// </summary>
		/// <param name="linearWipeMode">描画モード　LinearWipeMode_Directionを設定する場合はSetLinearWipeDirection()で方向を設定して下さい</param>
		void SetLinearWipeDrawingMode(LinearWipeDrawingMode linearWipeMode)
		{
			m_spriteRenderConstantBuffer.linearWipeDrawingMode = linearWipeMode;
			m_spriteRenderConstantBuffer.linearWipe.size = 0.0f;
		}

		/// <summary>
		/// ワイプサイズを設定
		/// </summary>
		/// <param name="wipeSize">ワイプサイズ</param>
		void SetWipeSize(float wipeSize)
		{
			m_spriteRenderConstantBuffer.linearWipe.size = wipeSize;
		}

		/// <summary>
		/// ワイプサイズの取得
		/// </summary>
		/// <returns>ワイプサイズ</returns>
		float GetWipeSize() const
		{
			return m_spriteRenderConstantBuffer.linearWipe.size;
		}

		/// <summary>
		/// リニアワイプの速度を設定
		/// </summary>
		/// <param name="wipeScroolSpeed">速度</param>
		void SetWipeScrollSpeed(float wipeScroolSpeed)
		{
			m_wipeScrollSpeed = wipeScroolSpeed;
		}

		/// <summary>
		/// リニアワイプ(方向)
		/// </summary>
		/// <param name="x">x軸方向(1.0f~0.0f)</param>
		/// <param name="y">y軸方向(1.0f~0.0f)</param>
		void SetLinearWipeDirection(float x, float y)
		{
			m_spriteRenderConstantBuffer.linearWipe.direction.Set(x, y);
			m_spriteRenderConstantBuffer.linearWipe.direction.Normalize();
		}

		/// <summary>
		/// 画像加工の描画モードを設定
		/// </summary>
		/// <param name="screenMode">描画モード</param>
		void SetScreenDrawingMode(ScreenDrawingMode screenDrawingMode)
		{
			m_spriteRenderConstantBuffer.screenDrawingMode = screenDrawingMode;
			m_spriteRenderConstantBuffer.drawingRate = 0.0f;
		}

		/// <summary>
		/// 画像加工をイージングする速度を設定
		/// </summary>
		/// <param name="screenDrawingEasingSpeed">速度</param>
		void SetScreenDrawingEasingSpeed(float screenDrawingSpeed)
		{
			m_screenDrawingEasingSpeed = screenDrawingSpeed;
		}

		/// <summary>
		/// フェードステートの切り替え
		/// </summary>
		/// <param name="enFadeState">フェードステート</param>
		void SetFadeTransition(EnFadeState enFadeState)
		{
			m_fadeState = enFadeState;
		}

		/// <summary>
		/// スプライトレンダー用の定数バッファを取得
		/// </summary>
		/// <returns></returns>
		SpriteRenderConstantBuffer& GetSpriteRenderConstantBuffer()
		{
			return m_spriteRenderConstantBuffer;
		}

	private:
		/// <summary>
		/// リニアワイプの更新処理
		/// </summary>
		void LinearWipeUpdate()
		{
			//フェードステートがフェードインならワイプサイズを大きくする
			if (m_fadeState == enFadeState_FadeIn)
			{
				m_spriteRenderConstantBuffer.linearWipe.size += m_wipeScrollSpeed * g_gameTime->GetFrameDeltaTime();
			}
			//フェードステートがフェードアウトならワイプサイズを小さくする
			else if (m_fadeState == enFadeState_FadeOut)
			{
				m_spriteRenderConstantBuffer.linearWipe.size -= m_wipeScrollSpeed * g_gameTime->GetFrameDeltaTime();
			}
		}

		/// <summary>
		/// 画像加工の更新処理
		/// </summary>
		void ScreenDrawingUpdate()
		{
			m_spriteRenderConstantBuffer.drawingRate += m_screenDrawingEasingSpeed * g_gameTime->GetFrameDeltaTime();
			//割合が1.0f以上になったら割合を固定する
			if (m_spriteRenderConstantBuffer.drawingRate > 1.0f)
			{
				m_spriteRenderConstantBuffer.drawingRate = 1.0f;
			}
		}

		/// <summary>
		///	2Dの描画
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void OnRender2D(RenderContext& rc) override
		{
			m_sprite.Draw(rc);
		}

		Sprite m_sprite;												//スプライト
		Vector3 m_position = Vector3::Zero;								//座標
		Quaternion m_rotation = Quaternion::Identity;					//回転
		Vector3 m_scale = Vector3::One;									//拡大率
		Vector2 m_pivot = Sprite::DEFAULT_PIVOT;						//ピボット
		SpriteRenderConstantBuffer m_spriteRenderConstantBuffer;		//リニアワイプ
		float m_wipeScrollSpeed = 1.0f;									//ワイプ速度
		float m_screenDrawingEasingSpeed = 0.01f;						//画像加工用のイージング速度
		int m_fadeState = enFadeState_FadeIn;							//フェードステート
	};
}

