#pragma once
/// <summary>
/// ミニマップクラス
/// </summary>
class Player;
class Enemy;
class MiniMap : public IGameObject
{
public:
	MiniMap() {};// コンストラクタ
	~MiniMap() {};// デストラクタ

	bool Start();//開始処理
	void Render(RenderContext& rc);//描画処理

private://メンバ関数

	/// <summary>
	/// ワールド座標系からマップ座標系に変換
	/// </summary>
	/// <param name="worldCenterPosition">マップの中心とするオブジェクトのワールド座標</param>
	/// <param name="worldPosition">マップに表示したいオブジェクトのワールド座標</param>
	/// <param name="mapPosition">変換した後のマップ座標</param>
	/// <returns>マップに表示する座標ならtrue</returns>
	void WorldPositionConvertToMapPosition(Vector3 worldCenterPosition, Vector3 worldPosition, Vector3& mapPosition);

public://メンバ関数

	/// <summary>
	/// 実行処理
	/// </summary>
	void Execute();

	/// <summary>
	/// プレイヤークラスのポインタの設定
	/// </summary>
	/// <param name="player">プレイヤークラスのポインタ</param>
	void SetPlayerPtr(Player* player)
	{
		m_player = player;
	}

	/// <summary>
	/// 敵クラスのポインタの設定
	/// </summary>
	/// <param name="enemy">敵クラスのポインタ</param>
	void SetEnemyPtr(Enemy* enemy)
	{
		m_enemy.push_back(enemy);
	}

	/// <summary>
	/// 敵クラスのポインタの削除
	/// </summary>
	/// <param name="enemy">敵クラスのポインタ</param>
	void DeleteEnemyPtr(Enemy* enemy)
	{
		auto it = std::remove(m_enemy.begin(), m_enemy.end(), enemy);
		m_enemy.erase(it);
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
	SpriteRender m_miniMapUI;//ミニマップUI
	SpriteRender m_currentPlayerUI;//プレイヤーの現在地UI
	SpriteRender m_currentEnemyUI[3];//敵の現在地UI
	Player* m_player = nullptr;//プレイヤー用のインスタンス
	std::vector<Enemy*> m_enemy;//敵用のインスタンス
	bool m_isDrawingUI = false;//UIを描画するか?
};

