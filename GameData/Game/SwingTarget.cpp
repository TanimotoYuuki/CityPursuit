#include "stdafx.h"
#include "SwingTarget.h"

//スイングのターゲットの座標の計算
void SwingTarget::CalcSwingTargetPositions(
	const Matrix& worldMatrix,
	const UINT widthSegments,
	const UINT heightSegments,
	const UINT lengthSegments,
	const float	heightLowerLimit
)
{
	//1.スイングのターゲットの座標コンテナを再初期化
	ReInitSwingTargetPositions(widthSegments, heightSegments, lengthSegments);


	//2.AABBの8頂点の座標を計算
	//AABBの8頂点の座標
	Vector3 vertexPos[AABB::enVertNum];
	//AABBの8頂点のワールド座標を計算
	m_aabb.CalcVertexPositions(vertexPos, worldMatrix);
	//座標も計算
	worldMatrix.Apply(m_position);


	//2.セグメント一つ当たりのベクトルを計算
	//ボックスの横幅、高さ、長さ
	const Vector3 boxWidth = vertexPos[AABB::enVertPos_X1Y0Z0] - vertexPos[AABB::enVertPos_X0Y0Z0];
	const Vector3 boxHeight = vertexPos[AABB::enVertPos_X0Y1Z0] - vertexPos[AABB::enVertPos_X0Y0Z0];
	const Vector3 boxLength = vertexPos[AABB::enVertPos_X0Y0Z1] - vertexPos[AABB::enVertPos_X0Y0Z0];
	//セグメント一つ当たりのベクトル
	const Vector3 widhtSegmentVec = boxWidth / static_cast<float>(widthSegments);
	const Vector3 heightSegmentVec = boxHeight / static_cast<float>(heightSegments);
	const Vector3 lengthSegmentVec = boxLength / static_cast<float>(lengthSegments);


	//3.ターゲットの座標たちの計算
	//セグメント+1がターゲットの数
	//側面（前面、後面、左面、右面）にターゲットがある
	//全ての側面で共通する、高さで繰り返す
	for (UINT hSeg = 0; hSeg < heightSegments + 1; hSeg++)
	{
		//高さセグメント分のベクトル
		const Vector3 hSegVec = heightSegmentVec * static_cast<float>(hSeg);

		//前面と後面のターゲットの座標を計算して保持する
		//前面と後面で共通する、幅で繰り返す
		for (UINT wSeg = 0; wSeg < widthSegments + 1; wSeg++)
		{
			//セグメントによる座標のオフセット
			const Vector3 targetOffset = hSegVec + widhtSegmentVec * static_cast<float>(wSeg);

			//前面のターゲットの座標を計算
			Vector3 targetPosition = vertexPos[AABB::enVertPos_X0Y0Z0] + targetOffset;	//ターゲットの座標

			//ターゲットが高さの下限より大きかったら
			if (targetPosition.y > heightLowerLimit)
			{
				//前面のターゲットの座標を保持
				m_swingTargetPositions.emplace_back(targetPosition);
			}

			//後面のターゲットの座標を計算
			targetPosition = vertexPos[AABB::enVertPos_X0Y0Z1] + targetOffset;

			//ターゲットが高さの下限より大きかったら
			if (targetPosition.y > heightLowerLimit)
			{
				//後面のターゲットの座標を保持
				m_swingTargetPositions.emplace_back(targetPosition);
			}
		}

		//左面と右面のターゲットの座標を計算して保持する
		//左面と右面で共通する、長さで繰り返す。
		//前面、後面と重複している両端の座標は計算しない。
		for (UINT lSeg = 1; lSeg < lengthSegments; lSeg++)
		{
			//セグメントによる座標のオフセット
			const Vector3 targetOffset = hSegVec + lengthSegmentVec * static_cast<float>(lSeg);

			//左面のターゲットの座標を計算
			Vector3 targetPosition = vertexPos[AABB::enVertPos_X0Y0Z0] + targetOffset;	// ターゲットの座標

			//ターゲットが高さの下限より大きかったら
			if (targetPosition.y > heightLowerLimit)
			{
				//左面とターゲットの座標を保持
				m_swingTargetPositions.emplace_back(targetPosition);
			}

			//右面のターゲット座標を計算
			targetPosition = vertexPos[AABB::enVertPos_X1Y0Z0] + targetOffset;

			//ターゲットが高さの下限より大きかったら
			if (targetPosition.y > heightLowerLimit)
			{
				//右面とターゲットの座標を保持
				m_swingTargetPositions.emplace_back(targetPosition);
			}
		}
	}

	return;
}

//スイングのターゲットの座標コンテナの再初期化
void SwingTarget::ReInitSwingTargetPositions(
	const UINT widthSegments,
	const UINT heightSegments,
	const UINT lengthSegments
)
{
	//コンテナをリセット
	//コンテナのデータを破棄
	m_swingTargetPositions.clear();
	//コンテナのキャパシティをリセット
	m_swingTargetPositions.shrink_to_fit();

	//ターゲットの数
	int TargetsNum = 0;

	//セグメントは面を分割する数のため、1つに分割されたら端は2つ、2つに分割されたら端は3つ
	//という風にセグメント+1がターゲットの数になる

	//AABBの壁（側面）の部分にターゲットを作る
	//前面と後面のターゲットの数を加算する
	TargetsNum += (widthSegments + 1) * (heightSegments + 1) * 2;
	//左面と右面のターゲットの数を加算する
	TargetsNum += (lengthSegments + 1) * (heightSegments + 1) * 2;
	//重複している四つ角のターゲットを減算する
	TargetsNum -= (heightSegments + 1) * 4;

	//コンテナのキャパシティをターゲットの数分だけ確保する
	m_swingTargetPositions.reserve(TargetsNum);

	return;
}