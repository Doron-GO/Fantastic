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
	void ResetCounter();

	int count_;

private:


	Vector2DFloat scaleMin_;
	Vector2DFloat scaleMax_;

	Vector2DFloat& outSideMax_;//死亡エリアの座標の最大値
	Vector2DFloat& outSideMin_;//死亡エリアの座標の最小値

};

