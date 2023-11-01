#include "LoadMap.h"
#include<DxLib.h>
#include<fstream>
#include <stdlib.h>
#include"ImageMng.h"

bool LoadMap::LoadTSX(std::string fileName)
{
	//jsonファイルを開く

	std::ifstream f(fileName.c_str());
	json_ = json::parse(f);

	std::string source;

	auto width = json_["width"].get<int>();
	auto height = json_["height"].get<int>();
	auto tilewidth = json_["width"].get<int>();
	auto tilecount = json_["tilewidth"].get<int>();
	auto tileheight = json_["tileheight"].get<int>();
	auto columns = json_["columns"].get<int>();
	source = json_["source"];

	mapKey_ = "mapChip";
	lpImageMng.GetID("./tmx" + source.substr(1), mapKey_, { tilewidth, tileheight }, { columns ,tilecount / columns });
	return true;

}

bool LoadMap::LoadStage(std::string fileName)
{
	std::ifstream f(fileName.c_str());
	json_ = json::parse(f);

	WorldArea_.x = json_["width"].get<int>();
	WorldArea_.y = json_["height"].get<int>();
	tileSize_.x = json_["tilewidth"].get<int>();
	tileSize_.y = json_["tileheight"].get<int>();

	SetMap();
	return true;
}

bool LoadMap::SetMap()
{

	auto layer = json_["layers"][0];

	layerSize_.x = layer["width"].get<int>();
	layerSize_.y = layer["height"].get<int>();







	auto col = json_["layers"][2]["objects"];
	for (int cnt = 0; cnt < col.size()-1 ; cnt++)
	{
		float x = col[cnt]["x"].get<int>();
		float y = col[cnt]["y"].get<int>();
		float w = col[cnt]["width"].get<int>();
		float h = col[cnt]["height"].get<int>();
		colList_.push_back(Collision{ Vector2DFloat{x,y},Vector2DFloat{w,h} });
	}
	return true;
}

const ColList& LoadMap::GetColList(void)
{
	return colList_;
}

const Vector2D& LoadMap::GetWorldArea(void)
{
	return  WorldArea_;
}

const Vector2D& LoadMap::GetTileSize(void)
{
	return  tileSize_;
}

const MapData& LoadMap::GetMapData(void)
{
	return  mapData;
}

const std::string& LoadMap::GetMapKey(void)
{
	return  mapKey_;
}
