#pragma once

class TimeCount
{
public:
	TimeCount();
	~ TimeCount();

	void Update();
	void Draw();
private:

	void (TimeCount::* _dupdate)();


	float deltaTime_ ;
	float nowTime_ ;
	float elapsedTime_;
	float time_;
};

