#include<DxLib.h>
#include "GameScene.h"
#include"../Object/ImageMng.h"
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
		player->Init(stage_->GetColList(), stage_->GetWallColList());
		players_.push_back(player);
	}
	camera_->ReConnect(players_[0]);
	new_Num_ = PLAYER_NUM::P_1;
	old_Num_ = PLAYER_NUM::P_1;
	camera_->Init(stage_->GetWorldArea() * stage_-> GetTileSize());//カメラを初期化
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

	//プレイヤーの人数分回す
	for (int playerNum =0; playerNum <players_.size()-1; playerNum++)
	{	
		//プレイヤーnの座標が次のプレイヤーn+1よりも先を走っていたら
		if (players_[playerNum]->GetPos().x < players_[playerNum + 1]->GetPos().x)
		{
			new_Num_ = static_cast<PLAYER_NUM>(playerNum + 1);
		}
		else
		{
			new_Num_ = static_cast<PLAYER_NUM>(playerNum);
		}
	}

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
	//auto worldArea = tmxObj_.GetWorldArea();
	//const auto tileSize = tmxObj_.GetTileSize();
	//auto mapData = tmxObj_.GetMapData();

	DrawString(50, 50, "GameScene", 0xffffff);

	stage_->Draw(camera_->GetPos());

	for (const auto& player : players_)
	{
		player->Draw(camera_->GetPos());
	}
	DrawFormatStringF(0, 80, 0xffffff, "camera:%f,%f", camera_->GetPos().x, camera_->GetPos().y);
}
