#include "SwingPoint.h"

SwingPoint::SwingPoint(Stage::STAGE_NUM num, Transform transform)
{
	num_ = num;
	transform_ = transform;
}

SwingPoint::~SwingPoint()
{
} 

void SwingPoint::Load(void)
{
}

void SwingPoint::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}
