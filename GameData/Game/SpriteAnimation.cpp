#include "stdafx.h"
#include "SpriteAnimation.h"

//座標を変えるアニメーションの更新処理
void PositionSpriteAnimation::Update()
{
	if (!m_isLoop && m_isCompleted)
	{
		Vector3 completePostion = m_targetPosition;
		m_render->SetPosition(completePostion);
		m_render->Update();
		return;
	}

	const float deltaTime = g_gameTime->GetFrameDeltaTime();

	Vector3 targetPosition;
	Vector3 basePosition;
	
	switch (m_currentStep)
	{
	case SpriteAnimationBase::enAnimationStep_Min:
		targetPosition = m_targetPosition;
		basePosition = m_basePosition;
		break;
	case SpriteAnimationBase::enAnimationStep_Max:
		targetPosition = m_basePosition;
		basePosition = m_targetPosition;
		break;
	default:
		break;
	}

	const float computePercent = m_elapsedTime / m_targetTime;

	Vector3 computePosition = basePosition;
	computePosition.Lerp(computePercent, basePosition, targetPosition);

	m_render->SetPosition(computePosition);
	m_render->Update();

	m_elapsedTime += deltaTime * m_playSpeed;
	if (m_elapsedTime >= m_targetTime)
	{
		m_elapsedTime = 0.0f;

		m_currentStep = m_currentStep == enAnimationStep_Max ? enAnimationStep_Min : enAnimationStep_Max;

		m_isCompleted = true;
	}
}

//回転を変えるアニメーションの更新処理
void RotationSpriteAnimation::Update()
{
	if (!m_isLoop && m_isCompleted)
	{
		Quaternion completeRotation = m_targetRotation;
		m_render->SetRotation(completeRotation);
		m_render->Update();
		return;
	}

	const float deltaTime = g_gameTime->GetFrameDeltaTime();

	Quaternion targetRotation;
	Quaternion baseRotation;

	switch (m_currentStep)
	{
	case enAnimationStep_Min:
		targetRotation = m_targetRotation;
		baseRotation = m_baseRotation;
		break;
	case enAnimationStep_Max:
		targetRotation = m_baseRotation;
		baseRotation = m_targetRotation;
		break;
	}

	const float computePercent = m_elapsedTime / m_targetTime;

	Quaternion computeRotation = baseRotation;
	computeRotation.Slerp(computePercent, baseRotation, targetRotation);

	m_render->SetRotation(computeRotation);
	m_render->Update();

	m_elapsedTime += deltaTime * m_playSpeed;
	if (m_elapsedTime >= m_targetTime) {
		m_elapsedTime = 0.0f;

		m_currentStep = m_currentStep == enAnimationStep_Min ? enAnimationStep_Max : enAnimationStep_Min;

		// 完了
		m_isCompleted = true;
	}
}

//大きさを変えるアニメーションの更新処理
void ScaleSpriteAnimation::Update()
{
	// 繰り返し実行するか
	if (!m_isLoop && m_isCompleted)
	{
		Vector2 completeScale = m_targetScale;
		m_render->SetScale(Vector3(completeScale.x, completeScale.y, 1.0f));
		m_render->Update();
		return;
	}

	const float deltaTime = g_gameTime->GetFrameDeltaTime();

	Vector2 targetScale;
	Vector2 baseScale;
	// ステップの切り替わりで拡縮
	switch (m_currentStep)
	{
	case enAnimationStep_Min:
		targetScale = m_targetScale;
		baseScale = m_baseScale;
		break;
	case enAnimationStep_Max:
		targetScale = m_baseScale;
		baseScale = m_targetScale;
		break;
	}

	// 現在のアニメーション経過時間のパーセント
	const float computePercent = m_elapsedTime / m_targetTime;

	// 初期値から目標値までをなめらかに変化
	Vector2 computeScale = baseScale;
	computeScale.Lerp(computePercent, baseScale, targetScale);

	// 大きさを設定する
	m_render->SetScale(Vector3(computeScale.x, computeScale.y, 1.0f));
	m_render->Update();

	// 一定時間経過したら往復
	m_elapsedTime += deltaTime * m_playSpeed;
	if (m_elapsedTime >= m_targetTime) {
		m_elapsedTime = 0.0f;
		// 三項演算子を使ったif文
		// 今の状態(Step)がMaxだった場合->Min。そうでない場合->Max
		// 例文)condition ? value_if_true : value_if_false;
		m_currentStep = m_currentStep == enAnimationStep_Max ? enAnimationStep_Min : enAnimationStep_Max;

		// 完了
		m_isCompleted = true;
	}
}

//色を変えるアニメーションの更新処理
void ColorSpriteAnimation::Update()
{
	if (!m_isLoop && m_isCompleted)
	{
		Vector4 completeColor = m_targetColor;
		m_render->SetMulColor(completeColor);
		m_render->Update();
		return;
	}

	const float deltaTime = g_gameTime->GetFrameDeltaTime();

	Vector4 targetColor;
	Vector4 baseColor;

	switch (m_currentStep)
	{
	case enAnimationStep_Min:
		targetColor = m_targetColor;
		baseColor = m_baseColor;
		break;
	case enAnimationStep_Max:
		targetColor = m_baseColor;
		baseColor = m_targetColor;
		break;
	}

	const float computePercent = m_elapsedTime / m_targetTime;

	Vector4 computeColor = nsK2EngineLow::Math::Lerp<Vector4>(computePercent, baseColor, targetColor);

	m_render->SetMulColor(computeColor);
	m_render->Update();

	m_elapsedTime += deltaTime * m_playSpeed;
	if (m_elapsedTime >= m_targetTime) {
		m_elapsedTime = 0;
		m_currentStep = m_currentStep == enAnimationStep_Min ? enAnimationStep_Max : enAnimationStep_Min;

		// 完了
		m_isCompleted = true;
	}
}

//透明度を変えるアニメーションの更新処理
void AlphaSpriteAnimation::Update()
{
	if (!m_isLoop && m_isCompleted)
	{
		float completeAlpha = m_targetAlpha;
		m_render->SetMulColor(Vector4(1.0f, 1.0f, 1.0f, completeAlpha));
		m_render->Update();
		return;
	}

	const float deltaTime = g_gameTime->GetFrameDeltaTime();

	float targetAlpha;
	float baseAlpha;

	switch (m_currentStep)
	{
	case enAnimationStep_Min:
		targetAlpha = m_targetAlpha;
		baseAlpha = m_baseAlpha;
		break;
	case enAnimationStep_Max:
		targetAlpha = m_baseAlpha;
		baseAlpha = m_targetAlpha;
		break;
	}

	const float computePercent = m_elapsedTime / m_targetTime;

	float computeAlpha = nsK2EngineLow::Math::Lerp<float>(computePercent, baseAlpha, targetAlpha);

	m_render->SetMulColor(Vector4(1.0f, 1.0f, 1.0f, computeAlpha));
	m_render->Update();

	m_elapsedTime += deltaTime * m_playSpeed;
	if (m_elapsedTime >= m_targetTime) {
		m_elapsedTime = 0;
		m_currentStep = m_currentStep == enAnimationStep_Min ? enAnimationStep_Max : enAnimationStep_Min;

		// 完了
		m_isCompleted = true;
	}
}
