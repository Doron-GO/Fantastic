#pragma once
#include"../../Vector2D.h"

class ItemBase
{
public:
	 ItemBase();
	 virtual ~ItemBase();

	virtual void Update();
	virtual void Draw(Vector2DFloat offset);


protected:

	Vector2DFloat itemPos_;
};

