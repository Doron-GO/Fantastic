#pragma once
#include<vector>
#include"../../Vector2D.h"
#include"../../Common/Collision.h"

class ItemBox
{
public:
	ItemBox(ColList itemBoxCol);
	~ItemBox();

private:
	std::vector<Vector2DFloat> boxPos_;
	bool IsBox(Vector2DFloat pos);
	ColList itemBoxColList_;
};

