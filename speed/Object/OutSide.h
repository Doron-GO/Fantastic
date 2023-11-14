#pragma once
#include"../Vector2D.h"
#include"../Common/Collision.h"

class Camera;

class OutSide
{
public:
	
	OutSide(Camera& camera);
	~OutSide();

	void Update();
	void Draw();

	bool IsOutSide();


private:

	Camera& camera_;

	Vector2DFloat minPos_;
	Vector2DFloat maxPos_;

};

