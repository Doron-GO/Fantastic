#pragma once
#include<vector>
#include<memory>
#include"../../Vector2D.h"
#include"Missile.h"
class ItemManager
{
public:
	enum class ItemList
	{
		NON,
		MISSILE,
	};

	ItemManager();
	~ItemManager();

	void Update();
	void Draw();
	//void ItemToYou(ItemList itemlist, int num);

private:

	Vector2DFloat pos_;
	ItemList item_;
};

