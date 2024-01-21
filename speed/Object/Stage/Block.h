#pragma once
#include<vector>
#include<memory>
#include"../../Common/Raycast.h"
#include"../../Common/Collision.h"
#include"../../Vector2D.h"

class LoadMap;
class Player;

class Block
{
public:
	Block(LoadMap& loadMap, std::vector<std::shared_ptr<Player>> players);
	~Block();

	void Update();
	void Draw();


private:

	Raycast rayCast_;
	std::vector<Vector2DFloat> blockPos_;
	std::vector<std::shared_ptr<Player>> players_;

	ColList itemBoxColList_;
	LoadMap& loadMap_;

};

