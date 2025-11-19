#pragma once
/// <summary>
/// タイトルクラス
/// </summary>
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

private:// メンバ変数
	GameTime m_stopwatch;//ストップウォッチ
	EnTitleState m_titleState = enTitleState_Fade;//タイトルの状態
	TitleSprite* m_titleSprite = nullptr;//タイトルのスプライトを表示する用のインスタンス
	TitleSelect* m_titleSelect = nullptr;//タイトルのときに選択する用のインスタンス
};

