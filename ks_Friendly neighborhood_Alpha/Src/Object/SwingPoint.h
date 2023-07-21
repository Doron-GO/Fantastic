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

	std::vector<VECTOR> pos_;//支点の座標
	std::map<SIDE, std::vector<VECTOR>> swingPoint_; //支点情報がたくさん入っている

	std::map<int , std::map<SIDE, std::vector<VECTOR>>>BuildingList_;//20個
	
	std::map<Stage::STAGE_NUM, std::map<int, std::map<SIDE, std::vector<VECTOR>>>> sectionList_;
	
	VECTOR norm_;

	int total;
	float min;
	int minNum;
	std::map<int, VECTOR> testPoint_;//支点番号を渡すと支点の座標が戻ってくる
	std::map<int, std::pair<VECTOR,VECTOR>> testPoint2_;//支点番号を渡すとpairが戻ってくる　重力方向・座標


	std::vector<float> distance_;//プレイヤーから支点までの距離を格納する
	float distans_2;
	std::pair<float, VECTOR>fainal_;//firstはdistance,secondは支点座標
	std::vector<std::pair<float, VECTOR>> comparison_;//fainal_が入っている


};

