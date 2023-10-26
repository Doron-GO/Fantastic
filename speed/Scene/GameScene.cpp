#include<DxLib.h>
#include "GameScene.h"
#include"../Obj/ImageMng.h"
GameScene::GameScene(SceneMng& manager):Scene(manager)
{
	SetDrawScreen(DX_SCREEN_BACK);
	screenID_ = MakeScreen(1000, 800, true);
	stage_ = std::make_unique<Stage>();
	stage_->Init();
	camera_ = std::make_unique<Camera>();
	//tmxObj_.LoadTMX("./tmx/stage.tmx");
	//playerNum = GetJoypadNum();

	for (int playerNum = 1; playerNum <= GetJoypadNum(); playerNum++)
	{
		std::shared_ptr<Player> player;
		player = std::make_shared<Player>(playerNum);
		player->Init(stage_->GetColList(), playerNum);
		players_.push_back(player);
	}

	camera_->ReConnect(players_[0]);
	camera_->Init(stage_->GetWorldArea() * stage_-> GetTileSize());//ƒJƒƒ‰‚ð‰Šú‰»

}

void GameScene::Update(Input& input)
{
	DecideOnTheBeginning();
	camera_->Update();
	
	for (const auto& player : players_)
	{
		player->Update(input);
	}
	DrawOwnScreen();
}

void GameScene::Draw()
{
	

}

void GameScene::DecideOnTheBeginning()
{
	int beginning = 0;
	for (int playerNum =0; playerNum <players_.size()-1; playerNum++)
	{
		if (players_[playerNum]->GetPos().x <= players_[playerNum + 1]->GetPos().x)
		{
			beginning = playerNum+1;
		}
		else
		{
			beginning = playerNum;
		}
	}
	camera_->ReConnect(players_[beginning]);
}

void GameScene::DrawOwnScreen()
{

	//auto worldArea = tmxObj_.GetWorldArea();
	//const auto tileSize = tmxObj_.GetTileSize();
	//auto mapData = tmxObj_.GetMapData();

	DrawString(50, 50, "GameScene", 0xffffff);

	Vector2DFloat view = { 1200.0f, 800.0f };

	auto offset = (view / 3.0f) - camera_->GetPos();
	auto camera =  camera_->GetPos();

	stage_->Draw(camera_->GetPos());

	for (const auto& player : players_)
	{
		player->Draw(camera_->GetPos());
	}

	DrawFormatStringF(0, 80, 0xffffff, "camera:%f,%f", camera_->GetPos().x, camera_->GetPos().y);

}
