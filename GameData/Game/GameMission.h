#pragma once
/// <summary>
/// ゲームミッションのスプライトを表示するクラス
/// </summary>
class GameMission : public IGameObject
{
public:
	GameMission() {};//コンストラクタ
	~GameMission() {};//デストラクタ

	bool Start();//開始処理
	void Render(RenderContext& rc);//描画処理

public://列挙型

	//敵の捕獲数を表示するUIの描画順番
	enum EnCaptureEnemyNumDrawingOrder
	{
		enCaptureEnemyNumDrawingOrder_CurrentCaptureEnemy,//現在の敵の捕獲数
		enCaptureEnemyNumDrawingOrder_Slash,//スラッシュ
		enCaptureEnemyNumDrawingOrder_QuotaCaptureEnemy,//ノルマの敵の捕獲数
		enCaptureEnemyNumDrawingOrder_Num,//敵の捕獲数を表示するUIの描画順番数
	};

	//敵の捕獲数で表示するUI
	enum EnCaptureEnemyNumDisplayUI
	{
		enCaptureEnemyNumDisplayUI_Zero,//0
		enCaptureEnemyNumDisplayUI_One,//1
		enCaptureEnemyNumDisplayUI_Two,//2
		enCaptureEnemyNumDisplayUI_Three,//3
		enCaptureEnemyNumDisplayUI_Four,//4
		enCaptureEnemyNumDisplayUI_Five,//5
		enCaptureEnemyNumDisplayUI_Six,//6
		enCaptureEnemyNumDisplayUI_Seven,//7
		enCaptureEnemyNumDisplayUI_Eight,//8
		enCaptureEnemyNumDisplayUI_Nine,//9
		enCaptureEnemyNumDisplayUI_Slash,//スラッシュ
		enCaptureEnemyNumDisplayUI_Num//敵の捕獲数で表示するUIの数
	};

private://メンバ関数

	/// <summary>
	/// 敵の捕獲数を表示するUIの初期化
	/// </summary>
	/// <param name="order">敵の捕獲数を表示するUIでの描画順番</param>
	/// <param name="displayUI">敵の捕獲数を表示するUI</param>
	void InitCaptureEnemyNumUI(EnCaptureEnemyNumDrawingOrder order, EnCaptureEnemyNumDisplayUI displayUI);

	/// <summary>
	/// ミッションテキストUIの初期化
	/// </summary>
	void InitMissionTextUI();

	/// <summary>
	/// ミッション内容テキストUIの初期化
	/// </summary>
	void InitMissionContentTextUI();

	/// <summary>
	/// 敵の捕獲数を表示するUIの更新処理
	/// </summary>
	void CaptureEnemyNumUIUpdate();

	/// <summary>
	/// 敵の捕獲数を表示するUIを表示するための計算
	/// </summary>
	void CalcCaptureEnemyNumUI();

	/// <summary>
	/// 敵の捕獲数を表示するUIの位置の更新処理
	/// </summary>
	/// <param name="captureEnemyNumData">敵の捕獲数を表示するUIに使うデータ</param>
	/// <param name="dataNum">敵の捕獲数を表示するUIに使うデータの順番</param>
	/// <param name="captureEnemyNumUI">敵の捕獲数を表示するUI</param>
	void CaptureEnemyNumUIPositionUpdate(std::vector<EnCaptureEnemyNumDisplayUI> captureEnemyNumData, int dataNum, SpriteRender& captureEnemyNumUI);

public://メンバ関数

	/// <summary>
	/// 実行処理
	/// </summary>
	void Execute();

	/// <summary>
	/// 全ての敵を捕獲したか?
	/// </summary>
	/// <returns>trueなら全ての敵を捕獲できている</returns>
	bool IsAllEnemyCaptureEnemy() const
	{
		return m_isAllEnemyCaptureEnemy;
	}

	/// <summary>
	/// 現在捕獲している敵の数の加算
	/// </summary>
	void AddCurrentCaptureEnemyNum()
	{
		m_currentCaptureEnemyNum++;
	}

	/// <summary>
	/// ノルマで捕獲する敵の数の加算
	/// </summary>
	void AddQuotaCaptureEnemyNum()
	{
		m_quotaCaptureEnemyNum++;
	}

	/// <summary>
	/// 現在捕獲している敵の数の取得
	/// </summary>
	/// <returns>現在捕獲している敵の数</returns>
	int GetCurrentCaptureEnemyNum() const
	{
		return m_currentCaptureEnemyNum;
	}

	/// <summary>
	/// ノルマで捕獲する敵の数の取得
	/// </summary>
	/// <returns>ノルマで捕獲する敵の数</returns>
	int GetQuotaCaptureEnemyNum() const
	{
		return m_quotaCaptureEnemyNum;
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

private://メンバ変数
	SpriteRender m_captureEnemyNumUI[enCaptureEnemyNumDrawingOrder_Num][enCaptureEnemyNumDisplayUI_Num];//敵の捕獲数を表示するUI
	SpriteRender m_missionTextUI;//ミッションテキストUI
	SpriteRender m_missionContentTextUI;//ミッション内容テキストUI
	int m_currentCaptureEnemyNum = 0;//現在捕獲している敵の数
	int m_quotaCaptureEnemyNum = 0;//ノルマで捕獲する敵の数
	bool m_isAllEnemyCaptureEnemy = false;//全ての敵を捕獲したか?
	bool m_isDrawingUI = false;//UIを描画するか?
	std::vector<EnCaptureEnemyNumDisplayUI> m_captureEnemyNumManage;//敵の捕獲数を管理する変数

private://表示するUIのファイルパス用のメンバ変数

	//敵の捕獲数を表示するUIのファイルパス
	const std::string m_captureEnemyNumUIFilePath[enCaptureEnemyNumDisplayUI_Num] = {
		"Assets/sprite/text/number/zero.dds",//0
		"Assets/sprite/text/number/one.dds",//1
		"Assets/sprite/text/number/two.dds",//2
		"Assets/sprite/text/number/three.dds",//3
		"Assets/sprite/text/number/four.dds",//4
		"Assets/sprite/text/number/five.dds",//5
		"Assets/sprite/text/number/six.dds",//6
		"Assets/sprite/text/number/seven.dds",//7
		"Assets/sprite/text/number/eight.dds",//8
		"Assets/sprite/text/number/nine.dds",//9
		"Assets/sprite/text/number/slash.dds",//スラッシュ
	};

	//ミッションテキストUIのファイルパス
	const std::string m_missionTextUIFilePath = "Assets/sprite/text/mission.dds";

	//ミッション内容テキストUIのファイルパス
	const std::string m_missionContextTextUIFilePath = "Assets/sprite/text/missionContent.dds";
};

