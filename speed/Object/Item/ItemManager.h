#pragma once
#include<vector>
#include<memory>
#include"../../Vector2D.h"
#include"ItemBase.h"
#include"Missile.h"
class ItemManager
{
public:
	enum class ItemList
	{
		NON,
		MISSILE,
	};

	ItemManager(ItemBase item);
	~ItemManager();

	void Update();
	void Draw();
	//void ItemToYou(ItemList itemlist, int num);

private:

	Vector2DFloat pos_;
	ItemList item_;
};

