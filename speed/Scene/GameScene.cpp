#include<DxLib.h>
#include "GameScene.h"
#include "SceneMng.h"
#include"../Object/Manager/ImageMng.h"
#include"Transition/TileTransitor.h"
#include"../Object/Time/TimeCount.h"
#include"../Object/Time/DeltaTime.h"


GameScene::GameScene(SceneMng& manager, int n, Transitor& transit):Scene(manager, n, transit), playerNum_(n),_update(&GameScene::MultiPlayUpdate)
{

	sceneTransitor_.Start();
	camera_ = std::make_unique<Camera>();	
	stage_ = std::make_unique<Stage>();
	outSide_ = std::make_unique<OutSide>(*camera_, playerNum_);

	playerManager_ = std::make_unique<PlayerManager>(outSide_->conclusion_);
	playerManager_->Init(playerNum_, stage_->GetColList(), stage_->GetWallColList(), stage_->GetWireColList());
	//camera_->Init(stage_->GetWorldArea() * stage_-> GetTileSize());//カメラを初期化
	stage_->Init(playerManager_->GetPlayers());
	checkPoint_ = std::make_unique<CheckPoint>(playerManager_->GetPlayers(), stage_->CheckPointGetColList());
	timeCount_ = std::make_unique<TimeCount>(*checkPoint_);
	camera_->ReConnect(playerManager_->GetPlayers()[(int)playerManager_->GetNewLeadNum()]);
	if (playerNum_ ==1)
	{
		playerManager_->SinglePlay();
		checkPoint_->SetSingleMode();
		_update = &GameScene::SinglePlayUpdate;
		timeCount_->SinglePlay();
	}
	GoImg_ = LoadGraph("Src/Img/Go.png");
	ReadyImg_ = LoadGraph("Src/Img/Ready.png");

	deltaTime.Reset();
	startTime_ = deltaTime.GetElapsedTime();

}

GameScene::~GameScene()
{
}

void GameScene::Update(Input& input)
{
	deltaTime.update();
	auto elapsed = deltaTime.GetElapsedTime();
	(this->*_update)(input, elapsed);


}

void GameScene::Draw()
{

	ClearDrawScreen();
	stage_->Draw(camera_->GetPos());
	outSide_->Draw(camera_->GetPos());
	playerManager_->Draw(camera_->GetPos());
	checkPoint_->Draw(camera_->GetPos());
	//DrawFormatStringF(0, 140, 0xffffff, "camera:%f,%f", camera_->GetPos().x, camera_->GetPos().y);
	auto newLeder = playerManager_->GetNewLeadNum();
	auto Last = playerManager_->GetLastLeadNum();
	timeCount_->Draw();
	auto elapsed = deltaTime.GetElapsedTime();

	if (!(startTime_ +0.6f> elapsed)&& elapsed <= startTime_ + 1.5f)
	{
		DrawRotaGraph2F(screenSize_.x / 2.0f,0.0f+ (screenSize_.y / 3.0f) - elapsed * 40,
			288.0f,33.0f, 2.0f, 0.0f, ReadyImg_, true);
	}
	if (!(startTime_ + 1.5f > elapsed) && elapsed <= startTime_ + 2.0f)
	{
		DrawRotaGraph2F(screenSize_.x / 2.0f, 0.0f+(screenSize_.y / 3.0f) ,
			225.0f, 80.0f, 1.0f, 0.0f, GoImg_, true);
	}


	sceneTransitor_.Draw();
}

void GameScene::DecideOnTheBeginning()
{
	playerManager_->DecideOnTheBeginning2(checkPoint_->GetCheckPoint2());
	auto newLeder= playerManager_->GetNewLeadNum();
	auto oldLeder =playerManager_->GetOldLeadNum();
	//前のフレームの先頭プレイヤーと今の先頭プレイヤーが違っていたら、
	//カメラ追従対象を変更する。
	if (oldLeder != newLeder)
	{
		camera_->ReConnect(playerManager_->GetPlayers()[(int)newLeder]);
		camera_->PhaseChanging((int)newLeder);
		outSide_->PhaseChanging();
	}
	playerManager_->SetOld();
}

void GameScene::MultiPlayUpdate(Input& input, float elapsedTime)
{
	camera_->Update();

	if (elapsedTime >= startTime_+2.0f)
	{		

		timeCount_->Update(elapsedTime);
		checkPoint_->Update();
		outSide_->Update(playerManager_->GetPlayers());
		playerManager_->Update(input);
		DecideOnTheBeginning();
	}
	if (outSide_->conclusion_)
	{
		if (input.IsTriggerd("jump"))
		{
			sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_, playerNum_, sceneTransitor_));
			return;
		}
	}
	sceneTransitor_.Update();

}

void GameScene::SinglePlayUpdate(Input& input, float elapsedTime)
{
	camera_->Update();
	if (elapsedTime >= startTime_+ 2.0f)
	{		
		timeCount_->Update(elapsedTime);
		if (checkPoint_->IsGoal())
		{
			playerManager_->Goal();
		}
		checkPoint_->Update();
		playerManager_->Update(input);
		DecideOnTheBeginning();	

	}
	if (timeCount_->IsEnd())
	{
		if (input.IsTriggerd("jump"))
		{
			sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_, playerNum_, sceneTransitor_));
			return;
		}
	}
	sceneTransitor_.Update();

}

