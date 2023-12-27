#include<DxLib.h>
#include "Missile.h"

Missile::Missile()
{
	itemPos_ = { 600.0f,200.0f };
	test = { 990.0f,200.0f };
}

Missile::~Missile()
{
}

void Missile::Update()
{

}

void Missile::Draw(Vector2DFloat offset)
{
	DrawCircle(itemPos_.x+offset.x, itemPos_.y + offset.y, 20, 0xff0000);
}

