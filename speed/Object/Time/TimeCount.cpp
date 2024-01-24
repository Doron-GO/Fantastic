#include "TimeCount.h"
#include<DxLib.h>

TimeCount::TimeCount():oldTime_(0.0f),deltaTime_(0.000001f),elapsedTime_(0.0f), nowTime_(0.0f), startFlag_(false)
{
}

TimeCount::~TimeCount()
{
}

void TimeCount::Update(float startime)
{
	if (startime>=2.0f&&(!startFlag_))
	{
		SetStart();
		startFlag_ = true;
	}
	nowTime_ = GetNowHiPerformanceCount();
	deltaTime_ = (nowTime_ - oldTime_) / 1000000.0f;
	oldTime_ = nowTime_;

	elapsedTime_ += deltaTime_;
}

void TimeCount::Draw()
{
	DrawFormatStringF(0.0f, 50.0f, 0xff0000, "deltaTime_%f", elapsedTime_);
}

void TimeCount::SetStart()
{
	oldTime_ = GetNowHiPerformanceCount();

}

float TimeCount::ElapsedTime()
{
	
	return elapsedTime_;
}
