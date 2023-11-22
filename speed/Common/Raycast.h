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
	//最後の引数trueでレイ、falseで矩形
	bool CheckCollision(Ray ray, Collision collision ,Vector2DFloat playerPos);
	bool CheckCollision( Collision collision ,Vector2DFloat playerPos);
	bool CheckRay(Ray ray, Line line, Vector2DFloat offset);
	bool CheckRectangle(Collision collision, Vector2DFloat pos);
	//bool CheckRectangle(Ray ray, Collision collision, Vector2DFloat playerPos)
};

