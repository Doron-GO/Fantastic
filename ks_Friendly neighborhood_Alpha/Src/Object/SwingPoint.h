#pragma once
#include<map>
#include<string>
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


private:
	Transform transform_;
	Stage::STAGE_NUM num_;

	json json_;

	//std::map < Stage::STAGE_NUM, std::vector < std::pair <VECTOR, VECTOR>>> swingPoint_;
	std::vector<VECTOR> pos_;
	std::map<SIDE, std::vector<VECTOR>> swingPoint_; // 40(îzóÒ)   360

	//std::map<int ,swingPoint_>BuildingList_;Å@20å¬
	
	//std::map<Stage::STAGE_NUM, std::map<int, BuildingList_>> sectionList_;
	
	
	
	//std::map < std::string, std::list < std::pair <VECTOR,VECTOR>>> swingPoint_;
	VECTOR norm_;
	VECTOR point_;
};

