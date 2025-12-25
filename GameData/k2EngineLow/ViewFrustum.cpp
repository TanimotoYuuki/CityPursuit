#include "k2EngineLowPreCompile.h"
#include "ViewFrustum.h"

namespace nsK2EngineLow {

	void ViewFrustum::Update(Camera& camera)
	{
		//カメラからビュープロジェクション行列を取得
		const Matrix& vp = camera.GetViewProjectionMatrix();

		//左平面
		m_planes[enPlaceType_Left].normal.x = vp.m[0][3] + vp.m[0][0];
		m_planes[enPlaceType_Left].normal.y = vp.m[1][3] + vp.m[1][0];
		m_planes[enPlaceType_Left].normal.z = vp.m[2][3] + vp.m[2][0];
		m_planes[enPlaceType_Left].distance = vp.m[3][3] + vp.m[3][0];

		//右平面
		m_planes[enPlaceType_Right].normal.x = vp.m[0][3] - vp.m[0][0];
		m_planes[enPlaceType_Right].normal.y = vp.m[1][3] - vp.m[1][0];
		m_planes[enPlaceType_Right].normal.z = vp.m[2][3] - vp.m[2][0];
		m_planes[enPlaceType_Right].distance = vp.m[3][3] - vp.m[3][0];

		//下平面
		m_planes[enPlaceType_Bottom].normal.x = vp.m[0][3] + vp.m[0][1];
		m_planes[enPlaceType_Bottom].normal.y = vp.m[1][3] + vp.m[1][1];
		m_planes[enPlaceType_Bottom].normal.z = vp.m[2][3] + vp.m[2][1];
		m_planes[enPlaceType_Bottom].distance = vp.m[3][3] + vp.m[3][1];

		//上平面
		m_planes[enPlaceType_Top].normal.x = vp.m[0][3] - vp.m[0][1];
		m_planes[enPlaceType_Top].normal.y = vp.m[1][3] - vp.m[1][1];
		m_planes[enPlaceType_Top].normal.z = vp.m[2][3] - vp.m[2][1];
		m_planes[enPlaceType_Top].distance = vp.m[3][3] - vp.m[3][1];

		//近平面
		m_planes[enPlaceType_Near].normal.x = vp.m[0][3] + vp.m[0][2];
		m_planes[enPlaceType_Near].normal.y = vp.m[1][3] + vp.m[1][2];
		m_planes[enPlaceType_Near].normal.z = vp.m[2][3] + vp.m[2][2];
		m_planes[enPlaceType_Near].distance = vp.m[3][3] + vp.m[3][2];

		//遠平面
		m_planes[enPlaceType_Far].normal.x = vp.m[0][3] - vp.m[0][2];
		m_planes[enPlaceType_Far].normal.y = vp.m[1][3] - vp.m[1][2];
		m_planes[enPlaceType_Far].normal.z = vp.m[2][3] - vp.m[2][2];
		m_planes[enPlaceType_Far].distance = vp.m[3][3] - vp.m[3][2];

		//正規化
		for (int i = 0; i < enPlaceType_Num; i++)
		{
			m_planes[i].Normalize();
		}
	}

	bool ViewFrustum::CheckSphere(const Vector3& center, float radius) const
	{
		//6枚すべての壁に対して球体が視界内に入っているか判定する
		for (int i = 0; i < enPlaceType_Num; i++)
		{
			float dist = m_planes[i].normal.Dot(center) + m_planes[i].distance;

			if (dist < -radius)
			{
				return false;
			}
		}

		return true;
	}
}