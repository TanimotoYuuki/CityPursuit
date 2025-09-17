#pragma once
/// <summary>
/// ゲーム全体を管理するクラス
/// </summary>
class BackGround;
class Game : public IGameObject
{
public:
	Game() {};//コンストラクタ
	~Game() {};//デストラクタ

	bool Start();//開始処理
	void Update();//更新処理

private:
	BackGround* m_backGround;//背景用のインスタンス
};

