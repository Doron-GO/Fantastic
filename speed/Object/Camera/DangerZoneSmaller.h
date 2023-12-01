#pragma once
#include"../../Vector2D.h"
class OutSide;
class DangerZoneSmaller
{
public:
	 
	void Smaller(Vector2DFloat& max, Vector2DFloat& min);

private:

	Vector2DFloat scale_;

	//Vector2DFloat& outSideMax_;
	//Vector2DFloat& outSideMin_;

	int count_;

};

