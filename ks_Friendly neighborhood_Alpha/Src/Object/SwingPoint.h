#pragma once
#include<DxLib.h>
#include<map>
#include<string>
#include"../Json/json.hpp"
#include"Common/Transform.h"
#include"Stage.h"


class SwingPoint
{
public:
	SwingPoint(Stage::STAGE_NUM num,Transform transform);
	~SwingPoint();

	void Load(void);
	void Draw(void);

private:

	//std::map < std::string, std::list < std::pair <VECTOR,VECTOR>>> swingPoint_;
	Transform transform_;
	Stage::STAGE_NUM num_;

	json json_;

};

