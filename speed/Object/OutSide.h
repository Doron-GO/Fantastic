#pragma once
#include"../Vector2D.h"
#include"../Common/Collision.h"
#include<vector>
#include<memory>

class Camera;
class Player;

class OutSide
{
public:
	
	OutSide(Camera& camera,std::vector< std::shared_ptr<Player >>& players);
	~OutSide();

	void Update();
	void Draw();

	//‰æ–ÊŠO‚©‚Ç‚¤‚©‚ð”»’è
	void IsDead();
	bool IsOutSide(Vector2DFloat pos);


private:

	Camera& camera_;
	std::vector<std::shared_ptr<Player>>& players_;


	Vector2DFloat minPos_;
	Vector2DFloat maxPos_;

};

