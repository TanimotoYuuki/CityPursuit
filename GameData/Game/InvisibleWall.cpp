#include "stdafx.h"
#include "InvisibleWall.h"

namespace {
    //壁の位置
    Vector3 FORWARD_INVISIBLE_WALL_POSITION{ 0.0f,0.0f,11750.0f };

    Vector3 BACK_INVISIBLE_WALL_POSITION{ 0.0f,0.0f,-11750.0f };

    Vector3 LEFT_INVISIBLE_WALL_POSITION{ -11750.0f,0.0f,0.0f };

    Vector3 RIGHT_INVISIBLE_WALL_POSITION{ 11750.0f,0.0f,0.0f };

    //壁の大きさ
    Vector3 INVISIBLE_WALL_SCALE{ 100000.0f,100000.0f,50.0f };
}

//デストラクタ
InvisibleWall::~InvisibleWall() {
    for (int i = 0; i < enInvisibleWall_Num; i++)
    {
        Release((EnInvisibleWall)i);//破棄処理
    }
}

//開始処理
bool InvisibleWall::Start() {
	//壁の配置
    CreateInvisibleWall(enInvisibleWall_Forward ,FORWARD_INVISIBLE_WALL_POSITION, Quaternion::Identity, INVISIBLE_WALL_SCALE);//前方の壁
    CreateInvisibleWall(enInvisibleWall_Back, BACK_INVISIBLE_WALL_POSITION, Quaternion::Identity, INVISIBLE_WALL_SCALE);//後方の壁

    Quaternion leftAndRightInvisibleWallRotation;
    leftAndRightInvisibleWallRotation.SetRotationDegY(90.0f);

    CreateInvisibleWall(enInvisibleWall_Left, LEFT_INVISIBLE_WALL_POSITION, leftAndRightInvisibleWallRotation, INVISIBLE_WALL_SCALE);//左側の壁
    CreateInvisibleWall(enInvisibleWall_Right, RIGHT_INVISIBLE_WALL_POSITION, leftAndRightInvisibleWallRotation, INVISIBLE_WALL_SCALE);//右側の壁

	return true;
}

//見えない壁の生成
void InvisibleWall::CreateInvisibleWall(EnInvisibleWall enInvisibleWall, const Vector3& pos, const Quaternion& rot, const Vector3& size)
{
    //ボックスコライダーを作成
    m_collider[enInvisibleWall] = std::make_unique<BoxCollider>();
    m_collider[enInvisibleWall]->Create(size);

    //剛体の初期化情報を設定
    RigidBodyInitData rbInfo;
    rbInfo.pos = pos;
    rbInfo.rot = rot;
    rbInfo.collider = m_collider[enInvisibleWall].get();
    rbInfo.mass = 0.0f;
    rbInfo.restitution = 0.0f;

    //剛体を初期化
    m_rigidBody[enInvisibleWall].Init(rbInfo);

    //摩擦力の設定
    m_rigidBody[enInvisibleWall].SetFriction(1.0f);
}

//破棄処理
void InvisibleWall::Release(EnInvisibleWall enInvisibleWall)
{
    m_rigidBody[enInvisibleWall].Release();
    m_collider[enInvisibleWall].reset();
}
