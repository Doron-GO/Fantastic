#pragma once
#include"../../Vector2D.h"

class ItemBase
{
public:
	 ItemBase();
	 ~ItemBase();

	virtual void Update();
	virtual void Draw(Vector2DFloat offset);
	virtual void Use(Vector2DFloat pos);


protected:

	Vector2DFloat itemPos_;
};

