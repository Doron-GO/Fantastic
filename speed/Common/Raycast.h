#pragma once
#include"../Vector2D.h"
#include"Collision.h"
struct Raycast
{
	struct Ray {

		Vector2DFloat p; //�n�_
		Vector2DFloat vec;//�����x�N�g��
	};
	struct Line {
		Vector2DFloat p; //�n�_
		Vector2DFloat end;//�I�_

	};
	bool CheckCollision(Ray ray, Collision collision ,Vector2DFloat offset);
	bool CheckRay(Ray ray, Line line);

};

