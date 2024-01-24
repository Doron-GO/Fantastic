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

	bool startFlag_;//�v���J�n�t���O;
	float deltaTime_ ;
	float nowTime_ ;
	float elapsedTime_;
	float oldTime_;
};

