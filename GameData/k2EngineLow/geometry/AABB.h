#pragma once

namespace nsK2EngineLow {
	/// <summary>
	/// AABB
	/// </summary>
	class TkmFile;
	class AABB {
	private:
		Vector3 m_max;					// AABBを構成するボックスの最大座標。
		Vector3 m_min;					// AABBを構成するボックスの最小座標。
		Vector3 m_centerPos;			// 中心座標。
		Vector3 m_vertexPosition[8];	// AABBを構成する8頂点の座標。

	public:
		//AABBの8頂点の情報
		enum EnVertexInfo
		{
			enVertPos_X0Y0Z0,	//!< 左下手前の場所の頂点
			enVertPos_X1Y0Z0,	//!< 右下手前の場所の頂点
			enVertPos_X0Y1Z0,	//!< 左上手前の場所の頂点
			enVertPos_X1Y1Z0,	//!< 右上手前の場所の頂点
			enVertPos_X0Y0Z1,	//!< 左下奥の場所の頂点
			enVertPos_X1Y0Z1,	//!< 右下奥の場所の頂点
			enVertPos_X0Y1Z1,	//!< 左上奥の場所の頂点
			enVertPos_X1Y1Z1,	//!< 右上奥の場所の頂点
			enVertNum			//!< 頂点の数
		};
	public:
		/// <summary>
		/// AABBの情報を初期化する。
		/// </summary>
		/// <param name="vMax">最大値</param>
		/// <param name="vMin">最小値</param>
		void Init(const Vector3& vMax, const Vector3& vMin);
		/// <summary>
		/// tkmファイルからAABBの情報を初期化
		/// </summary>
		/// <param name="tkmFile">tkmファイル</param>
		void InitFromTkmFile(const TkmFile& tkmFile);
		/// <summary>
		/// AABBを構成する8頂点のワールド空間での座標を計算する。
		/// </summary>
		/// <param name="pos"></param>
		/// <param name="mWorld"></param>
		void CalcVertexPositions(Vector3* pos, const Matrix& mWorld);
		/// <summary>
		/// AABBの最大値を取得。
		/// </summary>
		const Vector3& GetMax() const
		{
			return m_max;
		}
		/// <summary>
		/// AABBの最小値を取得。
		/// </summary>
		/// <returns>最小値</returns>
		const Vector3& GetMin() const
		{
			return m_min;
		}
		/// <summary>
		/// 中心座標を取得。
		/// </summary>
		/// <returns>中心座標</returns>
		const Vector3& GetCenter() const
		{
			return m_centerPos;
		}
	};
}