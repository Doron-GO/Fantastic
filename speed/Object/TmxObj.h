#pragma once
#include<DxLib.h>
#include<string>
#include"../Vector2D.h"
#include<map>
#include"../Common/Collision.h"
#include"../rapidxml/rapidxml.hpp"
#include"../rapidxml/rapidxml_utils.hpp"

using MapData = std::map <std::string, std::vector <int>>;

class TmxObj
{
public:
	bool LoadTSX(std::string fileName);
	bool LoadTMX(std::string fileName);
	bool SetMap(rapidxml::xml_node<>* node);
	const Vector2D& GetWorldArea(void);
	const Vector2D& GetTileSize(void);
	const  MapData& GetMapData(void);
	const std::string& GetMapKey(void);
	const ColList& GetColList(void);


private:

	rapidxml::xml_document<> tsxDoc_;

	unsigned int layerSize_;
	Vector2D WorldArea_;	//	世界の大きさ
	Vector2D tileSize_;
	ColList colList_;

	std::string mapKey_;
	MapData mapData;//マップにストリングはあまり使わない方がいいenumの方がいい
	
//bool operator !=(const float& vec) const
//{
//	return(x != vec.x && y != vec.y);
//}
//
};

