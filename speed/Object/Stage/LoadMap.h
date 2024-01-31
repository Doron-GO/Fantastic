#pragma once
#include<string>
#include<map>
#include<vector>
#include"../../Common/Collision.h"
#include"../../Vector2D.h"
#include"../../Src/Json/json.hpp"

using MapData = std::map <std::string, std::vector <int>>;
using json = nlohmann::json;


class LoadMap
{
public:

	bool LoadTSX(std::string fileName);
	bool LoadStage(std::string fileName);

	bool SetMap();

	const ColList& GetColList(void);
	const ColList& WallGetColList(void);
	const ColList& WireGetColList(void);
	const ColList& itemBoxGetColList(void);
	const ColList& BlockGetColList(void);
	const PointColList& CheckPointGetColList(void);
	const Vector2D& GetWorldArea(void);
	const Vector2D& GetTileSize(void);
	const  MapData& GetMapData(void);
	const std::string& GetMapKey(void);
	
private:
	Vector2D layerSize_;
	Vector2D WorldArea_;	//	世界の大きさ
	Vector2D tileSize_;
	ColList colList_;
	ColList WallCollList_;
	ColList WireCollList_;
	ColList itemBoxCollList_;
	PointColList checkPointCollList_;
	ColList blockCollList_;

	std::string mapKey_;
	MapData mapData;//マップにストリングはあまり使わない方がいいenumの方がいい

	json json_;
};

