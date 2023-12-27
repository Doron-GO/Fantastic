#include<DxLib.h>
#include "GameScene.h"
#include"../Object/Manager/ImageMng.h"

GameScene::GameScene(SceneMng& manager):Scene(manager)
{
	SetDrawScreen(DX_SCREEN_BACK);
	screenID_ = MakeScreen(1600.0f, 1000.0f, true);
	camera_ = std::make_unique<Camera>();	
	stage_ = std::make_unique<Stage>();

	playerManager_ = std::make_unique<PlayerManager>();
	playerManager_->Init(GetJoypadNum(), stage_->GetColList(), stage_->GetWallColList(),
		stage_->GetWireColList());
	stage_->Init(playerManager_->GetPlayers());
	checkPoint_ = std::make_unique<CheckPoint>(playerManager_->GetPlayers());
	camera_->ReConnect(playerManager_->GetPlayers()[(int)playerManager_->GetNewLeadNum()]);
	camera_->Init(stage_->GetWorldArea() * stage_-> GetTileSize());//カメラを初期化
	outSide_ = std::make_unique<OutSide>(*camera_, playerManager_->GetPlayers(), GetJoypadNum());

}

void GameScene::Update(Input& input)
{
	camera_->Update();
	playerManager_->Update(input);
	outSide_->Update();
	checkPoint_->Update();
	DecideOnTheBeginning();
	DrawOwnScreen();
}

void GameScene::Draw()
{
}

void GameScene::DecideOnTheBeginning()
{
	playerManager_->DecideOnTheBeginning(checkPoint_->GetCheckPoint());
	auto newLeder= playerManager_->GetNewLeadNum();
	auto oldLeder =playerManager_->GetOldLeadNum();
	//前のフレームの先頭プレイヤーと今の先頭プレイヤーが違っていたら、
	//カメラ追従対象を変更する。
	if (oldLeder != newLeder)
	{
		camera_->ReConnect(playerManager_->GetPlayers()[(int)newLeder]);
		camera_->PhaseChanging((int)newLeder);
	}
	playerManager_->SetOld();
}

void GameScene::DrawOwnScreen()
{
	stage_->Draw(camera_->GetPos());
	playerManager_->Draw(camera_->GetPos());
	outSide_->Draw(camera_->GetPos());
	checkPoint_->Draw(camera_->GetPos());
	DrawFormatStringF(0, 140, 0xffffff, "camera:%f,%f", camera_->GetPos().x, camera_->GetPos().y);
	auto newLeder = playerManager_->GetNewLeadNum();
	auto Last = playerManager_->GetLastLeadNum();

	DrawFormatStringF(0, 580, 0xffffff, "最後尾は:%d", static_cast<int>(Last));
	DrawFormatStringF(0, 560, 0xffffff, "先頭は:%d", static_cast<int>(newLeder));
}
