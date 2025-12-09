#pragma once
/// <summary>
/// 敵のエフェクトクラス
/// </summary>
class EnemyEffect : public IGameObject
{
public:
	EnemyEffect() {};//コンストラクタ
	~EnemyEffect();//デストラクタ

	bool Start();//開始処理

public://列挙型

	//敵のエフェクト一覧
	enum EnEnemyEffectList
	{
		enEnemyEffectList_EngineSmoke,//エンジンの煙
		enEnemyEffectList_EngineSmoke_Left,//エンジンの煙(左側)
		enEnemyEffectList_EngineSmoke_Right,//エンジンの煙(右側)
		enEnemyEffectList_EngineSmoke_Large,//エンジンの煙大
		enEnemyEffectList_EngineSmoke_Large_Left,//エンジンの煙大(左側)
		enEnemyEffectList_EngineSmoke_Large_Right,//エンジンの煙大(右側)
		enEnemyEffectList_Explosion,//爆発エフェクト
		enEnemyEffectList_None,//エフェクトなし
		enEnemyEffectList_Num//敵のエフェクト数
	};

private://メンバ関数

	/// <summary>
	/// エンジンの煙のエフェクトの再生処理
	/// </summary>
	/// <param name="position">敵の位置</param>
	/// <param name="rotation">敵の回転</param>
	void PlayEngineSmokeEffect(const Vector3& position, const Quaternion& rotation);

	/// <summary>
	/// エンジンの煙大のエフェクトの再生処理
	/// </summary>
	/// <param name="position">敵の位置</param>
	/// <param name="rotation">敵の回転</param>
	void PlayEngineSmokeLargeEffect(const Vector3& position, const Quaternion& rotation);

	/// <summary>
	/// 爆発エフェクトの再生処理
	/// </summary>
	/// <param name="position">敵の位置</param>
	void PlayExplosionEffect(const Vector3& position);

public://メンバ関数

	/// <summary>
	/// 実行処理
	/// </summary>
	/// <param name="position">敵の位置</param>
	/// <param name="rotation">敵の回転</param>
	void Execute(const Vector3& position, const Quaternion& rotation);

	/// <summary>
	/// エフェクトの再生
	/// </summary>
	/// <param name="currentPlayEffect">現在再生しているエフェクト</param>
	void PlayEffect(EnEnemyEffectList currentPlayEffect);

	/// <summary>
	/// エフェクトの停止
	/// </summary>
	/// <param name="formerPlayEffect">前回再生していたエフェクト</param>
	void StopEffect(EnEnemyEffectList formerPlayEffect);

	/// <summary>
	/// エフェクトの切り替え
	/// </summary>
	void ChangeEffect(EnEnemyEffectList enPlayerEffectList)
	{
		if (m_currentPlayEffect != enPlayerEffectList)
		{
			m_formerPlayEffect = m_currentPlayEffect;
			m_currentPlayEffect = enPlayerEffectList;
		}
	}

	/// <summary>
	/// 現在再生しているエフェクトの取得
	/// </summary>
	/// <returns>現在再生しているエフェクト</returns>
	EnEnemyEffectList IsPlayEffect() const
	{
		return m_currentPlayEffect;
	}

private://メンバ変数
	EnEnemyEffectList m_currentPlayEffect = enEnemyEffectList_None;//現在どのエフェクトを再生しているか?
	EnEnemyEffectList m_formerPlayEffect = enEnemyEffectList_None;//前回どのエフェクトを再生していたか?
	EffectEmitter* m_enemyEffect[enEnemyEffectList_Num] = { nullptr };//敵のエフェクトのインスタンス
};

