#pragma once
#include"../Vector2D.h"
#include"Collision.h"
struct Raycast
{
	struct Ray {

		Vector2DFloat p; //始点
		Vector2DFloat vec;//方向ベクトル
	};
	struct Line {
		Vector2DFloat p; //始点
		Vector2DFloat end;//終点

	};
	bool CheckCollision(Ray ray, Collision collision ,Vector2DFloat offset);
	bool CheckRay(Ray ray, Line line);

};

