#pragma once
/// <summary>
/// タイトルクラス
/// </summary>
class TitleBackGround;
class Player;
class TitleCamera;
class TitleSprite;
class TitleSelect;
class Title : public IGameObject
{
public:
	Title() {};//コンストラクタ
	~Title();//デストラクタ

	bool Start();//開始処理
	void Update();//更新処理

public://列挙型

	//タイトルの状態
	enum EnTitleState
	{
		enTitleState_Fade,//フェード
		enTitleState_TitleText,//タイトルのテキスト
		enTitleState_Select,//選択
		enTitleState_GameStartDirection,//ゲームスタート演出
		enTitleState_TransitionScene,//シーン遷移
		enTitleState_Num//タイトルの状態数
	};

private://メンバ関数

	/// <summary>
	/// フェードの更新処理
	/// </summary>
	void FadeUpdate();

	/// <summary>
	/// タイトルのテキストの更新処理
	/// </summary>
	void TitleTextUpdate();

	/// <summary>
	/// 選択の更新処理
	/// </summary>
	void SelectUpdate();

	/// <summary>
	/// ゲームスタート演出の更新処理
	/// </summary>
	void GameStartDirectionUpdate();

	/// <summary>
	/// シーンの遷移の更新処理
	/// </summary>
	void TransitionSceneUpdate();

public://メンバ関数

	/// <summary>
	/// タイトルの状態の取得
	/// </summary>
	/// <returns>タイトルの状態</returns>
	EnTitleState GetTitleState() const
	{
		return m_titleState;
	}

private:// メンバ変数
	bool m_isGameStartDirectionPlayerJump = false;//ゲームスタート演出用でプレイヤーがジャンプしているか?
	EnTitleState m_titleState = enTitleState_Fade;//タイトルの状態
	TitleBackGround* m_titleBackGround = nullptr;//タイトル背景用のインスタンス
	Player* m_player = nullptr;//プレイヤー用のインスタンス
	TitleCamera* m_titleCamera = nullptr;//タイトルカメラ用のインスタンス
	TitleSprite* m_titleSprite = nullptr;//タイトルのスプライトを表示する用のインスタンス
	TitleSelect* m_titleSelect = nullptr;//タイトルのときに選択する用のインスタンス
};

