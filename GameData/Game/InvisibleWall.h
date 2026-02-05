#pragma once

//見えない壁クラス
class InvisibleWall : public IGameObject
{
public:
	~InvisibleWall();//デストラクタ

	bool Start();//開始処理

private://列挙型

    //見えない壁
    enum EnInvisibleWall
    {
        enInvisibleWall_Forward,//前方の壁
        enInvisibleWall_Back,//後方の壁
        enInvisibleWall_Left,//左側の壁
        enInvisibleWall_Right,//右側の壁
        enInvisibleWall_Num,//見えない壁の種類
    };

private://メンバ関数

    /// <summary>
    /// 見えない壁の生成
    /// </summary>
    /// <param name="enInvisibleWall">壁の種類</param>
    /// <param name="pos">位置</param>
    /// <param name="rot">回転</param>
    /// <param name="size">大きさ</param>
    void CreateInvisibleWall(EnInvisibleWall enInvisibleWall, const Vector3& pos, const Quaternion& rot, const Vector3& size);

    /// <summary>
    /// 破棄処理
    /// </summary>
    /// <param name="enInvisibleWall">壁の種類</param>
    void Release(EnInvisibleWall enInvisibleWall);

private://メンバ変数
    std::unique_ptr<BoxCollider> m_collider[enInvisibleWall_Num];//コライダー
    RigidBody m_rigidBody[enInvisibleWall_Num];//剛体
};

