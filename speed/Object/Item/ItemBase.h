#pragma once
#include"../../Vector2D.h"

class ItemBase
{
public:
	ItemBase();

	virtual void Update() = 0;
	virtual void Draw(Vector2DFloat offset) = 0;

	

protected:

	Vector2DFloat itemPos_;
};

