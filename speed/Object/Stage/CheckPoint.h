#pragma once
#include"../../Vector2D.h"
#include"../../Common/Collision.h"
#include"../../Common/Raycast.h"
#include<vector>
#include<memory>
#include<list>

class Player;
using vec =Vector2DFloat;
using CHECKPOINT= std::pair<bool, vec>;
using CHECKPOINTS= std::vector < std::pair<bool, Vector2DFloat>>;

class CheckPoint
{
public:

	CheckPoint(std::vector<std::shared_ptr<Player>> players, PointColList checkpoint);
	~CheckPoint();

	void Update();
	void Draw(Vector2DFloat pos);
	Vector2DFloat GetCheckPoint()const; 
	CHECKPOINT GetCheckPoint2()const;

private:
	std::vector<std::shared_ptr<Player>> players_;
	std::vector<Vector2DFloat> checkPoints_;
	CHECKPOINTS checkPoints3_;
	PointColList checkPointColList_;//チェックポイント当たり判定
	Raycast rayCast_;
	PointColList checkPointColList2_;
	int currentPoint_;

};

