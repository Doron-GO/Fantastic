#include<DxLib.h>
#include "GameScene.h"
#include"../Object/Manager/ImageMng.h"


GameScene::GameScene(SceneMng& manager):Scene(manager)
{
	SetDrawScreen(DX_SCREEN_BACK);
	screenID_ = MakeScreen(1600.0f, 1000.0f, true);
	stage_ = std::make_unique<Stage>();

	stage_->Init();
	camera_ = std::make_unique<Camera>();
	//tmxObj_.LoadTMX("./tmx/stage.tmx");
	//playerNum = GetJoypadNum();

	for (int playerNum = 1; playerNum <= GetJoypadNum(); playerNum++)
	{
		std::shared_ptr<Player> player;
		player = std::make_shared<Player>(playerNum);
		player->Init(stage_->GetColList(), stage_->GetWallColList(), stage_->GetWireColList());
		players_.push_back(player);
	}
	checkPoint_ = std::make_unique<CheckPoint>(players_);
	camera_->ReConnect(players_[0]);
	new_Num_ = PLAYER_NUM::P_1;
	old_Num_ = PLAYER_NUM::P_1;
	last_Num_ = PLAYER_NUM::P_1;
	//FirstPos_ = players_[0]->GetPos();

	CheckPoint_ = { 1600.0f,1000.0f };
	camera_->Init(stage_->GetWorldArea() * stage_-> GetTileSize());//カメラを初期化
	outSide_ = std::make_unique<OutSide>(*camera_, players_);
}

void GameScene::Update(Input& input)
{
	camera_->Update();
	for (const auto& player : players_)
	{
		player->Update(input);
	}
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
	//プレイヤーの人数分回す
	for (int playerNum =0; playerNum <players_.size(); playerNum++)
	{	
		distance_.push_back(players_[playerNum]->GetPos().distance(checkPoint_->GetCheckPoint()));
	}
	for (int playerNum = 0; playerNum < players_.size()-1 ; playerNum++)
	{
		if (distance_[playerNum] <= distance_[playerNum + 1])//
		{
			//最前を探す
			if (distance_[static_cast<int>(new_Num_)] >= distance_[playerNum ])
			{
				new_Num_ = static_cast<PLAYER_NUM>(playerNum );
			}
			//最後尾を探さす
			if (distance_[static_cast<int>(last_Num_)] <= distance_[playerNum+1])
			{
				last_Num_ = static_cast<PLAYER_NUM>(playerNum+1);
			}
		}
		else 
		{
			//最前を探す
			if (  distance_[playerNum]<=distance_[static_cast<int>(new_Num_)])
			{
				new_Num_ = static_cast<PLAYER_NUM>(playerNum+1);
			}
			//最後を探す
			if (  distance_[playerNum]>=distance_[static_cast<int>(last_Num_)])
			{
				last_Num_ = static_cast<PLAYER_NUM>(playerNum);
			}
		}
	}

	distance_.clear();
	if (old_Num_!=new_Num_)
	{
		camera_->ReConnect(players_[static_cast<int>(new_Num_)]);
		camera_->PhaseChanging(static_cast<int>(new_Num_));
	}
	//camera_->ReConnect(players_[static_cast<int>(new_Num_)]);
	old_Num_ = new_Num_;
}

void GameScene::DrawOwnScreen()
{
	stage_->Draw(camera_->GetPos());
	for (const auto& player : players_)
	{
		player->Draw(camera_->GetPos());
	}

	outSide_->Draw();
	checkPoint_->Draw(camera_->GetPos());
	DrawFormatStringF(0, 140, 0xffffff, "camera:%f,%f", camera_->GetPos().x, camera_->GetPos().y);
	DrawFormatStringF(0, 280, 0xffffff, "最後尾は:%d", static_cast<int>(last_Num_)+1);
	DrawFormatStringF(0, 260, 0xffffff, "先頭は:%d", static_cast<int>(new_Num_)+1);
}
