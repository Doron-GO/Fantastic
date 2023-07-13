#include "SwingPoint.h"
#include<fstream>
#include<DxLib.h>
#include <stdlib.h>
#include<string>



SwingPoint::SwingPoint()
{
	norm_ = VECTOR{ 0.0f,0.0f,0.0f };
}

SwingPoint::~SwingPoint()
{
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

		for (int idx = 1; idx <= totalBldgNum; idx++)
		{
			std::string st = std::to_string(idx);
			std::string BldgNum = "Bldg" + st;
			auto Bldg = Buildings_[BldgNum];
			int VecNum = Bldg["VECTORTotalNum"].get<int>();
			int PointTotalNum = Bldg["PointTotalNum"];
			std::string PointTotalNumSt = std::to_string(PointTotalNum);

			auto SideNum = Bldg["SideNum"].get<int>();

			for (int idx2 = 1; idx2 <= SideNum; idx2++)
			{
				std::string num = std::to_string(idx2);
				std::string Side = "Side" + num;
				auto sideObj = Bldg[Side];

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

}

const VECTOR SwingPoint::SetSwingPoint(VECTOR pos, int section)
{
	auto BuildingList = sectionList_[static_cast<Stage::STAGE_NUM>(section-1)];
	auto swingSide = BuildingList[1];
	auto swingPointOptions = swingSide[static_cast<SIDE>(0)];

	VECTOR swingPoint = swingPointOptions[0];
	return swingPoint;
}

