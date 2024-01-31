#pragma once
#include<vector>
#include<memory>
#include"../../Common/Raycast.h"
#include"../../Common/Collision.h"
#include"../../Vector2D.h"

class LoadMap;
class Player;

class Blocks
{

public:
	Blocks(LoadMap& loadMap);
	~Blocks();

	void Update();
	void Draw(Vector2DFloat cameraPos);
	struct Block
	{
		bool blockFlag_;
		Collision col_;
		int count_;
	};
	std::list<Block> &GetBlockList();

private:
	std::list<Block> blocks_;
	Raycast rayCast_;
	LoadMap& loadMap_;


};

