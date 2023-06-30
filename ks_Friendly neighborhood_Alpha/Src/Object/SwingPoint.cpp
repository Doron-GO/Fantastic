#include "SwingPoint.h"


SwingPoint::SwingPoint()
{
	Load();
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

	json_ = json_["Building1"]["Bldg1"];

	

}
