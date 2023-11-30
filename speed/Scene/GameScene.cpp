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

	CheckPoint_ = { 1600.0f,1000.0f };
	camera_->Init(stage_->GetWorldArea() * stage_-> GetTileSize());//ƒJƒƒ‰‚ğ‰Šú‰»
	outSide_ = std::make_unique<OutSide>(*camera_, players_);
}

void GameScene::Update(Input& input)
{
	camera_->Update();
	for (const auto& player : players_)
	{
		if (player->IsAlive())
		{
			player->Update(input);
		}	
	}
	outSide_->Update();
	DecideOnTheBeginning(); 
	checkPoint_->Update();
	DrawOwnScreen();
}

void GameScene::Draw()
{
}

void GameScene::DecideOnTheBeginning()
{
	//ƒvƒŒƒCƒ„[‚Ìl”•ª‰ñ‚·
	for (int playerNum =0; playerNum <players_.size(); playerNum++)
	{	
		distance_.push_back(players_[playerNum]->GetPos().distance(checkPoint_->GetCheckPoint()));
	}
	for (int playerNum = 0; playerNum < players_.size()-1 ; playerNum++)
	{
		if (distance_[playerNum] < distance_[playerNum + 1])//
		{
			//Å‘O‚ğ’T‚·
			if (distance_[static_cast<int>(new_Num_)] > distance_[playerNum ])
			{
				new_Num_ = static_cast<PLAYER_NUM>(playerNum );
			}
			//ÅŒã”ö‚ğ’T‚³‚·
			if (distance_[static_cast<int>(last_Num_)] < distance_[playerNum+1])
			{
				last_Num_ = static_cast<PLAYER_NUM>(playerNum+1);
			}
		}
		else 
		{
			//Å‘O‚ğ’T‚·
			if (  distance_[playerNum]<distance_[static_cast<int>(new_Num_)])
			{
				new_Num_ = static_cast<PLAYER_NUM>(playerNum);
			}
			else if(distance_[playerNum+1] < distance_[static_cast<int>(new_Num_)])
			{
				new_Num_ = static_cast<PLAYER_NUM>(playerNum+1);
			}
			//ÅŒã‚ğ’T‚·
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
		if (player->IsAlive())
		{
			player->Draw(camera_->GetPos());
		}
	}

	outSide_->Draw(camera_->GetPos());
	checkPoint_->Draw(camera_->GetPos());
	//postEfffect_->Draw();

	DrawFormatStringF(0, 140, 0xffffff, "camera:%f,%f", camera_->GetPos().x, camera_->GetPos().y);
	DrawFormatStringF(0, 280, 0xffffff, "ÅŒã”ö‚Í:%d", static_cast<int>(last_Num_)+1);
	DrawFormatStringF(0, 260, 0xffffff, "æ“ª‚Í:%d", static_cast<int>(new_Num_)+1);


}
