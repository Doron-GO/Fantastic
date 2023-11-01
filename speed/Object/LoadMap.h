#pragma once
#include<string>
#include<map>
#include<vector>
#include"../Common/Collision.h"
#include"../Vector2D.h"
#include"../Src/Json/json.hpp"

using MapData = std::map <std::string, std::vector <int>>;
using json = nlohmann::json;


class LoadMap
{
public:

	bool LoadTSX(std::string fileName);
	bool LoadStage(std::string fileName);

	bool SetMap();

	const GrndColList& GetColList(void);
	const WallColList& WallGetColList(void);
	const Vector2D& GetWorldArea(void);
	const Vector2D& GetTileSize(void);
	const  MapData& GetMapData(void);
	const std::string& GetMapKey(void);

private:
	Vector2D layerSize_;
	Vector2D WorldArea_;	//	世界の大きさ
	Vector2D tileSize_;
	GrndColList colList_;
	WallColList WallCollList_;

	std::string mapKey_;
	MapData mapData;//マップにストリングはあまり使わない方がいいenumの方がいい

	json json_;

};

