#pragma once
namespace nsK2EngineLow
{
	class Camera;
	/// <summary>
	/// 視錐台クラス
	/// </summary>
	class ViewFrustum
	{
	public:
		/// <summary>
		///	平面構造体
		/// </summary>
		struct Plane
		{
			Vector3 normal;
			float distance;

			//正規化
			void Normalize() {
				float len = normal.Length();
				// 0除算防止
				if (len > 1e-6f) {
					float invLen = 1.0f / len;
					normal *= invLen;
					distance *= invLen;
				}
			}
		};

		//6枚の平面の識別子
		enum EnPlaceType {
			enPlaceType_Near,//近平面
			enPlaceType_Far,//遠平面
			enPlaceType_Left,//左平面
			enPlaceType_Right,//右平面
			enPlaceType_Top,//上平面
			enPlaceType_Bottom,//下平面
			enPlaceType_Num
		};

		/// <summary>
		/// カメラ情報から視錐台(6枚の平面)を更新する
		/// </summary>
		/// <param name="camera"></param>
		void Update(Camera& camera);

		/// <summary>
		/// 球体が視界内に入っているか判定する
		/// </summary>
		/// <param name="center">球の中心座標</param>
		/// <param name="radius">球の半径</param>
		/// <returns>trueなら視界内</returns>
		bool CheckSphere(const Vector3& center, float radius) const;

	private://メンバ変数
		Plane m_planes[enPlaceType_Num];//6枚の平面データ
	};
}

