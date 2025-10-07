#include "stdafx.h"
#include "SwingModel.h"
#include "Player.h"

namespace {
	constexpr const wchar_t* PLAYER_BONE_NAME = L"Character1_RightHand";//プレイヤーの手のボーンの名前
	const float STRETCHED_TIME = 0.3f;//糸が伸びきるまでの時間
}

//開始処理
bool SwingModel::Start()
{
	//スイングモデルの初期化
	m_swingModel.Init("Assets/ModelData/wire/wire.tkm");
	m_swingModel.Update();

	//プレイヤークラスのインスタンスの検索
	m_player = FindGO<Player>("player");

	return true;
}

//更新処理
void SwingModel::Update()
{
	//糸を伸びる開始座標の計算
	//プレイヤーの手のボーンIDの検索
	int boneID = m_player->GetModelData().FindBoneID(PLAYER_BONE_NAME);

	//プレイヤーの手のボーンのワールド行列の取得
	const Matrix& mHand = m_player->GetModelData().GetBone(boneID)->GetWorldMatrix();

	//座標に変換
	m_startStretchPos.x = mHand.m[3][0];
	m_startStretchPos.y = mHand.m[3][1];
	m_startStretchPos.z = mHand.m[3][2];

	//モデルを回転させる処理
	ModelRotation();

	//糸を伸ばす処理
	//糸を伸ばす率を増やす
	if (m_isStretched != true)
	{
		m_stretchRate += g_gameTime->GetFrameDeltaTime() / STRETCHED_TIME;
	}

	if (m_stretchRate >= 1.0f)
	{
		//伸びきった
		m_isStretched = true;
		//最大率を上限にする
		m_stretchRate = 1.0f;
	}

	//伸ばす座標へのベクトル
	const Vector3 toStretchPosVec = m_toStretchPos - m_startStretchPos;
	//伸ばす座標への距離
	const float toStretchPosDist = toStretchPosVec.Length();
	//伸ばす率で、伸ばす長さを決める
	float stretchLength = Math::Lerp<float>(m_stretchRate, 0.0f, toStretchPosDist);

	//手元側のモデルの拡大率
	float atHandModelScale = stretchLength;

	//手元側のモデルの拡大率が最大値より大きいか？
	if (atHandModelScale > 500.0f)
	{
		// 最大値に固定する
		atHandModelScale = 500.0f;
	}

	//モデルの拡大率と座標を設定する
	m_swingModel.SetScale({ 1.0f,1.0f,atHandModelScale * 100 });
	m_swingModel.SetPosition(m_startStretchPos);
	//スイングモデルの更新処理
	m_swingModel.Update();
}

//モデルを回転させる処理
void SwingModel::ModelRotation()
{
	// 伸ばす先への方向
	Vector3 toStretchPosDir = m_toStretchPos - m_startStretchPos;
	// 正規化する
	toStretchPosDir.Normalize();

	// 前方向と伸ばす先への方向の外積で、回転軸を求める。
	Vector3 rotAxis = Cross(Vector3::Front, toStretchPosDir);
	// 正規化する
	rotAxis.Normalize();

	// 前方向と伸ばす先への方向の内積を求める
	const float dotFrontAndToStretchPosDir = Dot(Vector3::Front, toStretchPosDir);
	// 内積からラジアン角を求める
	const float radAngle = acosf(dotFrontAndToStretchPosDir);

	// 回転クォータニオン
	Quaternion qRot;
	// 任意の軸周りで回転させる
	qRot.SetRotation(rotAxis, radAngle);

	// モデルに回転を設定する
	m_swingModel.SetRotation(qRot);
	//スイングモデルの更新処理
	m_swingModel.Update();
}

//指定した座標に向かって伸びる処理を開始する
void SwingModel::StartWireStretchToPos(const Vector3& position)
{
	//伸びきっていないようにする
	m_isStretched = false;

	//伸びる先の座標を設定する
	m_toStretchPos = position;

	//伸びる先へのベクトル
	Vector3 toStretchVec = m_toStretchPos - m_startStretchPos;
	//伸びる速度を設定する
	m_stretchSpeed =
		toStretchVec.Length() * g_gameTime->GetFrameDeltaTime() / STRETCHED_TIME;

	//モデルを描画させる
	EnableDrawingModel();

	// モデルを回転させる処理
	ModelRotation();

	//モデルの拡大率を初期化
	m_swingModel.SetScale(Vector3::One);
	//スイングモデルの座標を設定する
	m_swingModel.SetPosition(m_startStretchPos);
	//スイングモデルの更新処理
	m_swingModel.Update();
}

//指定した座標に向かって伸びる処理を終了する
void SwingModel::EndWireStretchToPos()
{
	// 伸びきっているようにする
	m_isStretched = true;

	// モデルを無効化する
	DisableDrawingModel();

	m_stretchRate = 0.0f;
}

//描画処理
void SwingModel::Render(RenderContext& rc)
{
	//モデルを描画するかどうか判断する変数がtrueなら描画する
	if (m_isDrawingModel)
	{
		//スイングモデルの描画処理
		m_swingModel.Draw(rc);
	}
}