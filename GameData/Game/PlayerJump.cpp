#include "stdafx.h"
#include "PlayerJump.h"

//ジャンプ処理の実行
void PlayerJump::Execute(Vector3& moveSpeed, CharacterController& characterController)
{
    //重力を無くす
    moveSpeed.y = 0.0f;

    //Aボタンが押されたら。
    if (g_pad[0]->IsTrigger(enButtonA))
    {
        //ジャンプさせる
        moveSpeed.y = 700.0f;
		m_isJump = true;
    }
}