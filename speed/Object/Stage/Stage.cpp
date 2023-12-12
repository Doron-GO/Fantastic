#include <DxLib.h>
#include "Stage.h"
#include"../Manager/ImageMng.h"
#include "../../Player/Player.h"
Stage::Stage(std::vector<std::shared_ptr<Player>>& players)
	: players_(players)
{	
	loadMap_.LoadTSX("Src/Json/stageTSX_.json");
	loadMap_.LoadStage("Src/Json/stage_.json");
}

Stage::~Stage()
{
}

void Stage::Init()
{
	//tmxObj_.LoadTSX("./tmx/stage.tmx");
	itemBox_ = std::make_unique<ItemBox>(loadMap_,players_);
}

void Stage::Update()
{
	itemBox_->Update();
}

void Stage::Draw(Vector2DFloat cameraPos)
{
	auto &worldArea = loadMap_.GetWorldArea();
	const auto &tileSize = loadMap_.GetTileSize();
	auto& mapData = loadMap_.GetMapData();
	for (const auto& layer : loadMap_.GetMapData()) 
	{
		for (int y = 0; y < worldArea.y; y++)
		{
			for (int x = 0; x < worldArea.x; x++)
			{
				if (x + y * worldArea.x < layer.second.size())
				{
					auto gid = layer.second[x + y * worldArea.x];
					if (gid >= 0)
					{
						DrawGraph(x * tileSize.x+ cameraPos.x,
							(y * tileSize.y + cameraPos.y),
							lpImageMng.GetID(loadMap_.GetMapKey())
							[gid], true);
					}
				}
			}
		}
	}
	itemBox_->Draw(cameraPos);
	for ( auto& line : loadMap_.GetColList())
	{
		auto first = line.first;
		auto second = line.second;
		Line lines[4] = {
			//è„ÇÃï”   
			{first + cameraPos,
			(first + cameraPos) + Vector2DFloat{second.x,0} },
			//âEï”
			{(first + cameraPos) + Vector2DFloat{second.x,0} ,
			first + line.second + cameraPos },
			//â∫ÇÃï”
			{ first + second + cameraPos,
			first + Vector2DFloat{0,second.y } + cameraPos },
			//ç∂ï”
			{first + Vector2DFloat{0,second.y} + cameraPos ,
			first + cameraPos }
		};
		for (auto line : lines)
		{
			DrawLine(line.p.x, line.p.y, line.end.x, line.end.y, 0x00ff00);
		}
	}
}

const  Vector2D& Stage::GetWorldArea()
{
	return loadMap_.GetWorldArea();
}

const  Vector2D& Stage::GetTileSize()
{
	return loadMap_.GetTileSize();
}

const  std::string& Stage::GetMapKey()
{
	return loadMap_.GetMapKey();
}

const ColList& Stage::GetColList(void)
{
	return loadMap_.GetColList();
}

const ColList& Stage::GetWallColList(void)
{
	return loadMap_.WallGetColList();
}

const ColList& Stage::GetWireColList(void)
{
	return loadMap_.WireGetColList();
}
