#include "k2EngineLowPreCompile.h"
#include "GameSoundEngine.h"
namespace
{
	const int BGM_NO_PLAYING = -1; //BGMが再生していない
}

namespace nsK2EngineLow
{
	//初期化
	void GameSoundEngine::Init()
	{
		//BGM
		//0. タイトル画面BGM
		g_soundEngine->ResistWaveFileBank(GameSoundList_BGM_Title, "Assets/sound/bgm/title.wav");
		m_isPlayingSound[GameSoundList_BGM_Title] = false;
		m_sound[GameSoundList_BGM_Title] = nullptr;

		//1. ステージ1BGM
		g_soundEngine->ResistWaveFileBank(GameSoundList_BGM_Stage1, "Assets/sound/bgm/stage1.wav");
		m_isPlayingSound[GameSoundList_BGM_Stage1] = false;
		m_sound[GameSoundList_BGM_Stage1] = nullptr;

		//2. ステージ2BGM
		g_soundEngine->ResistWaveFileBank(GameSoundList_BGM_Stage2, "Assets/sound/bgm/stage2.wav");
		m_isPlayingSound[GameSoundList_BGM_Stage2] = false;
		m_sound[GameSoundList_BGM_Stage2] = nullptr;

		//3. リザルト画面BGM
		g_soundEngine->ResistWaveFileBank(GameSoundList_BGM_Result, "Assets/sound/bgm/result.wav");
		m_isPlayingSound[GameSoundList_BGM_Result] = false;
		m_sound[GameSoundList_BGM_Result] = nullptr;

		//SE
		//0. 決定音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_SelectScreen_Decision, "Assets/sound/se/decision.wav");
		m_isPlayingSound[GameSoundList_SE_SelectScreen_Decision] = false;
		m_sound[GameSoundList_SE_SelectScreen_Decision] = nullptr;

		//1. 決定音(画面遷移用)
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_SelectScreen_Decision_ScreenTransition,"Assets/sound/se/decision_gamestart.wav");
		m_isPlayingSound[GameSoundList_SE_SelectScreen_Decision_ScreenTransition] = false;
		m_sound[GameSoundList_SE_SelectScreen_Decision_ScreenTransition] = nullptr;

		//2. 選択音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_SelectScreen_Select, "Assets/sound/se/select.wav");
		m_isPlayingSound[GameSoundList_SE_SelectScreen_Select] = false;
		m_sound[GameSoundList_SE_SelectScreen_Select] = nullptr;

		//3. キャンセル音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_SelectScreen_Cancel, "Assets/sound/se/cancel.wav");
		m_isPlayingSound[GameSoundList_SE_SelectScreen_Cancel] = false;
		m_sound[GameSoundList_SE_SelectScreen_Cancel] = nullptr;

		//4. プレイヤーの足音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Player_FootStep, "Assets/sound/se/footstep.wav");
		m_isPlayingSound[GameSoundList_SE_Player_FootStep] = false;

		//5. プレイヤーのジャンプ音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Player_Jump, "Assets/sound/se/jump.wav");
		m_isPlayingSound[GameSoundList_SE_Player_Jump] = false;
		m_sound[GameSoundList_SE_Player_Jump] = nullptr;

		//6. プレイヤーのがミスした時の音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Player_Miss, "Assets/sound/se/miss.wav");
		m_isPlayingSound[GameSoundList_SE_Player_Miss] = false;
		m_sound[GameSoundList_SE_Player_Miss] = nullptr;

		//7. プレイヤーが敵を踏んだ時の音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Player_StepOnEnemy, "Assets/sound/se/stepon.wav");
		m_isPlayingSound[GameSoundList_SE_Player_StepOnEnemy] = false;
		m_sound[GameSoundList_SE_Player_StepOnEnemy] = nullptr;

		//8. ブロックを叩いた時の音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Object_BlockTouch, "Assets/sound/se/blocktouch.wav");
		m_isPlayingSound[GameSoundList_SE_Object_BlockTouch] = false;
		m_sound[GameSoundList_SE_Object_BlockTouch] = nullptr;
		
		//9. アイテムを取得した時の音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Object_ItemGet, "Assets/sound/se/itemget.wav");
		m_isPlayingSound[GameSoundList_SE_Object_ItemGet] = false;
		m_sound[GameSoundList_SE_Object_ItemGet] = nullptr;

		//10. 中間地点を通過した時の音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Object_PassingCheckPoint, "Assets/sound/se/passingcheckpoint.wav");
		m_isPlayingSound[GameSoundList_SE_Object_PassingCheckPoint] = false;
		m_sound[GameSoundList_SE_Object_PassingCheckPoint] = nullptr;

		//11. リザルト画面で取得したアイテムを表示した時の音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Result_ViewItemGet, "Assets/sound/se/viewresult.wav");
		m_isPlayingSound[GameSoundList_SE_Result_ViewItemGet] = false;
		m_sound[GameSoundList_SE_Result_ViewItemGet] = nullptr;

		//12. Cランク用の音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Result_CRank, "Assets/sound/se/crank.wav");
		m_isPlayingSound[GameSoundList_SE_Result_CRank] = false;
		m_sound[GameSoundList_SE_Result_CRank] = nullptr;

		//13. Bランク用の音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Result_BRank, "Assets/sound/se/brank.wav");
		m_isPlayingSound[GameSoundList_SE_Result_BRank] = false;
		m_sound[GameSoundList_SE_Result_BRank] = nullptr;

		//14. Aランク用の音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Result_ARank, "Assets/sound/se/arank.wav");
		m_isPlayingSound[GameSoundList_SE_Result_ARank] = false;
		m_sound[GameSoundList_SE_Result_ARank] = nullptr;

		//15. Sランク用の音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Result_SRank, "Assets/sound/se/srank.wav");
		m_isPlayingSound[GameSoundList_SE_Result_SRank] = false;
		m_sound[GameSoundList_SE_Result_SRank] = nullptr;

		//16. ステージクリア時の音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_System_StageClear, "Assets/sound/se/stageclear.wav");
		m_isPlayingSound[GameSoundList_SE_System_StageClear] = false;
		m_sound[GameSoundList_SE_System_StageClear] = nullptr;

		//17. ゲームオーバー時の音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_System_GameOver, "Assets/sound/se/gameover.wav");
		m_isPlayingSound[GameSoundList_SE_System_GameOver] = false;
		m_sound[GameSoundList_SE_System_GameOver] = nullptr;
	}

	//BGMを再生
	void GameSoundEngine::PlayBGM(GameSoundList gameSoundList, float soundVolume)
	{
		if (!IsPlayingSound(gameSoundList))
		{
			m_sound[gameSoundList] = NewGO<SoundSource>(0);
			m_sound[gameSoundList]->Init(gameSoundList);
			m_sound[gameSoundList]->Play(true);
			m_sound[gameSoundList]->SetVolume(soundVolume);
			m_isPlayingSound[gameSoundList] = true;
			m_nowBGMPlaying = gameSoundList;
		}
	}

	//SEを再生
	void GameSoundEngine::PlaySE(GameSoundList gameSoundList, float soundVolume)
	{
		m_sound[gameSoundList] = NewGO<SoundSource>(0);
		m_sound[gameSoundList]->Init(gameSoundList);
		m_sound[gameSoundList]->Play(false);
		m_sound[gameSoundList]->SetVolume(soundVolume);
	}

	//更新処理
	void GameSoundEngine::Update()
	{
		//BGMを再生しているときに処理する
		if (m_nowBGMPlaying != BGM_NO_PLAYING)
		{
			//今再生しているサウンドインスタンスが削除したら
			if (m_sound[m_nowBGMPlaying]->IsDead())
			{
				//サウンドが再生しているかを判定するフラグをfalseにする
				m_isPlayingSound[m_nowBGMPlaying] = false;
			}
		}
	}
}