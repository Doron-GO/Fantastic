#pragma once

class TimeCount
{
public:
	TimeCount();
	~ TimeCount();

	void Update(float startime);
	void Draw();
	void SetStart();
	float ElapsedTime();
private:

	void (TimeCount::* _update)();

	bool startFlag_;//計測開始フラグ;
	float deltaTime_ ;
	float nowTime_ ;
	float elapsedTime_;
	float oldTime_;
};

