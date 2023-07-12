#include "SwingPoint.h"
#include<fstream>
#include<DxLib.h>
#include <stdlib.h>


SwingPoint::SwingPoint()
{
	//num_ = num;
	//transform_ = transform;
	//norm_ = { 0.0f,0.0f,0.0f };
	//point_ = { 0.0f,0.0f,0.0f };
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

	json_ = json_["Buildings1"];

	int totalBldgNum = json_["TotalBldgNum"].get<int>();

	for (int idx = 1; idx <= totalBldgNum; idx++)
	{
		std::string st = std::to_string(idx);
		std::string BldgNum ="Bldg"+st;
		auto Bldg = json_[BldgNum];
		int PointTotalNum = Bldg["PointTotalNum"];
		std::string PointTotalNumSt = std::to_string(PointTotalNum);

		auto SideNum = Bldg["SideNum"].get<int>();

		for (int idx2 = 1; idx2 <= SideNum; idx2++)
		{
			std::string num = std::to_string(idx2);
			std::string Side = "Side" + num;
			auto sideObj = Bldg[Side];

			auto norm= sideObj["Norm"];
				
			norm_ = { norm["x"].get<float>(),norm["y"].get<float>() ,norm["z"].get<float>() };
			
			std::vector<VECTOR> p;

			for (int Ptl = 1; Ptl <= 3; Ptl++)
			{
				std::string num2 = std::to_string(Ptl);

 				auto pos = Bldg[Side][num2]["VECTOR"];
				p.push_back( 
					VECTOR{ pos["x"].get<float>(),
					pos["y"].get<float>(),
					pos["z"].get<float>() });
				//pos_ = p;
				if (Ptl == 3)
				{
					swingPoint_[static_cast<SIDE>(idx2-1)] = p;
				}
			}


		}

	}
	swingPoint_;
}

