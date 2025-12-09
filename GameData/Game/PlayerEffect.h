#pragma once
/// <summary>
/// プレイヤーのエフェクトクラス
/// </summary>
class PlayerEffect : public IGameObject
{
public:
	PlayerEffect() {};//コンストラクタ
	~PlayerEffect();//デストラクタ

	bool Start();//開始処理

public://列挙型

	//プレイヤーのエフェクト一覧
	enum EnPlayerEffectList
	{
		enPlayerEffectList_Swing,//スイング
		enPlayerEffectList_None,//エフェクトなし
		enPlayerEffectList_Num//プレイヤーのエフェクト数
	};


public://メンバ関数

	/// <summary>
	/// 実行処理
	/// </summary>
	/// <param name="position">プレイヤーの位置</param>
	/// <param name="rotation">エフェクトの回転</param>
	void Execute(const Vector3& position, const Quaternion& rotation);

	/// <summary>
	/// エフェクトの再生
	/// </summary>
	void PlayEffect()
	{
		if (m_playerEffect[m_currentPlayEffect] != nullptr)
		{
			m_playerEffect[m_currentPlayEffect]->Play();
		}
	}

	/// <summary>
	/// エフェクトの停止
	/// </summary>
	void StopEffect()
	{
		if (m_playerEffect[m_formerPlayEffect] != nullptr)
		{
			m_playerEffect[m_formerPlayEffect]->Stop();
		}
	}

	/// <summary>
	/// エフェクトの切り替え
	/// </summary>
	void ChangeEffect(EnPlayerEffectList enPlayerEffectList)
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
	EnPlayerEffectList IsPlayEffect() const
	{
		return m_currentPlayEffect;
	}
	
private://メンバ変数
	EnPlayerEffectList m_currentPlayEffect = enPlayerEffectList_None;//現在どのエフェクトを再生しているか?
	EnPlayerEffectList m_formerPlayEffect = enPlayerEffectList_None;//前回どのエフェクトを再生していたか?
	EffectEmitter* m_playerEffect[enPlayerEffectList_Num] = { nullptr };//プレイヤーのエフェクトのインスタンス
};

