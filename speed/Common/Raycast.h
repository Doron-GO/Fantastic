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
	//�Ō�̈���true�Ń��C�Afalse�ŋ�`
	bool CheckCollision(Ray ray, Collision collision ,Vector2DFloat playerPos);
	bool CheckCollision( Collision collision ,Vector2DFloat playerPos);
	bool CheckRay(Ray ray, Line line, Vector2DFloat offset);
	bool CheckRectangle(Collision collision, Vector2DFloat pos);
	//bool CheckRectangle(Ray ray, Collision collision, Vector2DFloat playerPos)
};

