#pragma once
#include<vector>
#include<memory>
#include"../../Vector2D.h"
#include"../../Common/Raycast.h"
#include"../../Common/Collision.h"

class LoadMap;
class Player;

class ItemBox
{
public:
	ItemBox( LoadMap& loadMap, std::vector<std::shared_ptr<Player>> players);
	~ItemBox();
	void Draw(Vector2DFloat offset );
	void Update();
private:
	
	Raycast rayCast_;
	std::vector<Vector2DFloat> boxPos_;
	std::vector<std::shared_ptr<Player>> players_;

	bool IsBox(Vector2DFloat pos);
	ColList itemBoxColList_;
	LoadMap& loadMap_;
	int itemBoxImg_[11];
	int count_;
};

