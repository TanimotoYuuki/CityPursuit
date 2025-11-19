#pragma once
#include "Level3DRender.h"
/// <summary>
/// 背景クラス
/// </summary>
class Game;
class Street;
class BackGround : public IGameObject
{
public:
	BackGround() {};//コンストラクタ
	~BackGround();//デストラクタ

	bool Start();//開始処理
	void Update();//開始処理

private://メンバ関数
	void InitSkyCube();//スカイキューブの初期化
	void LoadLevel();//レベルの読み込み処理
	
public://メンバ関数

	/// <summary>
	/// ゲーム全体を管理するクラスのポインタの設定
	/// </summary>
	/// <param name="game">ゲーム全体を管理するクラスのポインタ</param>
	void SetGamePtr(Game* game)
	{
		m_game = game;
	}

	/// <summary>
	/// ゲーム全体を管理するクラスのポインタの取得
	/// </summary>
	/// <returns>ゲーム全体を管理するクラスのポインタ</returns>
	Game* GetGamePtr() const
	{
		return m_game;
	}

private://メンバ変数
	Level3DRender m_level3dRender;//レベル3Dレンダラー
	Game* m_game = nullptr;//ゲーム全体を管理する用のインスタンス
	Street* m_street = nullptr;//道用のインスタンス
	SkyCube* m_skyCube;//スカイキューブ用のインスタンス
};

