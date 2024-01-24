#pragma once
#include<DxLib.h>

#define deltaTime DeltaTime::GetInstsnce()

class DeltaTime
{
public:
	static DeltaTime& GetInstsnce()
	{
		static DeltaTime s_Instance;
		return s_Instance;
	}
	void update();

	const float GetDeltaTime();
	const float GetElapsedTime();
	void SetStart();

private:
	DeltaTime();
	~DeltaTime();
	float deltaTime_;
	float nowTime_;
	float elapsedTime_;
	float oldTime_;

};

