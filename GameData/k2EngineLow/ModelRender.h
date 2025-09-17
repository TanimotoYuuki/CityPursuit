#pragma once
namespace nsK2EngineLow
{
	/// <summary>
	/// モデルレンダー
	/// </summary>
	class ModelRender : public IRenderer
	{
	public:
		/// <summary>
		/// 通常描画用の初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="animationClips">アニメーションクリップ</param>
		/// <param name="numAnimationClips">アニメーションクリップの数</param>
		/// <param name="enModelUpAxis">モデルの上方向</param>
		/// <param name="isShadowCaster">trueなら影を描画する</param>
		/// <param name="isShadowReceiver">trueなら影を受ける</param>
		void Init(const char* filePath,
			AnimationClip* animationClips = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool isShadowCaster = false,
			bool isShadowReceiver = false,
			AlphaBlendMode alphaBlendMode = AlphaBlendMode_None);

		/// <summary>
		/// 半透明描画用の初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="animationClips">アニメーションクリップ</param>
		/// <param name="numAnimationClips">アニメーションクリップの数</param>
		/// <param name="enModelUpAxis">モデルの上方向</param>
		/// <param name="isShadowCaster">trueなら影を描画する</param>
		/// <param name="isShadowReceiver">trueなら影を受ける</param>
		void IniTranslucent(const char* filePath,
			AnimationClip* animationClips = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool isShadowCaster = false,
			bool isShadowReceiver = false,
			AlphaBlendMode alphaBlendMode = AlphaBlendMode_Trans);

		/// <summary>
		/// スカイキューブを初期化
		/// </summary>
		void InitSkyCube(ModelInitData& initData);

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
		/// アニメーション再生
		/// </summary>
		/// <param name="animNo">アニメーションクリップの番号</param>
		/// <param name="interpolateTime">補完時間(単位:秒)</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}

		/// <summary>
		/// アニメーションの再生中？
		/// </summary>
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}

		/// <summary>
		/// モデルを取得
		/// </summary>
		Model& GetModel()
		{
			return m_model;
		}

		/// <summary>
		/// 座標、回転、拡大を全て更新
		/// </summary>
		/// <param name="pos">座標</param>
		/// <param name="rotation">回転</param>
		/// <param name="scale">拡大</param>
		void SetTRS(const Vector3& pos, const Quaternion& rotation, const Vector3& scale)
		{
			SetPosition(pos);
			SetRotation(rotation);
			SetScale(scale);
		}

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
		/// 回転を設定
		/// </summary>
		/// <param name="rotation">回転</param>
		void SetRotation(const Quaternion& rotation)
		{
			m_rotation = rotation;
		}

		/// <summary>
		/// 回転を取得
		/// </summary>
		/// <returns>回転</returns>
		const Quaternion GetRotation() const
		{
			return m_rotation;
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
		const Vector3 GetScale() const
		{
			return m_scale;
		}

		/// <summary>
		/// アニメーションイベントの追加
		/// </summary>
		/// <param name="eventListener">アニメーションイベント</param>
		void AddAnimationEvent(AnimationEventListener eventListener)
		{
			m_animation.AddAnimationEventListener(eventListener);
		}

		/// <summary>
		/// アニメーションの速度を設定
		/// </summary>
		/// <param name="animationSpeed">アニメーションの速度</param>
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}

		/// <summary>
		/// ボーンの名前からボーン番号を検索
		/// </summary>
		/// <param name="boneName">ボーンの名前</param>
		/// <returns>ボーン番号。見つからなかった場合は-1が返ってきます</returns>
		int FindBoneID(const wchar_t* boneName) const
		{
			return m_skeleton.FindBoneID(boneName);
		}

		Bone* GetBone(int boneNo) const
		{
			return m_skeleton.GetBone(boneNo);
		}

		/// <summary>
		/// 透明度を設定
		/// </summary>
		/// <param name="alpha">透明度</param>
		void SetAlpha(const float alpha)
		{
			m_alpha = alpha;
		}

		/// <summary>
		/// 透明度を取得
		/// </summary>
		/// <returns>透明度</returns>
		const float& GetAlpha() const
		{
			return m_alpha;
		}

		/// <summary>
		/// スクロール速度を設定
		/// </summary>
		/// <param name="scrollSpeed">スクロール速度</param>
		void SetScrollSpeed(float scrollSpeed)
		{
			m_scrollSpeed = scrollSpeed;
		}

		/// <summary>
		/// 時間の更新処理
		/// </summary>
		void UpdateDeltaTime()
		{
			m_deltaTime += 0.1f * g_gameTime->GetFrameDeltaTime();
			if (m_deltaTime > 1.0f)
			{
				m_deltaTime = 0.0f;
			}
		}

	private:
		/// <summary>
		/// モデルの初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="enModelUpAxis">モデルの上方向</param>
		void InitModel(const char* filePath, EnModelUpAxis enModelUpAxis, bool isShadowReceiver ,AlphaBlendMode alphaBlendMode);

		/// <summary>
		/// モデル(影描画)の初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="enModelUpAxis">モデルの上方向</param>
		void InitShadowCasterDrawing(const char* filePath, EnModelUpAxis enModelUpAxis, AlphaBlendMode alphaBlendMode);

		/// <summary>
		/// スケルトンの初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		void InitSkeleton(const char* filePath);

		/// <summary>
		/// アニメーションの初期化
		/// </summary>
		/// <param name="animationClips">アニメーションクリップ</param>
		/// <param name="numAnimationClips">アニメーションクリップの数</param>
		/// <param name="enModelUpAxis">モデルの上方向</param>
		void InitAnimation(AnimationClip* animationClips,
			int numAnimationClips,
			EnModelUpAxis enModelUpAxis);

		/// <summary>
		/// シャドウマップの描画
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		/// <param name="ca">カメラ</param>
		void OnRenderShadowMap(RenderContext& rc, Camera& ca) override;

		/// <summary>
		/// モデルの描画
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void OnRenderModel(RenderContext& rc) override;

		AnimationClip* m_animationClips = nullptr;		//アニメーションクリップ
		int m_numAnimationClips = 0;					//アニメーションの数
		Vector3 m_position = Vector3::Zero;				//座標
		Quaternion m_rotation = Quaternion::Identity;	//回転
		Vector3 m_scale = Vector3::One;					//拡大率
		EnModelUpAxis m_modelUpAxis = enModelUpAxisZ;	//モデルの上方向
		Animation m_animation;							//アニメーション	
		Model m_model;									//モデル
		Model m_shadowModel;							//影描画用モデル
		bool m_isUpdateAnimation = true;				//アニメーションを更新する？
		Skeleton m_skeleton;							//骨
		float m_animationSpeed = 1.0f;					//アニメーション再生速度
		float m_alpha = 1.0f;							//透明度
		float m_scrollSpeed = 0.0f;						//スクロール速度
		float m_deltaTime = 0.0f;						//時間
	};
}

