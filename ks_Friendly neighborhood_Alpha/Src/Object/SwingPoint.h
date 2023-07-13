#pragma once
#include<map>
#include"../Json/json.hpp"
#include"Common/Transform.h"
#include"Stage.h"
using json = nlohmann::json;


class SwingPoint
{
public:

	enum class SIDE
	{
		FRONT,
		BACK,
		RIGHT,
		LEFT
	};

	SwingPoint();
	~SwingPoint();

	void Load(void);

	const VECTOR SetSwingPoint(VECTOR PlayerPos,int section);
private:
	json json_;

	//std::map < Stage::STAGE_NUM, std::vector < std::pair <VECTOR, VECTOR>>> swingPoint_;
	std::vector<VECTOR> pos_;
	std::map<SIDE, std::vector<VECTOR>> swingPoint_; // 40(”z—ñ)   360

	std::map<int , std::map<SIDE, std::vector<VECTOR>>>BuildingList_;//20ŒÂ
	
	std::map<Stage::STAGE_NUM, std::map<int, std::map<SIDE, std::vector<VECTOR>>>> sectionList_;
	
	
	
	//std::map < std::string, std::list < std::pair <VECTOR,VECTOR>>> swingPoint_;
	VECTOR norm_;
};

