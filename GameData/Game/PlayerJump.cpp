#include "stdafx.h"
#include "PlayerJump.h"

//ジャンプ処理の実行
void PlayerJump::Execute(Vector3& moveSpeed)
{
    //重力を無くす
    moveSpeed.y = 0.0f;

    //Aボタンが押されたら。
    if (g_pad[0]->IsTrigger(enButtonA))
    {
        //ジャンプさせる
        moveSpeed.y = 375.0f;
    }
}