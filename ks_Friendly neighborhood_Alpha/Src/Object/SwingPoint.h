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

	void Draw(void);
	void Load(void);

	const VECTOR SetSwingPoint(VECTOR PlayerPos,int section);
	const VECTOR SetGravity(VECTOR PlayerPos);
private:
	float Magnitude(VECTOR pos)const;


	json json_;

	//std::map < Stage::STAGE_NUM, std::vector < std::pair <VECTOR, VECTOR>>> swingPoint_;
	std::vector<VECTOR> pos_;
	std::map<SIDE, std::vector<VECTOR>> swingPoint_; // 40(�z��)   360

	std::map<int , std::map<SIDE, std::vector<VECTOR>>>BuildingList_;//20��
	
	std::map<Stage::STAGE_NUM, std::map<int, std::map<SIDE, std::vector<VECTOR>>>> sectionList_;
	
	//std::map < std::string, std::list < std::pair <VECTOR,VECTOR>>> swingPoint_;
	VECTOR norm_;

	int total;
	float min;
	int minNum;
	std::map<int, VECTOR> testPoint_;
	std::vector<float> distance_;

};

