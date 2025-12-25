#include "stdafx.h"
#include "Game.h"
#include "GameBackGround.h"
#include "Player.h"
#include "PlayerMove.h"
#include "Enemy.h"
#include "GameTimeLimit.h"
#include "GameMission.h"
#include "MiniMap.h"
#include "GameStartSprite.h"
#include "FirstEnemyDirection.h"
#include "SceneManager.h"
#include "FadeManager.h"
#include "Loading.h"
#include "DebugLog.h"

namespace {
	const int GAME_TIME_LIMIT = 300;//ゲームの制限時間

	const Vector3 PLAYER_IN_GAME_INIT_POSITION{ 0.0f,2000.0f,500.0f };//プレイヤーのインゲームでの初期位置

	const Vector3 PLAYER_IN_GAME_SCALE{ 2.0f,2.0f,2.0f };//プレイヤーのインゲームでの大きさ

	struct RayResultIgnoreCharacter : public btCollisionWorld::ClosestConvexResultCallback
	{
		RayResultIgnoreCharacter(const btVector3& rayFromWorld, const btVector3& rayToWorld)
			: btCollisionWorld::ClosestConvexResultCallback(rayFromWorld, rayToWorld) {}

		//衝突判定を行うかどうかのフィルター処理
		virtual bool needsCollision(btBroadphaseProxy* proxy0) const override
		{
			//ベースの判定（通常のマスク処理など）
			bool collides = btCollisionWorld::ClosestConvexResultCallback::needsCollision(proxy0);
			if (!collides) return false;

			//相手が btCollisionObject か確認
			btCollisionObject* co = (btCollisionObject*)proxy0->m_clientObject;

			//ユーザーインデックスが「Character」なら無視する（falseを返す）
			if (co->getUserIndex() == nsK2EngineLow::enCollisionAttr_Character) {
				return false;
			}

			//それ以外（GroundやWallなど）なら判定する
			return true;
		}
	};
}

//デストラクタ
Game::~Game()
{
	DeleteGO(m_gameBackGround);
	DeleteGO(m_gameTimeLimit);
	DeleteGO(m_gameMission);
	DeleteGO(m_miniMap);
}

//開始処理
bool Game::Start()
{
	m_gameBackGround = NewGO<GameBackGround>(0, "gamebackground");
	m_gameBackGround->SetGamePtr(this);
	m_player = FindGO<Player>("player");
	m_player->SetDirectPosition(PLAYER_IN_GAME_INIT_POSITION);
	m_player->SetScale(PLAYER_IN_GAME_SCALE);
	m_player->GetPlayerMove()->SetCanMove(false);
	m_player->GetPlayerMove()->SetCanJump(false);
	m_player->SetGamePtr(this);
	m_player->GetPlayerCamera().Reset(m_player);
	m_gameTimeLimit = NewGO<GameTimeLimit>(0, "gametimelimit");
	m_gameTimeLimit->SetGamePtr(this);
	m_gameTimeLimit->SetTimeLimit(GAME_TIME_LIMIT);
	m_gameTimeLimit->DisableDrawingUI();
	m_gameMission = NewGO<GameMission>(0, "gamemission");
	m_gameMission->DisableDrawingUI();
	m_miniMap = NewGO<MiniMap>(0, "minimap");
	m_miniMap->SetPlayerPtr(m_player);
	m_miniMap->DisableDrawingUI();
	m_gameStartSprite = NewGO<GameStartSprite>(0, "gamestartsprite");
	m_gameStartSprite->DisableDrawingUI();
	g_renderingEngine->GetGameEndPostEffect().SetDrawingGameEndPostEffect(GameEndPostEffect::enGameEndPostEffect_None);
	GameSoundEngine::GetInstance()->PlayBGM(GameSoundList_BGM_InGame, 1.0f);
	return true;
}

//更新処理
void Game::Update()
{
	//ロード完了していなければ
	if (!m_isLoadComplete)
	{
		//ロードの更新処理
		LoadingUpdate();
		return;
	}

	m_miniMap->Execute();//ミニマップの実行処理

	if (m_isStartGameStartDirection != true)
	{
		//フェードインが終わっていないとき
		if (!m_isFinsishFadeIn)
		{
			if (FadeManager::GetInstance()->IsFinishFade())
			{
				m_isFinsishFadeIn = true;
			}
		}
		//フェードインが終わっているとき
		else
		{
			if (g_gameTime->StopWatch(1.5f))
			{
				m_isStartGameStartDirection = true;
				m_gameTimeLimit->EnableDrawingUI();
				m_gameMission->EnableDrawingUI();
				m_miniMap->EnableDrawingUI();
				m_gameStartSprite->EnableDrawingUI();
				GameSoundEngine::GetInstance()->PlaySE(GameSoundList_SE_GameStart, 1.0f);
			}
		}
		return;
	}

	auto startSprite = FindGO<GameStartSprite>("gamestartsprite");
	if (startSprite != nullptr)
	{
		m_gameStartSprite->PlaySpriteAnimation();

		if (m_gameStartSprite->IsFinishScaleDownSpriteAnimation())
		{
			m_player->GetPlayerMove()->SetCanMove(true);
			m_player->GetPlayerMove()->SetCanJump(true);
			m_isFinishGameStartDirection = true;
		}
		else
		{
			return;
		}
	}

	//インゲーム内のデルタタイムの更新処理
	InGameDeltaTimeUpdate();

	//視錐台を用いた当たり判定の更新処理
	ViewFrustumCollisionUpdate();

	m_gameTimeLimit->Execute();//制限時間の実行処理
	m_gameMission->Execute();//ゲームミッションの実行処理

	if (m_gameMission->GetCurrentCaptureEnemyNum() == m_gameMission->GetQuotaCaptureEnemyNum() && 
		m_player->GetCharacterController().IsOnGround())
	//if(g_pad[0]->IsPress(enButtonX))//デバッグ用
	{
		if (!m_isGameEnd)
		{
			if (g_gameTime->StopWatch(0.3f))
			{
				SceneManager::GetInstance()->CreateScene(SceneManager::enSceneID_GameClear);
				m_gameTimeLimit->DisableDrawingUI();
				m_gameMission->DisableDrawingUI();
				m_miniMap->DisableDrawingUI();
				m_isGameEnd = true;
			}
		}
	}


	if(m_gameTimeLimit->IsTimeUp())
	//if (g_pad[0]->IsTrigger(enButtonX))//デバッグ用
	{
		SceneManager::GetInstance()->CreateScene(SceneManager::enSceneID_GameOver);
		m_gameMission->DisableDrawingUI();
		m_miniMap->DisableDrawingUI();
		m_isGameEnd = true;
	}
	DebugLog::GetInstance()->Update();
}

//描画処理
void Game::Render(RenderContext& rc)
{
	DebugLog::GetInstance()->Render(rc);
}

//ロードの更新処理
void Game::LoadingUpdate()
{
	//1.5秒経過したらフェードインに移行
	if (g_gameTime->StopWatch(1.5f))
	{
		FadeManager::GetInstance()->SetFadeState(FadeManager::enFadeState_FadeIn);
		Loading::GetInstance()->FinishLoading();
		m_isLoadComplete = true;
	}
}

//インゲーム内のデルタタイムの更新処理
void Game::InGameDeltaTimeUpdate()
{
	if (m_gameTimeLimit->IsTimeUp() || IsFirstEnemyDirection())
	{
		if (m_firstEnemyDirection != nullptr)
		{
			if (m_firstEnemyDirection->IsDead())
			{
				m_firstEnemyDirection = nullptr;
				m_gameTimeLimit->EnableDrawingUI();
				m_gameMission->EnableDrawingUI();
				m_miniMap->EnableDrawingUI();
			}
		}

		m_inGameDeltaTime = 0.0f;
	}
	else
	{
		m_inGameDeltaTime = g_gameTime->GetFrameDeltaTime();
	}
}

//視錐台を用いた当たり判定の更新処理
void Game::ViewFrustumCollisionUpdate()
{
	//インゲーム用のカメラを取得
	Camera* inGameCam = m_player->GetPlayerCamera().GetSpringCamera().GetCamera();

	//インゲーム用のカメラが存在すれば視錐台を更新
	if (inGameCam)
	{
		m_viewFrustum.Update(*inGameCam);
	}
	else
	{
		return;//カメラがない場合は処理しない
	}

	Vector3 cameraPos = inGameCam->GetPosition();

	//敵のゲームオブジェクトを全て取得
	auto& enemys = FindGOs<Enemy>("enemy");
	//敵全てに対して視錐台との当たり判定を行う
	for (auto& enemy : enemys)
	{
		Vector3 enemyPos = enemy->GetPosition();

		//敵が視錐台内に存在するかどうかを判定
		bool isVisible = m_viewFrustum.CheckSphere(enemyPos, 100.0f);

		//敵が視錐台内に存在していて、かつプレイヤーから一定距離以内であれば最初の敵発見演出を開始
		if (isVisible)
		{
			//最初の敵発見演出がまだ開始されていなければ
			if (!m_isFirstEnemySee)
			{
				//レイキャストによる遮蔽判定

				//球体のコライダーの作成
				SphereCollider checkSphere;
				checkSphere.Create(1.0f);

				//レイの起点・終点の作成
				btTransform btStart, btEnd;
				btStart.setIdentity();
				btEnd.setIdentity();
				btStart.setOrigin(btVector3(cameraPos.x, cameraPos.y, cameraPos.z));

				Vector3 targetPos = enemyPos;
				targetPos.y += 80.0f;
				btEnd.setOrigin(btVector3(targetPos.x, targetPos.y, targetPos.z));

				//コールバックの作成
				RayResultIgnoreCharacter callBack(btStart.getOrigin(), btEnd.getOrigin());
				PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)checkSphere.GetBody(), btStart, btEnd, callBack);
				bool isHitWall = callBack.hasHit();
				if (!isHitWall)
				{
					m_firstEnemyDirection = NewGO<FirstEnemyDirection>(0, "firstenemydirection");
					m_firstEnemyDirection->SetPlayerPtr(m_player);
					m_firstEnemyDirection->SetEnemyPtr(enemy);
					m_gameTimeLimit->DisableDrawingUI();
					m_gameMission->DisableDrawingUI();
					m_miniMap->DisableDrawingUI();
					m_isFirstEnemySee = true;
				}
			}
		}
	}
}