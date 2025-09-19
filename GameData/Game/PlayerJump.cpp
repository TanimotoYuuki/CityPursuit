#include "stdafx.h"
#include "PlayerJump.h"

//ジャンプ処理の実行
void PlayerJump::Execute(CharacterController& characterController, Vector3& moveSpeed)
{
    //地面に付いていたら
    if (characterController.IsOnGround())
    {
        //重力を無くす
        moveSpeed.y = 0.0f;

        //Aボタンが押されたら。
        if (g_pad[0]->IsTrigger(enButtonA))
        {
            //ジャンプさせる
            moveSpeed.y = 250.0f;
        }
    }
    //地面に付いていなかったら
    else
    {
        //重力を発生させる。
        moveSpeed.y -= 9.8f;
    }
}