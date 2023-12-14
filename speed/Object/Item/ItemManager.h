#pragma once
#include"../../Vector2D.h"
class ItemManager
{
public:
	ItemManager();
	~ItemManager();

	void Update();
	void Draw();

private:

	Vector2DFloat pos_;

};

