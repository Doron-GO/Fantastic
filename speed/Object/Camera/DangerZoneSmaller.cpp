#include "DangerZoneSmaller.h"

Vector2DFloat Min;
Vector2DFloat Max;

DangerZoneSmaller::DangerZoneSmaller(Vector2DFloat& max, Vector2DFloat& min)
	:outSideMax_(max),outSideMin_(min)
{
	Min = { 200.0f,200.0f };
	Max = { 1400.0f,800.0f };
	scaleMin_ = {-200.0f,-150.0f};
	scaleMax_ = {200.0f,150.0f};
	_update = &DangerZoneSmaller::UpdateWait;

}

DangerZoneSmaller::~DangerZoneSmaller()
{

}

void DangerZoneSmaller::Smaller()
{
	if (outSideMax_ >= scaleMax_&& outSideMin_<= scaleMin_)
	{
		Vector2DFloat Vec = { 0.2f, 0.13f };
		//if (count_ % 5 == 0)
		{
			outSideMax_-= Vec;
			outSideMin_ += Vec;
		}
		count_++;
	}
}

void DangerZoneSmaller::Update()
{
	(this->*_update)();

}

void DangerZoneSmaller::UpdateWait()
{

}
void DangerZoneSmaller::Activated()
{
	_update = &DangerZoneSmaller::UpdateSmaller;

}
void DangerZoneSmaller::UpdateSmaller()
{
	Smaller();
}
