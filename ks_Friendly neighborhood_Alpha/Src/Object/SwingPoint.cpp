#include "SwingPoint.h"

SwingPoint::SwingPoint(Stage::STAGE_NUM num, Transform transform)
{
	num_ = num;
	transform_ = transform;
	norm_ = { 0.0f,0.0f,0.0f };
	point_ = { 0.0f,0.0f,0.0f };
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
	auto totalBldgNum = json_["TotalBldgNum"].get<int>();

	for (int idx = 1; idx <= totalBldgNum; idx++)
	{
		std::string st = std::to_string(idx);
		std::string BldgNum ="Bldg"+st;
		auto Bldg = json_[BldgNum];
		auto SideNum = Bldg["SideNum"].get<int>();

		for (int idx2 = 1; idx2 <= SideNum; idx2++)
		{
			std::string num = std::to_string(idx);
			std::string Side = "Side" + num;
			auto sideObj = Bldg[Side];

			for (int l = 0; l < 3; l++)
			{
				auto norm= sideObj["Norm"];
				
				norm_ = { norm["x"].get<float>(),norm["y"].get<float>() ,norm["z"].get<float>() };
			}
			
			auto pos = Bldg[Side][]

		}
	}

}

