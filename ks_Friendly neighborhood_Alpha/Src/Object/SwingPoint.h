#pragma once
#include<DxLib.h>
#include<map>
#include<string>
#include"../Json/json.hpp"
#include"Common/Transform.h"
#include"Stage.h"
using json = nlohmann::json;


class SwingPoint
{
public:
	SwingPoint(Stage::STAGE_NUM num,Transform transform);
	~SwingPoint();

	void Load(void);

	enum class SIDE
	{
		FRONT,
		BACK,
		RIGHT,
		LEFT
	};

private:
	Transform transform_;
	Stage::STAGE_NUM num_;

	json json_;

	//std::map < Stage::STAGE_NUM, std::vector < std::pair <VECTOR, VECTOR>>> swingPoint_;
	std::map<SIDE, std::vector<VECTOR>> swingPoint_;
	std::map<, std::map<int, std::vector<VECTOR>>> swingList_;
	//std::map < std::string, std::list < std::pair <VECTOR,VECTOR>>> swingPoint_;
	VECTOR norm_;
	VECTOR point_;
};

