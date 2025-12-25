#include "k2EngineLowPreCompile.h"
#include "GameSoundEngine.h"
namespace
{
	const int BGM_NO_PLAYING = -1; //BGMが再生していない
}

namespace nsK2EngineLow
{
	GameSoundEngine* GameSoundEngine::m_instance = nullptr;

	//初期化
	void GameSoundEngine::Init()
	{
		//BGM
		//0.タイトル画面BGM
		g_soundEngine->ResistWaveFileBank(GameSoundList_BGM_Title, "Assets/sound/bgm/title.wav");
		m_isPlayingSound[GameSoundList_BGM_Title] = false;
		m_sound[GameSoundList_BGM_Title] = nullptr;

		//1.インゲームBGM
		g_soundEngine->ResistWaveFileBank(GameSoundList_BGM_InGame, "Assets/sound/bgm/inGame.wav");
		m_isPlayingSound[GameSoundList_BGM_InGame] = false;
		m_sound[GameSoundList_BGM_InGame] = nullptr;

		//3.ゲームオーバーBGM
		g_soundEngine->ResistWaveFileBank(GameSoundList_BGM_GameOver, "Assets/sound/bgm/gameOver.wav");
		m_isPlayingSound[GameSoundList_BGM_GameOver] = false;
		m_sound[GameSoundList_BGM_GameOver] = nullptr;

		//4.ゲームクリアBGM
		g_soundEngine->ResistWaveFileBank(GameSoundList_BGM_GameClear, "Assets/sound/bgm/gameClear.wav");
		m_isPlayingSound[GameSoundList_BGM_GameClear] = false;
		m_sound[GameSoundList_BGM_GameClear] = nullptr;

		//SE
		//0.選択SE
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Select, "Assets/sound/se/select.wav");
		m_isPlayingSound[GameSoundList_SE_Select] = false;
		m_sound[GameSoundList_SE_Select] = nullptr;

		//1.選択SE
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Cancel, "Assets/sound/se/cancel.wav");
		m_isPlayingSound[GameSoundList_SE_Cancel] = false;
		m_sound[GameSoundList_SE_Cancel] = nullptr;

		//2.決定SE
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Decision, "Assets/sound/se/decision.wav");
		m_isPlayingSound[GameSoundList_SE_Decision] = false;
		m_sound[GameSoundList_SE_Decision] = nullptr;

		//3.ゲームスタートSE
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_GameStart, "Assets/sound/se/gameStart.wav");
		m_isPlayingSound[GameSoundList_SE_GameStart] = false;
		m_sound[GameSoundList_SE_GameStart] = nullptr;

		//4.歩きSE
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Walk, "Assets/sound/se/walk.wav");
		m_isPlayingSound[GameSoundList_SE_Walk] = false;
		m_sound[GameSoundList_SE_Walk] = nullptr;

		//5.ダッシュSE
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Run, "Assets/sound/se/run.wav");
		m_isPlayingSound[GameSoundList_SE_Run] = false;
		m_sound[GameSoundList_SE_Run] = nullptr;

		//6.ジャンプSE
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Jump, "Assets/sound/se/jump.wav");
		m_isPlayingSound[GameSoundList_SE_Jump] = false;
		m_sound[GameSoundList_SE_Jump] = nullptr;

		//7.着地SE
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Landing, "Assets/sound/se/landing.wav");
		m_isPlayingSound[GameSoundList_SE_Landing] = false;
		m_sound[GameSoundList_SE_Landing] = nullptr;

		//8.スイング中SE
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Swing, "Assets/sound/se/swing.wav");
		m_isPlayingSound[GameSoundList_SE_Swing] = false;
		m_sound[GameSoundList_SE_Swing] = nullptr;

		//9.スイング後SE
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_AfterSwing, "Assets/sound/se/afterSwing.wav");
		m_isPlayingSound[GameSoundList_SE_AfterSwing] = false;
		m_sound[GameSoundList_SE_AfterSwing] = nullptr;

		//10.入力成功SE
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_InputSuccess, "Assets/sound/se/inputSuccess.wav");
		m_isPlayingSound[GameSoundList_SE_InputSuccess] = false;
		m_sound[GameSoundList_SE_InputSuccess] = nullptr;

		//11.入力失敗SE
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_InputFailed, "Assets/sound/se/inputFailed.wav");
		m_isPlayingSound[GameSoundList_SE_InputFailed] = false;
		m_sound[GameSoundList_SE_InputFailed] = nullptr;

		//12.QTEイベント成功SE
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_QteEventSuccess, "Assets/sound/se/qteEventSuccess.wav");
		m_isPlayingSound[GameSoundList_SE_QteEventSuccess] = false;
		m_sound[GameSoundList_SE_QteEventSuccess] = nullptr;

		//13.QTEイベント失敗SE
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_QteEventFailed, "Assets/sound/se/qteEventFailed.wav");
		m_isPlayingSound[GameSoundList_SE_QteEventFailed] = false;
		m_sound[GameSoundList_SE_QteEventFailed] = nullptr;

		//14.ビル衝突SE
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_BuildingCollision, "Assets/sound/se/buildingCollision.wav");
		m_isPlayingSound[GameSoundList_SE_BuildingCollision] = false;
		m_sound[GameSoundList_SE_BuildingCollision] = nullptr;

		//15.爆発SE
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Explosion, "Assets/sound/se/explosion.wav");
		m_isPlayingSound[GameSoundList_SE_Explosion] = false;
		m_sound[GameSoundList_SE_Explosion] = nullptr;

		//16.車エンジンSE
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_CarEngine, "Assets/sound/se/carEngine.wav");
		m_isPlayingSound[GameSoundList_SE_CarEngine] = false;
		m_sound[GameSoundList_SE_CarEngine] = nullptr;

		//17.タイムアップSE
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_TimeUp, "Assets/sound/se/timeUp.wav");
		m_isPlayingSound[GameSoundList_SE_TimeUp] = false;
		m_sound[GameSoundList_SE_TimeUp] = nullptr;
	}

	//BGMを再生
	void GameSoundEngine::PlayBGM(GameSoundList gameSoundList, float soundVolume, bool is3DSound)
	{
		if (!IsPlayingSound(gameSoundList))
		{
			m_sound[gameSoundList] = NewGO<SoundSource>(0);
			m_sound[gameSoundList]->Init(gameSoundList, is3DSound);
			m_sound[gameSoundList]->Play(true);
			m_sound[gameSoundList]->SetVolume(soundVolume);
			m_isPlayingSound[gameSoundList] = true;
			m_nowBGMPlaying = gameSoundList;
		}
	}

	//SEを再生
	void GameSoundEngine::PlaySE(GameSoundList gameSoundList, float soundVolume, bool is3DSound)
	{
		m_sound[gameSoundList] = NewGO<SoundSource>(0);
		m_sound[gameSoundList]->Init(gameSoundList, is3DSound);
		m_sound[gameSoundList]->Play(false);
		m_sound[gameSoundList]->SetVolume(soundVolume);
		m_isPlayingSound[gameSoundList] = true;
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

		//SEのみ処理をする
		for (int i = GameSoundList_SE_Select; i < GameSoundList_Num; i++)
		{
			//SEのサウンドインスタンスがnullptrだったら処理をしない
			if (m_sound[i] == nullptr)
			{
				continue;
			}

			//今再生しているサウンドインスタンスが削除したら
			if (m_sound[i]->IsDead())
			{
				//サウンドが再生しているかを判定するフラグをfalseにする
				m_isPlayingSound[i] = false;
				m_sound[i] = nullptr;
			}
		}
	}
}