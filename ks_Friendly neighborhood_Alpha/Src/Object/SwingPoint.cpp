#include "SwingPoint.h"
#include<fstream>
#include<DxLib.h>
#include <stdlib.h>
#include<string>



SwingPoint::SwingPoint()
{
	norm_ = VECTOR{ 0.0f,0.0f,0.0f };
	min = 9999999.0f;

}

SwingPoint::~SwingPoint()
{
} 

void SwingPoint::Draw(void)
{
	//DrawFormatString(50, 400, 0xffffff, " min %f", min);
	//DrawFormatString(50, 440, 0xffffff, "testpoint %f,%f,%f", testPoint_[minNum].x, testPoint_[minNum].y, testPoint_[minNum].z);
}

void SwingPoint::Load(void)
{
	std::ifstream f("Src/Json/Data.json");
	if (!f)
	{
		auto l = 0.0f;
	}
 	json_ = json::parse(f);

	int TotalSectionNum = json_["TotalSectionNum"].get<int>();

	for (int TSN = 1; TSN <= TotalSectionNum; TSN++)
	{
		std::string tsn = std::to_string(TSN);
		std::string Buildings = "Buildings" + tsn;
		auto Buildings_ = json_[Buildings];
		int totalBldgNum = Buildings_["TotalBldgNum"].get<int>();
		BuildingList_.clear();
		for (int idx = 1; idx <= totalBldgNum; idx++)
		{
			std::string st = std::to_string(idx);
			std::string BldgNum = "Bldg" + st;
			auto Bldg = Buildings_[BldgNum];
			//int VecNum = Bldg["VECTORTotalNum"].get<int>();
			int PointTotalNum = Bldg["PointTotalNum"];
			std::string PointTotalNumSt = std::to_string(PointTotalNum);

			auto SideNum = Bldg["SideNum"].get<int>();

			for (int idx2 = 1; idx2 <= SideNum; idx2++)
			{
				std::string num = std::to_string(idx2);
				std::string Side = "Side" + num;
				auto sideObj = Bldg[Side];
				int VecNum = sideObj["VECTORTotalNum"].get<int>();

				auto norm = sideObj["Norm"];

				norm_ = { norm["x"].get<float>(),norm["y"].get<float>() ,norm["z"].get<float>() };

				std::vector<VECTOR> p;

				for (int Ptl = 1; Ptl <= VecNum; Ptl++)
				{
					std::string num2 = std::to_string(Ptl);

					auto pos = Bldg[Side][num2]["VECTOR"];
					p.push_back(
						VECTOR{ pos["x"].get<float>(),
						pos["y"].get<float>(),
						pos["z"].get<float>() });
					//pos_ = p;
					if (Ptl == VecNum)
					{
						swingPoint_[static_cast<SIDE>(idx2 - 1)] = p;
					}
				}
			}
			BuildingList_[idx] = swingPoint_;
		}
			sectionList_[static_cast<Stage::STAGE_NUM>(TSN-1)]= BuildingList_;
	}

	auto BillPoint = json_["BillPoint"];
	 total = BillPoint["Total"].get<int>();

	for (int i = 1; i <= total; i++)
	{
		std::string num = std::to_string(i);
		VECTOR f = { BillPoint[num]["VECTOR"]["x"].get<float>(),BillPoint[num]["VECTOR"]["y"].get<float>(),BillPoint[num]["VECTOR"]["z"].get<float>()};
		testPoint_[i] = f;
	}
}

const VECTOR SwingPoint::SetSwingPoint(VECTOR pos, int section)
{
	min = 9999999.0f;
	auto pop = pos;
	pop.y = 0.0f;
	distance_.clear();
	for (int i = 1; i <= total; i++)
	{
		auto p =VSub(pop, testPoint_[i]);
		float pp = abs(p.x + p.z);
		distance_.push_back(pp);
	}
	for (int f =0;f<  distance_.size();f++)
	{
		//if (distance_[f] <= min&& distance_[f]<300.0f)
		if (distance_[f] <= min)
		{
			
			min = distance_[f];
			minNum = f+1;		
		}
	}
	//for (int i = 0; i < 2; i++)
	//{
	//	for ( auto list:sectionList_[static_cast<Stage::STAGE_NUM>(i)])
	//	{
	//		for (int m = 0; m < 4; m++)
	//		{
	//			list.second[static_cast<SIDE>(0)];
	//		}
	//	}
	//}
	auto BuildingList = sectionList_[static_cast<Stage::STAGE_NUM>(section-1)];
	auto swingSide = BuildingList[2];
	auto swingPointOptions = swingSide[static_cast<SIDE>(section+1)];

	VECTOR swingPoint = swingPointOptions[2];
	return 	testPoint_[minNum];
}

const VECTOR SwingPoint::SetGravity(VECTOR PlayerPos)
{
	float z;
	float x;
	auto tz = abs(testPoint_[minNum].z);
	auto tx = abs(testPoint_[minNum].x);
	auto px = abs(PlayerPos.x);
	auto pz = abs(PlayerPos.z);
	if (tz <= pz)
	{
		z = 1.0f;
	}
	else {
		z = -1.0f;

	}
	if (tx <= px)
	{
		x = 1.0f;
	}
	else {
		x = -1.0f;

	}

	return VECTOR{x,-10.0f,z};
}

float SwingPoint::Magnitude(VECTOR pos) const
{
	 return sqrtf(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);
	
}

