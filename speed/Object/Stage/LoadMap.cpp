#include "LoadMap.h"
#include<DxLib.h>
#include<fstream>
#include <stdlib.h>
#include"../Manager/ImageMng.h"

bool LoadMap::LoadTSX(std::string fileName)
{
	//json�t�@�C�����J��

	std::ifstream f(fileName.c_str());
	json_ = json::parse(f);

	std::string source;

	//TSX.json�ɏ����Ă���v�f��ǂݍ���
	auto width = json_["width"].get<int>();
	auto height = json_["height"].get<int>();
	auto tilewidth = json_["tilewidth"].get<int>();
	auto tilecount = json_["tilecount"].get<int>();
	auto tileheight = json_["tileheight"].get<int>();
	auto columns = json_["columns"].get<int>();
	source = json_["source"];

	mapKey_ = "mapChip";
	//GetID�ŉ�m��ǂݍ���(������LoadDiveGraph)
	lpImageMng.GetID("./Src/Img/" + source, mapKey_, { tilewidth, tileheight }, { columns ,tilecount / columns });
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

	auto& layer = json_["layers"][0];
	layerSize_.x = layer["width"].get<int>();
	layerSize_.y = layer["height"].get<int>();

	int firstgid = layer["id"].get<int>();

	std::string layerName = layer["name"];
	auto layerData = mapData.try_emplace(layerName);
	if (layerData.second)
	{
		layerData.first->second.resize(WorldArea_.x * WorldArea_.y);
	}

	//0�Ƃ�1�Ƃ��̃}�b�v�������i�[���Ă�
	auto& data = layer["data"];
	int cnt = 0;
	for (auto& vecData : layerData.first->second)
	{
		vecData = data[cnt].get<int>()-firstgid;
		cnt++;
	}

	//�X�e�[�W�̓����蔻���ǂݍ��ރ����_��
	auto LoadCol = [](json json, std::list<Collision> &col)
	{
		for (int cnt = 0; cnt <= json.size() - 1; cnt++)
		{
			float x = json[cnt]["x"].get<int>();
			float y = json[cnt]["y"].get<int>();
			float w = json[cnt]["width"].get<int>();
			float h = json[cnt]["height"].get<int>();
			col.push_back(Collision{ Vector2DFloat{x,y},Vector2DFloat{w,h} });
		}
	};
	auto & col = json_["layers"][2]["objects"];
	auto& wallCol = json_["layers"][3]["objects"];
	auto& wireCol = json_["layers"][4]["objects"];

	LoadCol(col, colList_);
	LoadCol(wallCol, WallCollList_);
	LoadCol(wireCol, WireCollList_);

	return true;
}

const ColList& LoadMap::GetColList(void)
{
	return colList_;
}

const ColList& LoadMap::WallGetColList(void)
{
	return WallCollList_;
}

const ColList& LoadMap::WireGetColList(void)
{
	return WireCollList_;
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
