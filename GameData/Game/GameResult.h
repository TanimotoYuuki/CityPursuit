#pragma once
#include "SpriteAnimation.h"
/// <summary>
/// ゲームリザルトクラス
/// </summary>
class GameResult : public IGameObject
{
public:
	GameResult() {};//コンストラクタ
	~GameResult() {};//デストラクタ

	bool Start();//開始処理
	void Render(RenderContext& rc);//描画処理

public://列挙型

	//リザルトで表示するUI
	enum EnResultDisplayUI
	{
		enResultDisplayUI_Zero,//0
		enResultDisplayUI_One,//1
		enResultDisplayUI_Two,//2
		enResultDisplayUI_Three,//3
		enResultDisplayUI_Four,//4
		enResultDisplayUI_Five,//5
		enResultDisplayUI_Six,//6
		enResultDisplayUI_Seven,//7
		enResultDisplayUI_Eight,//8
		enResultDisplayUI_Nine,//9
		enResultDisplayUI_Count,//回
		enResultDisplayUI_Colon,//〇分と〇秒の間のコロン
		enResultDisplayUI_Num//リザルトで描画するUIの数
	};

	//〇の位
	enum EnPlace
	{
		enPlace_Ten,//十の位
		enPlace_One,//一の位
		enPlace_Count,//回数
		enPlace_Num//位の数
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

	//リザルトの演出
	enum EnResultDirection
	{
		enResultDirection_ResultUI,//リザルトUIの演出
		enResultDirection_QteSuccessCountUI,//QTEイベントで成功した回数のUIの演出
		enResultDirection_QteFailedCountUI,//QTEイベントで失敗した回数のUIの演出
		enResultDirection_GameClearTimeLimitUI,//ゲームクリアしたときの残り時間のUIの演出
		enResultDirection_Num//演出数
	};

private://メンバ関数(初期化)

	/// <summary>
	/// リザルトUIの初期化
	/// </summary>
	void InitResultUI();

	/// <summary>
	/// QTEイベントで成功した回数を表示するUIの初期化
	/// </summary>
	/// <param name="enPlace">〇の位</param>
	/// <param name="enResultDrawingUI">リザルトで表示するUI</param>
	void InitQteEventSuccessCountUI(EnPlace enPlace, EnResultDisplayUI enResultDrawingUI);
	
	/// <summary>
	/// QTEイベントで失敗した回数を表示するUIの初期化
	/// </summary>
	/// <param name="enPlace">〇の位</param>
	/// <param name="enResultDrawingUI">リザルトで表示するUI</param>
	void InitQteEventFailedCountUI(EnPlace enPlace, EnResultDisplayUI enResultDrawingUI);

	/// <summary>
	/// ゲームクリアしたときの残り時間を表示するUIの初期化
	/// </summary>
	/// <param name="enTime">時間</param>
	/// <param name="enResultDrawingUI">リザルトで表示するUI</param>
	void InitGameClearTimeLimitUI(EnTime enTime, EnResultDisplayUI enResultDrawingUI);

private://メンバ関数(準備処理)

	/// <summary>
	/// リザルトで描画するUIの準備処理
	/// </summary>
	void PreResultDrawingUI();

	/// <summary>
	/// リザルトでQTEイベントの成功と失敗の回数を表示するUIの準備処理
	/// </summary>
	void PreQteEventResultUI();

	/// <summary>
	/// リザルトでクリアしたときの残り時間を表示するUIの準備処理
	/// </summary>
	void PreGameClearTimeLimitUI();

private://メンバ関数(計算処理)

	/// <summary>
	/// QTEイベントで成功と失敗した回数のUIを表示するための計算
	/// </summary>
	/// <param name="resultData">QTEイベントで成功と失敗した回数のデータ</param>
	/// <param name="resultDataManage">リザルトのデータを管理する変数</param>
	void CalcDrawingQteEventResultUI(int resultData, std::vector<EnResultDisplayUI>& resultDataManage);

	/// <summary>
	/// ゲームクリアしたときの残り時間UIを表示するための計算
	/// </summary>
	void CalcDrawingGameClearTimeLimitUI();

private://メンバ関数(更新処理)

	/// <summary>
	/// リザルトで描画するUIの位置の更新処理
	/// </summary>
	/// <param name="resultData">リザルトで描画するUIに使うデータ</param>
	/// <param name="dataNum">リザルトで描画するUIに使うデータの順番</param>
	/// <param name="resultUI">リザルトで描画するUI</param>
	/// <param name="IntervalPosition">描画するUIの間隔位置</param>
	void ResultDrawingUIPositionUpdate(std::vector<EnResultDisplayUI> resultData, int dataNum, SpriteRender& resultUI, float intervalPosition);

	/// <summary>
	/// 入力の更新処理
	/// </summary>
	void InputUpdate();

private://メンバ関数(演出処理)

	/// <summary>
	/// リザルトの演出
	/// </summary>
	void ResultDirection();

	/// <summary>
	/// 演出のスキップ処理
	/// </summary>
	void DirectionSkip();

	/// <summary>
	/// リザルトUIの演出
	/// </summary>
	void ResultUIDirection();

	/// <summary>
	/// QTEイベントで成功した回数のUIの演出
	/// </summary>
	void QteSuccessCountUIDirection();

	/// <summary>
	/// QTEイベントで失敗した回数のUIの演出
	/// </summary>
	void QteFailedCountUIDirection();

	/// <summary>
	/// ゲームクリアしたときの残り時間のUIの演出
	/// </summary>
	void GameClearTimeLimitUIDirection();

private://メンバ関数(再生処理)

	/// <summary>
	/// 透明度(不透明から透明)を変えるアニメーションを再生
	/// </summary>
	void PlayNoneAlphaAnimation();

public://メンバ関数

	/// <summary>
	/// リザルトの実行処理
	/// </summary>
	void Execute();

	/// <summary>
	/// QTEイベントで成功した回数の設定
	/// </summary>
	/// <param name="successCount">QTEイベントで成功した回数</param>
	void SetQteEventSuccessCount(int successCount)
	{
		m_qteEventSuccessCount = successCount;
	}

	/// <summary>
	/// QTEイベントで失敗した回数の設定
	/// </summary>
	/// <param name="failedCount">QTEイベントで失敗した回数</param>
	void SetQteEventFailedCount(int failedCount)
	{
		m_qteEventFaileCount = failedCount;
	}

	/// <summary>
	/// ゲームクリアしたときの残り時間の設定
	/// </summary>
	/// <param name="timeLimit">ゲームクリアしたときの残り時間</param>
	void SetGameClearTimeLimit(float timeLimit)
	{
		m_gameClearTimeLimit = timeLimit;
	}

	/// <summary>
	/// 透明度(不透明から透明)を変えるアニメーションを再生しているか?
	/// </summary>
	/// <returns>trueなら透明度(不透明から透明)を変えるアニメーションを再生している</returns>
	bool IsPlayNoneAlphaAnimation() const
	{
		return m_isPlayNoneAlphaAnimation;
	}

	/// <summary>
	/// 透明度(不透明から透明)を変えるアニメーションを再生終了したか?
	/// </summary>
	/// <returns>trueなら透明度(不透明から透明)を変えるアニメーションを再生終了している</returns>
	bool IsFinishNoneAlphaAnimation() const
	{
		return m_isFinishNoneAlphaAnimation;
	}

private://メンバ変数
	SpriteRender m_resultUI;//リザルトUI
	SpriteRender m_qteEventSuccessCountUI[enPlace_Num][enResultDisplayUI_Num];//QTEイベントで成功した回数を表示するUI
	SpriteRender m_qteEventFailedCountUI[enPlace_Num][enResultDisplayUI_Num];//QTEイベントで失敗した回数を表示するUI
	SpriteRender m_gameClearTimeLimitUI[enTime_Num][enResultDisplayUI_Num];//ゲームクリアしたときの残り時間を表示するUI
	int m_qteEventSuccessCount = 0;//QTEイベントで成功した回数
	int m_qteEventFaileCount = 0;//QTEイベントで失敗した回数
	float m_gameClearTimeLimit = 0.0f;//ゲームクリアしたときの残り時間
	bool m_isDrawingUI[enResultDirection_Num] = { false };//UIを描画するか?
	bool m_isFinishAllResultDirection = false;//全てのリザルトの演出が終わったか?
	bool m_isPlayNoneAlphaAnimation = false;//透明度(不透明から透明)を変えるアニメーションを再生しているか?
	bool m_isFinishNoneAlphaAnimation = false;//透明度(不透明から透明)を変えるアニメーションを再生終了したか?
	bool m_isPlaySwipeSe = false;//スワイプSEが再生されているか?
	EnResultDirection m_resultDirectionState = enResultDirection_ResultUI;//リザルトの演出ステート
	std::vector<EnResultDisplayUI> m_qteEventSuccessCountManage;//QTEイベントで成功した回数を管理する変数
	std::vector<EnResultDisplayUI> m_qteEventFailedCountManage;//QTEイベントで失敗した回数を管理する変数
	std::vector<EnResultDisplayUI> m_gameClearTimeLimitManage;//ゲームクリアしたときの残り時間を管理する変数
	std::unique_ptr<PositionSpriteAnimation> m_resultUIPositionAnimation;//リザルト用UIの位置を変えるアニメーションのユニークポインタ
	std::unique_ptr<AlphaSpriteAnimation> m_resultUIFullAlphaAnimation;//リザルトUI用の透明度(透明から不透明)を変えるアニメーションのユニークポインタ
	std::unique_ptr<AlphaSpriteAnimation> m_resultUINoneAlphaAnimation;//リザルトUI用の透明度(不透明から透明)を変えるアニメーションのユニークポインタ
	std::unique_ptr<AlphaSpriteAnimation> m_qteEventSuccessCountUINoneAlphaAnimation[enPlace_Num][enResultDisplayUI_Num];//QTEイベントで成功回数を表示するUI用の透明度(不透明から透明)を変えるアニメーションのユニークポインタ
	std::unique_ptr<AlphaSpriteAnimation> m_qteEventFailedCountUINoneAlphaAnimation[enPlace_Num][enResultDisplayUI_Num];//QTEイベントで失敗回数を表示するUI用の透明度(不透明から透明)を変えるアニメーションのユニークポインタ
	std::unique_ptr<AlphaSpriteAnimation> m_gameClearTimeLimitUINoneAlphaAnimation[enTime_Num][enResultDisplayUI_Num];//ゲームクリアしたときの残り時間を表示するUI用の透明度(不透明から透明)を変えるアニメーションのユニークポインタ

private://表示するUIのファイルパス用のメンバ変数

	//リザルトUIのファイルパス
	const std::string m_resultUIFilePath = "Assets/sprite/result/result.dds";

	//リザルトで表示するUIのファイルパス
	const std::string m_resultDisplayUIFilePath[enResultDisplayUI_Num] = {
		"Assets/sprite/result/count/zero.dds",//0
		"Assets/sprite/result/count/one.dds",//1
		"Assets/sprite/result/count/two.dds",//2
		"Assets/sprite/result/count/three.dds",//3
		"Assets/sprite/result/count/four.dds",//4
		"Assets/sprite/result/count/five.dds",//5
		"Assets/sprite/result/count/six.dds",//6
		"Assets/sprite/result/count/seven.dds",//7
		"Assets/sprite/result/count/eight.dds",//8
		"Assets/sprite/result/count/nine.dds",//9
		"Assets/sprite/result/count/count.dds",//回
		"Assets/sprite/result/time/colon.dds",//〇分と〇秒の間のコロン
	};
};

