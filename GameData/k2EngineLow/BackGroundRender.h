#pragma once

namespace nsK2EngineLow
{
	/// <summary>
	/// バックグラウンドレンダー
	/// </summary>
	class BackGroundRender : public IRenderer
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="w">画像の横幅</param>
		/// <param name="h">画像の縦幅</param>
		/// <param name="isScroll">trueならスクロールする</param>
		void Init(const char* filePath, const float w, const float h, bool isScroll = false);

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
		/// 更新処理
		/// </summary>
		void Update();

		/// <summary>
		/// 描画処理
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void Draw(RenderContext& rc);

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

		//バックグラウンドレンダー用の定数バッファ
		struct BackGroundRenderConstantBuffer
		{
			bool  scrollFlag = false;			//背景をスクロールするか?
			float screenEdgeCorrection = 0.0f;	//画像の画面端補正
			float rate = 0.0f;					//割合
			float texelSize = 0.0f;				//テクセルサイズ
		};

		/// <summary>
		/// 現在位置の設定
		/// </summary>
		/// <param name="pos">現在位置</param>
		void SetCurrentPosition(const Vector3& pos)
		{
			m_currentPosition = pos;
		}

		/// <summary>
		/// ゴール位置の設定
		/// </summary>
		/// <param name="pos">ゴール位置</param>
		void SetGoalPosition(const Vector3& pos)
		{
			m_goalPosition = pos;
		}

		/// <summary>
		/// 背景スクロールの進行割合を取得
		/// </summary>
		/// <returns>背景スクロールの進行割合</returns>
		float GetBackGroundScrollRate() const
		{
			return m_backGroundConstantBuffer.rate * 100.0f;
		}

		/// <summary>
		/// 背景スクロールの更新処理
		/// </summary>
		void UpdateBackGroundScroll()
		{
			m_backGroundConstantBuffer.rate = 
				m_currentPosition.x / m_goalPosition.x;

			if (m_backGroundConstantBuffer.rate > 1.0f)
			{
				m_backGroundConstantBuffer.rate = 1.0f;
			}
			else if (m_backGroundConstantBuffer.rate < 0.0f)
			{
				m_backGroundConstantBuffer.rate = 0.0f;
			}
		}

		/// <summary>
		/// テクセルサイズの設定
		/// </summary>
		/// <param name="texelSize">テクセルサイズ</param>
		void SetTexelSize(float texelSize)
		{
			m_backGroundConstantBuffer.texelSize = texelSize;
		}

		/// <summary>
		/// バックグラウンドレンダー用の定数バッファを取得
		/// </summary>
		/// <returns></returns>
		BackGroundRenderConstantBuffer& GetBackGroundRenderConstantBuffer()
		{
			return m_backGroundConstantBuffer;
		}

	private:

		/// <summary>
		/// 背景の描画
		/// </summary>
		/// <param name="rc"></param>
		void OnRenderBackGround(RenderContext& rc) override
		{
			m_sprite.Draw(rc);
		}

		Sprite  m_sprite;													//スプライト
		Vector3 m_position = Vector3::Zero;									//座標
		Quaternion m_rotation = Quaternion::Identity;						//回転
		Vector3 m_scale = Vector3::One;										//拡大率
		Vector2 m_pivot = Sprite::DEFAULT_PIVOT;							//ピボット
		Vector3 m_currentPosition = Vector3::Zero;							//現在位置
		Vector3 m_goalPosition = Vector3::Zero;								//ゴール位置
		BackGroundRenderConstantBuffer m_backGroundConstantBuffer;			//バックグラウンドレンダー用の定数バッファ
	};
}

