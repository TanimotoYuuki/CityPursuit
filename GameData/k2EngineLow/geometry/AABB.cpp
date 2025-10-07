#include "k2EngineLowPreCompile.h"
#include "geometry/AABB.h"

namespace nsK2EngineLow {
	void AABB::Init(const Vector3& vMax, const Vector3& vMin)
	{
		m_max = vMax;
		m_min = vMin;
		Vector3 halfSize = (m_max - m_min) * 0.5f;
		Vector3 centerPos = (vMax + vMin) * 0.5f;
		m_centerPos = centerPos;

		//左下奥
		m_vertexPosition[enVertPos_X0Y0Z1] = centerPos;
		m_vertexPosition[enVertPos_X0Y0Z1].x -= halfSize.x;
		m_vertexPosition[enVertPos_X0Y0Z1].y -= halfSize.y;
		m_vertexPosition[enVertPos_X0Y0Z1].z -= halfSize.z;

		//右下奥
		m_vertexPosition[enVertPos_X1Y0Z1] = centerPos;
		m_vertexPosition[enVertPos_X1Y0Z1].x += halfSize.x;
		m_vertexPosition[enVertPos_X1Y0Z1].y -= halfSize.y;
		m_vertexPosition[enVertPos_X1Y0Z1].z -= halfSize.z;

		//左下手前
		m_vertexPosition[enVertPos_X0Y0Z0] = centerPos;
		m_vertexPosition[enVertPos_X0Y0Z0].x -= halfSize.x;
		m_vertexPosition[enVertPos_X0Y0Z0].y += halfSize.y;
		m_vertexPosition[enVertPos_X0Y0Z0].z -= halfSize.z;

		//右下手前
		m_vertexPosition[enVertPos_X1Y0Z0] = centerPos;
		m_vertexPosition[enVertPos_X1Y0Z0].x += halfSize.x;
		m_vertexPosition[enVertPos_X1Y0Z0].y += halfSize.y;
		m_vertexPosition[enVertPos_X1Y0Z0].z -= halfSize.z;

		//左上奥
		m_vertexPosition[enVertPos_X0Y1Z1] = centerPos;
		m_vertexPosition[enVertPos_X0Y1Z1].x -= halfSize.x;
		m_vertexPosition[enVertPos_X0Y1Z1].y -= halfSize.y;
		m_vertexPosition[enVertPos_X0Y1Z1].z += halfSize.z;

		//右上奥
		m_vertexPosition[enVertPos_X1Y1Z1] = centerPos;
		m_vertexPosition[enVertPos_X1Y1Z1].x += halfSize.x;
		m_vertexPosition[enVertPos_X1Y1Z1].y -= halfSize.y;
		m_vertexPosition[enVertPos_X1Y1Z1].z += halfSize.z;

		//左上手前
		m_vertexPosition[enVertPos_X0Y1Z0] = centerPos;
		m_vertexPosition[enVertPos_X0Y1Z0].x -= halfSize.x;
		m_vertexPosition[enVertPos_X0Y1Z0].y += halfSize.y;
		m_vertexPosition[enVertPos_X0Y1Z0].z += halfSize.z;

		//右上手前
		m_vertexPosition[enVertPos_X1Y1Z0] = centerPos;
		m_vertexPosition[enVertPos_X1Y1Z0].x += halfSize.x;
		m_vertexPosition[enVertPos_X1Y1Z0].y += halfSize.y;
		m_vertexPosition[enVertPos_X1Y1Z0].z += halfSize.z;

	}

	void AABB::InitFromTkmFile(const TkmFile& tkmFile)
	{
		Vector3 vMax = { -FLT_MAX,-FLT_MAX,-FLT_MAX };
		Vector3 vMin = { FLT_MAX,FLT_MAX,FLT_MAX };

		tkmFile.QueryMeshParts(
			[&](const TkmFile::SMesh& mesh)
			{
				for (const auto& vertex : mesh.vertexBuffer)
				{
					vMax.Max(vertex.pos);
					vMin.Min(vertex.pos);
				}
				
				return;
			}
		);

		Init(vMax, vMin);
		return;
	}

	void AABB::CalcVertexPositions(Vector3* pos, const Matrix& mWorld)
	{
		for (int vertNo = 0; vertNo < 8; vertNo++) {
			pos[vertNo] = m_vertexPosition[vertNo];
			mWorld.Apply(pos[vertNo]);
		}
	}
}