#pragma once
#include "SpriteAnimation.h"
/// <summary>
/// ゲームの制限時間を管理するクラス
/// </summary>
class GameTimeLimit :public IGameObject
{
public:
	GameTimeLimit() {};//コンストラクタ
	~GameTimeLimit() {};//デストラクタ

	bool Start();//開始処理
	void Render(RenderContext& rc);//描画処理

public://列挙型

	//制限時間で表示するUI
	enum EnTimeLimitDisplayUI
	{
		enTimeLimitDisplayUI_Zero,//0
		enTimeLimitDisplayUI_One,//1
		enTimeLimitDisplayUI_Two,//2
		enTimeLimitDisplayUI_Three,//3
		enTimeLimitDisplayUI_Four,//4
		enTimeLimitDisplayUI_Five,//5
		enTimeLimitDisplayUI_Six,//6
		enTimeLimitDisplayUI_Seven,//7
		enTimeLimitDisplayUI_Eight,//8
		enTimeLimitDisplayUI_Nine,//9
		enTimeLimitDisplayUI_Colon,//〇分と〇秒の間のコロン
		enTimeLimitDisplayUI_Num//リザルトで描画するUIの数
	};

	//時間
	enum EnTime
	{
		enTime_Minute,//〇分
		enTime_Colon,//〇分と〇秒の間のコロン
		enTime_Second_Ten,//〇秒の十の位
		enTime_Second_One,//〇秒の一の位
		enTime_Num//時間の数
	};

	//スプライトの透明度の状態
	enum EnSpriteAlphaState
	{
		enSpriteAlphaState_Opaque,//不透明
		enSpriteAlphaState_HalfTransparent,//半透明
		enSpriteAlphaState_Num//状態数
	};

private://メンバ関数

	/// <summary>
	/// 制限時間UIの初期化
	/// </summary>
	/// <param name="enTime">時間I</param>
	/// <param name="enTimeLimitDisp">制限時間で表示するUI</param>
	void InitTimeLimitUI(EnTime enTime, EnTimeLimitDisplayUI enTimeLimitDisp);

	/// <summary>
	/// 制限時間UIの更新処理
	/// </summary>
	void TimeLimitUIUpdate();

	/// <summary>
	/// 制限時間UIを表示するための計算
	/// </summary>
	void CalcDrawingTimeLimitUI();

	/// <summary>
	/// 制限時間UIの位置の更新処理
	/// </summary>
	/// <param name="timeLimitData">制限時間UIに使うデータ</param>
	/// <param name="dataNum">制限時間UIに使うデータの順番</param>
	/// <param name="timeLimitUI">制限時間UI</param>
	void TimeLimitUIPositionUpdate(std::vector<EnTimeLimitDisplayUI> timeLimitData, int dataNum, SpriteRender& timeLimitUI);

public://メンバ関数

	/// <summary>
	/// 制限時間の実行処理
	/// </summary>
	void Execute();

	/// <summary>
	/// 制限時間の設定
	/// </summary>
	/// <param name="timeLimit">制限時間</param>
	void SetTimeLimit(float timeLimit)
	{
		m_timeLimit = timeLimit;
	}

	/// <summary>
	/// 制限時間の取得
	/// </summary>
	/// <returns>制限時間</returns>
	float GetTimeLimit() const
	{
		return m_timeLimit;
	}

	/// <summary>
	/// タイムアップしたかどうか
	/// </summary>
	/// <returns>trueならタイムアップしている</returns>
	bool IsTimeUp() const
	{
		return m_isTimeUp;
	}

	/// <summary>
	/// UIを描画する
	/// </summary>
	void EnableDrawingUI()
	{
		m_isDrawingUI = true;
	}

	/// <summary>
	/// UIを描画しない
	/// </summary>
	void DisableDrawingUI()
	{
		m_isDrawingUI = false;
	}

	/// <summary>
	/// 時間を止める
	/// </summary>
	void EnableTimeStop()
	{
		m_isTimeStop = true;
	}

	/// <summary>
	/// 時間を止めない
	/// </summary>
	void DisableTimeStop()
	{
		m_isTimeStop = false;
	}

private://メンバ変数
	SpriteRender m_timeLimitUI[enTime_Num][enTimeLimitDisplayUI_Num];//制限時間UI
	float m_timeLimit = 0.0f;//制限時間
	bool m_isTimeUp = false;//タイムアップしたかどうか?
	bool m_isDrawingUI = false;//UIを描画するか?
	bool m_isTimeStop = false;//時間を止めるか?
	std::vector<EnTimeLimitDisplayUI> m_timeLimitManage;//制限時間を管理する変数

private://表示するUIのファイルパス用のメンバ変数

	//制限時間で表示するUIのファイルパス
	const std::string m_timeLimitDisplayUIFilePath[enTimeLimitDisplayUI_Num] = {
		"Assets/sprite/timeLimit/time/zero.dds",//0
		"Assets/sprite/timeLimit/time/one.dds",//1
		"Assets/sprite/timeLimit/time/two.dds",//2
		"Assets/sprite/timeLimit/time/three.dds",//3
		"Assets/sprite/timeLimit/time/four.dds",//4
		"Assets/sprite/timeLimit/time/five.dds",//5
		"Assets/sprite/timeLimit/time/six.dds",//6
		"Assets/sprite/timeLimit/time/seven.dds",//7
		"Assets/sprite/timeLimit/time/eight.dds",//8
		"Assets/sprite/timeLimit/time/nine.dds",//9
		"Assets/sprite/timeLimit/time/colon.dds",//〇分と〇秒の間のコロン
	};
};

