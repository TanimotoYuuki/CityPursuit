#pragma once
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

//ゲームサウンドリスト
enum GameSoundList
{
	GameSoundList_BGM_Title,//タイトル画面BGM
	GameSoundList_BGM_InGame,//ゲーム中BGM
	GameSoundList_BGM_GameOver,//ゲームオーバーBGM
	GameSoundList_BGM_GameClear,//ゲームクリアBGM
	GameSoundList_SE_Select,//選択SE
	GameSoundList_SE_Cancel,//キャンセルSE
	GameSoundList_SE_Decision,//決定SE
	GameSoundList_SE_GameStart,//ゲームスタートSE
	GameSoundList_SE_Walk,//歩きSE
	GameSoundList_SE_Run,//ダッシュSE
	GameSoundList_SE_Jump,//ジャンプSE
	GameSoundList_SE_Landing,//着地SE
	GameSoundList_SE_Swing,//スイング中SE
	GameSoundList_SE_AfterSwing,//スイング後SE
	GameSoundList_SE_InputSuccess,//入力成功SE
	GameSoundList_SE_InputFailed,//入力失敗SE
	GameSoundList_SE_QteEventSuccess,//QTEイベント結果成功SE
	GameSoundList_SE_QteEventFailed,//QTEイベント結果失敗SE
	GameSoundList_SE_BuildingCollision,//ビル衝突SE
	GameSoundList_SE_Explosion,//爆発SE
	GameSoundList_SE_CarEngine,//車エンジンSE
	GameSoundList_SE_TimeUp,//タイムアップSE
	GameSoundList_Num//ゲームサウンドリスト数
};

/// <summary>
/// サウンドエンジン
/// </summary>
namespace nsK2EngineLow
{
	class GameSoundEngine : public Noncopyable
	{
	public://メンバ関数
		/// <summary>
		/// 初期化
		/// </summary>
		void Init();

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();

		/// <summary>
		/// BGMを再生
		/// </summary>
		/// <param name="soundList">ゲームサウンドリスト</param>
		/// <param name="soundVolume">サウンドの音量</param>
		/// <param name="is3DSound">trueなら3Dサウンドにする</param>
		void PlayBGM(GameSoundList gameSoundList, float soundVolume, bool is3DSound = false);

		/// <summary>
		/// SEを再生(2Dサウンド)
		/// </summary>
		/// <param name="soundList">ゲームサウンドリスト</param>
		/// <param name="soundVolume">サウンドの音量</param>
		/// <param name="is3DSound">trueなら3Dサウンドにする</param>
		void PlaySE(GameSoundList gameSoundList, float soundVolume, bool is3DSound = false);

		/// <summary>
		/// 3Dサウンドの位置設定
		/// </summary>
		/// <param name="gameSoundList">ゲームサウンドリスト</param>
		/// <param name="position">位置</param>
		void SetPosition(GameSoundList gameSoundList, const Vector3& position)
		{
			m_sound[gameSoundList]->SetPosition(position);
		}

		/// <summary>
		/// サウンドの音量の設定
		/// </summary>
		/// <param name="gameSoundList">ゲームサウンドリスト</param>
		/// <param name="soundVolume">サウンドの音量</param>
		void SetVolume(GameSoundList gameSoundList, float soundVolume)
		{
			m_sound[gameSoundList]->SetVolume(soundVolume);
		}

		/// <summary>
		/// サウンドの音量を取得
		/// </summary>
		/// <param name="gameSoundList">ゲームサウンドリスト</param>
		/// <returns>サウンドの音量</returns>
		float GetVolume(GameSoundList gameSoundList)
		{
			return m_sound[gameSoundList]->GetVolume();
		}

		/// <summary>
		/// 再生中？
		/// </summary>
		/// <param name="gameSoundList">ゲームサウンドリスト</param>
		/// <returns>trueなら再生中</returns>
		bool IsPlayingSound(GameSoundList gameSoundList)
		{
			return m_isPlayingSound[gameSoundList];
		}

		/// <summary>
		/// サウンドの停止
		/// </summary>
		/// <param name="gameSoundList">ゲームサウンドリスト</param>
		void StopSound(GameSoundList gameSoundList)
		{
			m_sound[gameSoundList]->Stop();
		}

		/// <summary>
		/// サウンドの一時停止
		/// </summary>
		/// <param name="gameSoundList">ゲームサウンドリスト</param>
		void PauseSound(GameSoundList gameSoundList)
		{
			m_sound[gameSoundList]->Pause();
		}

		/// <summary>
		/// サウンド用インスタンスを取得
		/// </summary>
		/// <param name="gameSoundList">ゲームサウンドリスト</param>
		/// <returns>サウンド用インスタンス</returns>
		SoundSource* GetSoundInstance(GameSoundList gameSoundList)
		{
			return m_sound[gameSoundList];
		}

	public://シングルトン用のメンバ関数

		/// <summary>
		/// シングルトンインスタンスの生成
		/// </summary>
		static void CreateInstance()
		{
			if (m_instance == nullptr)
			{
				m_instance = new GameSoundEngine();
			}
		}

		/// <summary>
		/// シングルトンインスタンスの取得
		/// </summary>
		/// <returns>シングルトンインスタンス</returns>
		static GameSoundEngine* GetInstance()
		{
			return m_instance;
		}

		/// <summary>
		/// シングルトンインスタンスの削除
		/// </summary>
		static void DeleteInstance()
		{
			if (m_instance != nullptr)
			{
				delete m_instance;
				m_instance = nullptr;
			}
		}

	private:
		int			 m_nowBGMPlaying = -1;//今何のBGMが再生されているのか？
		bool		 m_isPlayingSound[GameSoundList_Num];//サウンドが再生されているか？
		SoundSource* m_sound[GameSoundList_Num];//サウンド再生用インスタンス
		static GameSoundEngine* m_instance;//シングルトン用インスタンス
	};
}

