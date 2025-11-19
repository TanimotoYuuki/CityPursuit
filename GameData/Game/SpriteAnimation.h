#pragma once

//基底クラス
class SpriteAnimationBase
{
protected:
	//アニメーションステップ
	enum EnAnimationStep
	{
		enAnimationStep_Min,//下限
		enAnimationStep_Max//上限
	};

protected:
	SpriteRender* m_render = nullptr;//スプライトレンダー
	float m_elapsedTime = 0.0f;//経過した割合
	float m_targetTime = 0.0f;//ターゲットの割合
	float m_playSpeed = 1.0f;//アニメーションの再生速度
	EnAnimationStep m_currentStep;//現在のステップ
	bool m_isLoop = false;//ループするか?
	bool m_isCompleted = false;//処理が完了したか?

public:
	/// <summary>
	/// スプライトアニメションベースの引数付きコンストラクタ
	/// </summary>
	/// <param name="render">アニメーションをさせるスプライト</param>
	/// <param name="targetTime">ターゲットの割合</param>
	/// <param name="playSpeed">アニメーションの再生速度</param>
	/// <param name="isLoop">ループするか?</param>
	SpriteAnimationBase(SpriteRender* render, const float targetTime, const float playSpeed, const bool isLoop)
		: m_render(render)
		, m_targetTime(targetTime)
		, m_playSpeed(playSpeed)
		, m_isLoop(isLoop)
	{
	}

	/// <summary>
	/// 処理が完了したか
	/// </summary>
	/// <returns>trueなら処理が完了している</returns>
	bool IsCompleted()
	{
		return m_isCompleted;
	}
	
	/// <summary>
	/// リセット処理
	/// </summary>
	void Reset()
	{
		m_elapsedTime = 0.0f;
		m_currentStep = enAnimationStep_Min;
		m_isCompleted = false;
	}

	virtual void Update() = 0;//更新処理
};

//座標を変えるアニメーションクラス
class PositionSpriteAnimation : public SpriteAnimationBase
{
private:
	Vector3 m_basePosition = Vector3::Zero;//元の座標
	Vector3 m_targetPosition = Vector3::Zero;//ターゲットの座標

public:
	/// <summary>
	/// 位置を変えるアニメーションの引数付きコンストラクタ
	/// </summary>
	/// <param name="render">アニメーションをさせるスプライト</param>
	/// <param name="targetTime">ターゲットの割合</param>
	/// <param name="playSpeed">アニメーションの再生速度</param>
	/// <param name="isLoop">ループするか?</param>
	/// <param name="basePosition">元の座標</param>
	/// <param name="targetPosition">ターゲットの座標</param>
	PositionSpriteAnimation(SpriteRender* render, const float targetTime, const float playSpeed, const bool isLoop, const Vector3 basePosition, const Vector3 targetPosition)
		: SpriteAnimationBase(render, targetTime, playSpeed, isLoop)
		, m_basePosition(basePosition)
		, m_targetPosition(targetPosition)
	{
	}

	void Update() override;//更新処理
};

//回転を変えるアニメーションクラス
class RotationSpriteAnimation : public SpriteAnimationBase
{
private:
	Quaternion m_baseRotation = Quaternion::Identity;//元の回転
	Quaternion m_targetRotation = Quaternion::Identity;//ターゲットの回転

public:
	/// <summary>
	/// 回転を変えるアニメーションの引数付きコンストラクタ
	/// </summary>
	/// <param name="render">アニメーションをするスプライト</param>
	/// <param name="targetTime">ターゲットの割合</param>
	/// <param name="playSpeed">アニメーションの再生速度</param>
	/// <param name="isLoop">ループするか?</param>
	/// <param name="baseRotation">元の回転</param>
	/// <param name="targetRotation">ターゲットの回転</param>
	RotationSpriteAnimation(SpriteRender* render, const float targetTime, const float playSpeed, const bool isLoop, const Quaternion baseRotation, const Quaternion targetRotation)
		: SpriteAnimationBase(render, targetTime, playSpeed, isLoop)
		, m_baseRotation(baseRotation)
		, m_targetRotation(targetRotation)
	{
	}

	void Update() override;//更新処理
};

//大きさを変えるアニメーションクラス
class ScaleSpriteAnimation : public SpriteAnimationBase
{
private:
	Vector2 m_baseScale = Vector2::Zero;//元の大きさ
	Vector2 m_targetScale = Vector2::Zero;//ターゲットの大きさ

public:
	/// <summary>
	/// 大きさを変えるアニメーションの引数付きコンストラクタ
	/// </summary>
	/// <param name="render">アニメーションをするスプライト</param>
	/// <param name="targetTime">ターゲットの割合/param>
	/// <param name="playSpeed">アニメーションの再生速度</param>
	/// <param name="isLoop">ループするか?</param>
	/// <param name="baseScale">元の大きさ</param>
	/// <param name="targetScale">ターゲットの大きさ</param>
	ScaleSpriteAnimation(SpriteRender* render, const float targetTime, const float playSpeed, const bool isLoop, const Vector2 baseScale, const Vector2 targetScale)
		: SpriteAnimationBase(render, targetTime, playSpeed, isLoop)
		, m_baseScale(baseScale)
		, m_targetScale(targetScale)
	{
	}

	void Update() override;//更新処理
};

//色を変えるアニメーションクラス
class ColorSpriteAnimation : public SpriteAnimationBase
{
private:
	Vector4 m_baseColor = Vector4::White;//元の色
	Vector4 m_targetColor = Vector4::White;//ターゲットの色

public:
	/// <summary>
	/// 色を変えるアニメーションの引数付きコンストラクタ
	/// </summary>
	/// <param name="render">アニメーションをするスプライト</param>
	/// <param name="targetTime">ターゲットの割合</param>
	/// <param name="playSpeed">アニメーションの再生速度</param>
	/// <param name="isLoop">ループするか?</param>
	/// <param name="baseColor">元の色</param>
	/// <param name="targetColor">ターゲットの色</param>
	ColorSpriteAnimation(SpriteRender* render, const float targetTime, const float playSpeed, const bool isLoop, const Vector4 baseColor, const Vector3 targetColor)
		: SpriteAnimationBase(render, targetTime, playSpeed, isLoop)
		, m_baseColor(baseColor)
		, m_targetColor(targetColor)
	{
	}

	void Update() override;//更新処理
};

//透明度を変えるアニメーションクラス
class AlphaSpriteAnimation : public SpriteAnimationBase
{
private:
	float m_baseAlpha= 0.0f;//元の透明度
	float m_targetAlpha = 0.0f;//ターゲットの透明度

public:
	/// <summary>
	/// 透明度を変えるアニメーションの引数付きコンストラクタ
	/// </summary>
	/// <param name="render">アニメーションをするスプライト</param>
	/// <param name="targetTime">ターゲットの割合</param>
	/// <param name="playSpeed">アニメーションの再生速度</param>
	/// <param name="isLoop">ループするか?</param>
	/// <param name="baseAlpha">元の透明度</param>
	/// <param name="targetAlpha">ターゲットの透明度</param>
	AlphaSpriteAnimation(SpriteRender* render, const float targetTime, const float playSpeed, const bool isLoop, const float baseAlpha, const float targetAlpha)
		: SpriteAnimationBase(render, targetTime, playSpeed, isLoop)
		, m_baseAlpha(baseAlpha)
		, m_targetAlpha(targetAlpha)
	{
	}

	void Update() override;//更新処理
};

