#include "TimeCount.h"
#include<DxLib.h>

TimeCount::TimeCount():time_(0.0f),deltaTime_(0.000001f),elapsedTime_(0.0f), nowTime_(0.0f)
{
	time_ = GetNowHiPerformanceCount();
}

TimeCount::~TimeCount()
{
}

void TimeCount::Update()
{
	nowTime_ = GetNowHiPerformanceCount();
	deltaTime_ = (nowTime_ - time_) / 1000000.0f;
	time_ = nowTime_;

	elapsedTime_ += deltaTime_;
}

void TimeCount::Draw()
{
	DrawFormatStringF(0.0f, 50.0f, 0xff0000, "deltaTime_%f", elapsedTime_);
}
