#include<DxLib.h>
#include "GameScene.h"
#include"../Obj/ImageMng.h"
GameScene::GameScene(SceneMng& manager):Scene(manager)
{
	player_ = std::make_unique<Player>();
	tmxObj_.LoadTMX("./tmx/stage.tmx");
	player_->Init(tmxObj_.GetColList());
}

void GameScene::Update(Input& input)
{
	player_->Update(input);
}

void GameScene::Draw()
{
	auto worldArea = tmxObj_.GetWorldArea();
	const auto tileSize = tmxObj_.GetTileSize();
	auto mapData = tmxObj_.GetMapData();

	DrawString(50, 50, "GameScene", 0xffffff);

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

						DrawGraph(x * tileSize.x,
							y * tileSize.y,
							lpImageMng.GetID(tmxObj_.GetMapKey())
							[gid], true);
					}
				}

			}
		}
	}

	player_->Draw();



}
