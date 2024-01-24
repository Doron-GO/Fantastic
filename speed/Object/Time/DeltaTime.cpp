#include "DeltaTime.h"
#include<DxLib.h>

void DeltaTime::update()
{	
	nowTime_ = GetNowHiPerformanceCount();
	deltaTime_ = (nowTime_ - oldTime_) / 1000000.0f;
	oldTime_ = nowTime_;

	elapsedTime_ += deltaTime_;

}

const float DeltaTime::GetDeltaTime()
{
	return deltaTime_;
}

const float DeltaTime::GetElapsedTime()
{
	return elapsedTime_;
}

void DeltaTime::SetStart()
{
	oldTime_ = GetNowHiPerformanceCount();

}

DeltaTime::DeltaTime() :oldTime_(0.0f), deltaTime_(0.000001f), elapsedTime_(0.0f), nowTime_(0.0f)
{

}

DeltaTime::~DeltaTime()
{
}
