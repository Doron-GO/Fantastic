#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Common/Flame.h"
#include "../Input/Controller.h"
#include "../Object/Common/Capsule.h"
#include "../Object/Common/Collider.h"
#include "../Object/SkyDome.h"
#include "../Object/Stage.h"
#include "../Object/StagePiece.h"
#include "../Object/Player.h"
#include "GameScene.h"
#include "../Object/Common/Grid.h"
#include "../Object/SwingPoint.h"
#include "../Object/Coin.h"

GameScene::GameScene(void)
	:SceneBase()
{
	player_ = nullptr;
}

void GameScene::Init(void)
{
	isLoaded = false;
	//ステージ
	stage_ = std::make_unique<Stage>();
	//プレイヤー
	player_ = new Player();
	// スカイドーム
	skyDome_ = std::make_unique<SkyDome>();
	// グリッド線
	grid_ = std::make_unique<Grid>();
	swi_ =std::make_unique<SwingPoint>();

	stage_->Init();
	swi_->Load();

	player_->Init();

	//コイン
	MakeCoin();
	for (const auto c : coins_)
	{
		c->Init();
	}

	isHitNum_ = 0;



	//一時的	
	
	//for (int i =1; i<21;i++ )
	//{ 
	//	auto p = stage_->GetPiece();
	//	auto oo = p[static_cast<Stage::STAGE_NUM>(i-1)];
	//	player_->AddCollider(oo->GetTransform()->collider);
	//}

	for (int i = 1; i <= 4; i++)
	{
		auto p = stage_->GetPiece();
		auto oo = p[static_cast<Stage::STAGE_NUM>(i - 1)];
		player_->AddCollider(oo->GetTransform()->collider);
	}



	skyDome_->Init();

	grid_->Init();
	//カメラの設定
	lpSceneMng.GetCamera()->ChangeMode(Camera::MODE::FOLLOW);

	lpSceneMng.GetCamera()->SetTransform(player_->GetTransform());
	isLoaded_ = true;

	//loadThread_ = std::thread(&GameScene::Load, this);
	//Load();
}

void GameScene::Update(void)
{
	
	//controller_->Update();
	// シーン遷移
	//auto contData = controller_->GetInputData();
	//if (contData[NowFlame][static_cast<int>(InputID::Btn3)] &&
	//	!contData[OldFlame][static_cast<int>(InputID::Btn3)])

	if (!isLoaded_)
	{ return; 

	}

	player_->CheckSection();
	auto& ins = InputManager::GetInstance();
	if (ins.IsNew(KEY_INPUT_SPACE))
	{
		lpSceneMng.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	auto p= swi_->SetSwingPoint(player_->GetTransform()->pos,player_->CheckSection()); 

	auto gra= swi_->SetGravity(player_->GetTransform()->pos);
	player_->Update(SceneManager::GetInstance().GetDeltaTime(), SceneManager::GetInstance().GetCamera()->GetDir(), gra,p);

	stage_->Update();
	// コイン
	for (const auto c : coins_)
	{
		c->Update();
	}

	IsHitCoinPlayer();

	skyDome_->Update();

}

void GameScene::Draw(void)
{
	if (!isLoaded_) 
	{
		DrawFormatString(200, 200, 0xffffff, "Now Loading");
		return;
	}
	// コイン
	for (const auto c : coins_)
	{
		c->Draw();
	}

	//skyDome_->Draw();
	stage_->Draw();
	player_->Draw();
	swi_->Draw();
	grid_->Draw();
	// デバッグ表示
	//DrawDebug();

}

void GameScene::DrawDebug(void)
{
	DrawString(0, 0, "GameScene", 0xffffff);
}

void GameScene::Release(void)
{	
	delete player_;
	for (const auto c : coins_)
	{
		c->Release();
		delete c;
	}
	coins_.clear();

}

void GameScene::Load(void)
{



	//ステージ
	//stage_ = std::make_unique<Stage>();
	stage_->Init();

	//プレイヤー
	//player_ = new Player();
	player_->Init();
	//player_->AddCollider(stage_->GetTransform()->collider);

	// スカイドーム
	//skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();

	// グリッド線
	//grid_ = std::make_unique<Grid>();
	grid_->Init();
	//カメラの設定
	lpSceneMng.GetCamera()->ChangeMode(Camera::MODE::FOLLOW);

	lpSceneMng.GetCamera()->SetTransform(player_->GetTransform());

	isLoaded_ = true;

}
void GameScene::MakeCoin(void)
{
	Transform trans;
	Coin* coin;

	trans.pos = { 3000.0f, 200.0f, 2000.0f };
	trans.scl = { 1.0f,1.0f, 1.0f };
	trans.quaRot = Quaternion::Euler(
		AsoUtility::Deg2RadF(-0.0f),
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(0.0f)
	);
	coin = new Coin(trans);
	coin->Init();
	coins_.push_back(coin);

	trans.pos = { 2000.0f, 1200.0f, 200.0f };
	trans.scl = { 1.0f,1.0f, 1.0f };
	trans.quaRot = Quaternion::Euler(
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(0.0f)
	);
	coin = new Coin(trans);
	coin->Init();
	coins_.push_back(coin);
}

void GameScene::IsHitCoinPlayer(void)
{
	// コインとプレイヤーの衝突判定
	auto p = player_->GetTransform();

	auto coin = coins_;
	for (const auto c : coin)
	{
		//表示されているとき
		if (c->GetHit())
		{
			//衝突判定
			auto hit = MV1CollCheck_Sphere(p->modelId, -1, c->GetPos(), c->COL_RADIUS);

			//ひとつでも衝突していたら
			if (hit.HitNum > 0)
			{
				//衝突していたら
				if (hit.Dim->HitFlag > 0)
				{
					c->SetHit(false);
					isHitNum_++;
					break;
				}
			}
		}
	}
}