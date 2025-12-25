#include "stdafx.h"
#include "PlayerJump.h"

//ジャンプ処理の実行
void PlayerJump::Execute(Vector3& moveSpeed, CharacterController& characterController)
{
    //Aボタンが押されたら。
    if (g_pad[0]->IsTrigger(enButtonA))
    {
        //ジャンプさせる
        moveSpeed.y = 700.0f;
        GameSoundEngine::GetInstance()->PlaySE(GameSoundList_SE_Jump, 3.0f);
		m_isJump = true;
        m_isOnGround = false;
    }
}