#include<DxLib.h>
#include "GameScene.h"
#include"../Obj/ImageMng.h"
GameScene::GameScene(SceneMng& manager):Scene(manager)
{
	SetDrawScreen(DX_SCREEN_BACK);

	screenID_ = MakeScreen(1000, 800, true);
	
	camera_ = std::make_unique<Camera>();
	tmxObj_.LoadTMX("./tmx/stage.tmx");
	for (int i = 1; i <= 2; i++)
	{
		std::shared_ptr<Player> player;
		player = std::make_shared<Player>(i);
		player->Init(tmxObj_.GetColList(),i);
		players_.push_back(player);
	}

	camera_->ReConnect(players_[0]);
	camera_->Init(tmxObj_.GetWorldArea() * tmxObj_.GetTileSize());//ƒJƒƒ‰‚ð‰Šú‰»

}

void GameScene::Update(Input& input)
{
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

void GameScene::DrawOwnScreen()
{


	auto worldArea = tmxObj_.GetWorldArea();
	const auto tileSize = tmxObj_.GetTileSize();
	auto mapData = tmxObj_.GetMapData();

	DrawString(50, 50, "GameScene", 0xffffff);

	Vector2DFloat view = { 1200.0f, 800.0f };


	auto offset = (view / 3.0f) - camera_->GetPos();
	auto camera =  camera_->GetPos();


	for (const auto& layer : tmxObj_.GetMapData()) {


		for (int y = 0; y < worldArea.y; y++)
		{
			for (int x = 0; x < worldArea.x; x++)
			{
				if (x + y * worldArea.x < layer.second.size())
				{
					auto gid = layer.second[x + y * worldArea.x];

					if (gid >= 0)
					{
						auto a = tmxObj_.GetMapKey();

						DrawGraph(x  * tileSize.x+ offset.x,
							(y * tileSize.y + offset.y),
							lpImageMng.GetID(tmxObj_.GetMapKey())
							[gid], true);
						//DrawGraph(x  * tileSize.x,
						//	(y * tileSize.y ),
						//	lpImageMng.GetID(tmxObj_.GetMapKey())
						//	[gid], true);
					}
				}

			}
		}
	}
	for (const auto& player : players_)
	{
		player->Draw();
	}

	//player_->Draw();

	DrawFormatStringF(0, 80, 0xffffff, "camera:%f,%f", camera_->GetPos().x, camera_->GetPos().y);

}
