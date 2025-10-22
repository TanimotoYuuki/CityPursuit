#pragma once
/// <summary>
/// プレイヤーアニメーションクラス
/// </summary>
class Player;
class PlayerAnimation : public IGameObject
{
public:
	PlayerAnimation() {};
	~PlayerAnimation() {};

public://列挙型
	
	//アニメーションリスト
	enum EnAnimationList
	{
		enAnimationList_Idle,//待機アニメーション
		enAnimationList_Walk,//歩きアニメーション
		enAnimationList_Run,//ダッシュアニメーション
		enAnimationList_Jumping,//ジャンプ中アニメーション
		enAnimationList_Falling,//落下中アニメーション
		enAnimationList_SwingLeft,//スイング(左側)アニメーション
		enAnimationList_SwingRight,//スイング(右側)アニメーション
		enAnimationList_QteEvent,//QTEイベント用のアニメーション
		enAnimationList_Num//アニメーション数
	};

public://メンバ関数

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// アニメーションの実行処理
	/// </summary>
	/// <param name="modeldata">モデルのデータ</param>
	/// <param name="playerData">プレイヤー移動クラスのデータ</param>
	void Execute(ModelRender& modeldata, Player* playerData);

	/// <summary>
	/// アニメーションの切り替え
	/// </summary>
	/// <param name="modeldata">モデルのデータ</param>
	/// <param name="playerData">プレイヤークラスのデータ</param>
	void ChangeAnimation(ModelRender& modeldata, Player* playerData);

	/// <summary>
	/// アニメーションクリップの取得
	/// </summary>
	/// <returns>アニメーションクリップの取得</returns>
	AnimationClip* GetAnimationClips()
	{
		return m_animationClips;
	}

private://メンバ変数
	AnimationClip m_animationClips[enAnimationList_Num];//アニメーションリスト
	EnAnimationList m_nowPlayAnimation = enAnimationList_Idle;//現在再生しているアニメーション
	float m_animationSpeed = 0.0f;//アニメーションの再生速度
	float m_animationInterpolateTime = 0.0f;//アニメーションの補間時間

private://アニメーションのファイルパス
	const std::string m_animationFilePath[enAnimationList_Num] = {
		"Assets/animData/playerIdle.tka",//待機アニメーション
		"Assets/animData/playerWalk.tka",//歩きアニメーション
		"Assets/animData/playerRun.tka",//ダッシュアニメーション
		"Assets/animData/playerJumping.tka",//ジャンプ中アニメーション
		"Assets/animData/playerFalling.tka",//落下中アニメーション
		"Assets/animData/playerSwingLeft.tka",//スイング(左側)アニメーション
		"Assets/animData/playerSwingRight.tka",//ジャンプ(右側)アニメーション
		"Assets/animData/qteEvent.tka",//QTEイベント用のアニメーション
	};
};

