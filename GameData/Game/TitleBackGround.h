#pragma once
#include "Level3DRender.h"
#include "Buildings.h"
/// <summary>
/// タイトル背景クラス
/// </summary>
class Street;
class TitleBackGround : public IGameObject
{
public:
	TitleBackGround() {};//コンストラクタ
	~TitleBackGround() {};//デストラクタ

	bool Start();//開始処理
	void Update();//開始処理

private://メンバ関数
	void InitSkyCube();//スカイキューブの初期化
	void LoadLevel();//レベルの読み込み処理

public://メンバ関数

	/// <summary>
	/// タイトル用のビルの生成
	/// </summary>
	void CreateTitleBuilding()
	{
		m_titleBuilding = NewGO<Buildings>(0, "buildings");
		m_titleBuilding->SetPosition({ 1000.0f,2000.0f,-16000.0f });
		m_titleBuilding->SetScale({1.5f,1.0f,1.0f});
	}

	/// <summary>
	/// タイトル用のビルのポインタの取得
	/// </summary>
	/// <returns>タイトル用のビルのポインタ</returns>
	Buildings* GetTitleBuildingPtr() const
	{
		return m_titleBuilding;
	}

private://メンバ変数
	Level3DRender m_level3dRender;//レベル3Dレンダラー
	Street* m_street = nullptr;//道用のインスタンス
	Buildings* m_titleBuilding = nullptr;//タイトル用のビルの生成
	SkyCube* m_skyCube;//スカイキューブ用のインスタンス
};

