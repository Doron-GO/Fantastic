#pragma once
#include<vector>
#include"../../Vector2D.h"
class ItemBox
{
public:
	ItemBox();
	~ItemBox();


private:

	std::vector<Vector2DFloat> boxPos_;

	bool IsBox(Vector2DFloat pos);

};

