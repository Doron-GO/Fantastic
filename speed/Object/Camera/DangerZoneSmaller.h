#pragma once
#include"../../Vector2D.h"
class OutSide;
class DangerZoneSmaller
{
public:
	DangerZoneSmaller(Vector2DFloat& max, Vector2DFloat& min);
	~DangerZoneSmaller();
	void Smaller();

	void UpdateSmaller();
	void Update();
	void UpdateWait();
	void (DangerZoneSmaller::* _update)();
	void Activated();

private:


	Vector2DFloat scaleMin_;
	Vector2DFloat scaleMax_;

	Vector2DFloat& outSideMax_;
	Vector2DFloat& outSideMin_;

	int count_;

};

