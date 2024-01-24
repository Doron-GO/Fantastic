#include<DxLib.h>
#include "GameScene.h"
#include "SceneMng.h"
#include"../Object/Manager/ImageMng.h"
#include"Transition/TileTransitor.h"
#include"../Object/Time/TimeCount.h"
#include"../Object/Time/DeltaTime.h"


GameScene::GameScene(SceneMng& manager, int n, Transitor& transit):Scene(manager, n, transit), playerNum_(n),_update(&GameScene::MultiPlayUpdate)
{
	SetDrawScreen(DX_SCREEN_BACK);

	sceneTransitor_.Start();
	camera_ = std::make_unique<Camera>();	
	stage_ = std::make_unique<Stage>();
	outSide_ = std::make_unique<OutSide>(*camera_, playerNum_);
	timeCount_ = std::make_unique<TimeCount>();

	playerManager_ = std::make_unique<PlayerManager>(outSide_->conclusion_);
	playerManager_->Init(playerNum_, stage_->GetColList(), stage_->GetWallColList(), stage_->GetWireColList());
	if (playerNum_ ==1)
	{
		playerManager_->SinglePlay();
		_update = &GameScene::SinglePlayUpdate;
	}
	//camera_->Init(stage_->GetWorldArea() * stage_-> GetTileSize());//カメラを初期化
	stage_->Init(playerManager_->GetPlayers());
	checkPoint_ = std::make_unique<CheckPoint>(playerManager_->GetPlayers(), stage_->CheckPointGetColList());
	deltaTime.SetStart();
	camera_->ReConnect(playerManager_->GetPlayers()[(int)playerManager_->GetNewLeadNum()]);
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
	//DrawFormatStringF(0, 580, 0xffffff, "最後尾は:%d", static_cast<int>(Last));
	//DrawFormatStringF(0, 560, 0xffffff, "先頭は:%d", static_cast<int>(newLeder));
	timeCount_->Draw();
	sceneTransitor_.Draw();
}

void GameScene::DecideOnTheBeginning()
{
	//playerManager_->DecideOnTheBeginning(checkPoint_->GetCheckPoint());
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
	if (elapsedTime >= 2.0f)
	{
		checkPoint_->Update();
		timeCount_->Update(elapsedTime);
		outSide_->Update(playerManager_->GetPlayers());
		playerManager_->Update(input);
	}
	DecideOnTheBeginning();

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
	if (elapsedTime >= 2.0f)
	{
		checkPoint_->Update();
		playerManager_->Update(input);
		timeCount_->Update(elapsedTime);
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

