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

	std::vector<VECTOR> pos_;//�x�_�̍��W
	std::map<SIDE, std::vector<VECTOR>> swingPoint_; //�x�_��񂪂�����������Ă���

	std::map<int , std::map<SIDE, std::vector<VECTOR>>>BuildingList_;//20��
	
	std::map<Stage::STAGE_NUM, std::map<int, std::map<SIDE, std::vector<VECTOR>>>> sectionList_;
	
	VECTOR norm_;

	int total;
	float min;
	int minNum;
	std::map<int, VECTOR> testPoint_;//�x�_�ԍ���n���Ǝx�_�̍��W���߂��Ă���
	std::map<int, std::pair<VECTOR,VECTOR>> testPoint2_;//�x�_�ԍ���n����pair���߂��Ă���@�d�͕����E���W


	std::vector<float> distance_;//�v���C���[����x�_�܂ł̋������i�[����
	float distans_2;
	std::pair<float, VECTOR>fainal_;//first��distance,second�͎x�_���W
	std::vector<std::pair<float, VECTOR>> comparison_;//fainal_�������Ă���


};

