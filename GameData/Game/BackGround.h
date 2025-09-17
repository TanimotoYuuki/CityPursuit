#pragma once
#include "Level3DRender.h"
/// <summary>
/// 背景クラス
/// </summary>
class BackGround : public IGameObject
{
public:
	BackGround() {};//コンストラクタ
	~BackGround() {};//デストラクタ

	bool Start();//開始処理
	void Update();//開始処理

private://メンバ関数
	void InitSkyCube();//スカイキューブの初期化
	void LoadLevel();//レベルの読み込み処理
	
private://メンバ変数
	Level3DRender m_level3dRender;//レベル3Dレンダラー
	SkyCube* m_skyCube;//スカイキューブ用のインスタンス
};

