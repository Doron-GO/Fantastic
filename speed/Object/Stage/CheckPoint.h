#pragma once
#include"../../Vector2D.h"
#include"../../Common/Collision.h"
#include"../../Common/Raycast.h"
#include<vector>
#include<memory>
#include<list>

class Player;
using vec =Vector2DFloat;

class CheckPoint
{
public:

	CheckPoint(std::vector<std::shared_ptr<Player>>& players);
	~CheckPoint();


	void Update();
	void Draw(Vector2DFloat pos);

	bool IsCheckPoint(Vector2DFloat player);

	Vector2DFloat GetCheckPoint()const; 

private:
	std::vector<std::shared_ptr<Player>>& players_;
	std::vector<Vector2DFloat> checkPoints_;
	PointColList checkPointColList_;//チェックポイント当たり判定
	Raycast rayCast_;

	int currentPoint_;

};

