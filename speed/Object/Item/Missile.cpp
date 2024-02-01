#include<DxLib.h>
#include "Missile.h"

Missile::Missile()
{
	count_ = 0;
	drawCount_ = 0;
	_update = &Missile::WaitUpdate;
	_draw = &Missile::MissileDraw;
	type_ = ITEM_TYPE::MISSILE;
	LoadDivGraph("Src/Img/Explosion.png", 11, 11, 1, 32, 31, img_);
	missileImg_ = LoadGraph("Src/Img/alamo.png");
	explosionFlag_ = false;
	activateFlag_ = false;

}

Missile::~Missile()
{

}

void Missile::Update()
{
	(this->*_update)();
	col_.min_ = { itemPos_.x - 20.0f,itemPos_.y + 20.0f };
	col_.max_ = { itemPos_.x + 20.0f,itemPos_.y - 20.0f };
}


void Missile::Draw(Vector2DFloat offset)
{
	float deg = atan2f(vel_.y, vel_.x);
	auto rad = 90 * (DX_PI_F / 180.0f);
	auto angle = deg + rad;
	if (activateFlag_)
	{
		DrawRotaGraph2F(itemPos_.x + offset.x, itemPos_.y + offset.y, 4, 10, 3.0, angle, missileImg_, true);
	}
	(this->*_draw)(offset);
}

void Missile::Activate(Vector2DFloat playerpos)
{
	_update = &Missile::ActivateUpdate;
	itemPos_ = playerpos;
	activateFlag_ = true;
}

bool Missile::IsEnd()
{
	if (_update == &Missile::ActivateUpdate &&!activateFlag_ )
	{
		_draw = &Missile::ExplosionDraw;
		explosionFlag_ = true;
		return true;
	}
	return false;
}

void Missile::ActivateUpdate()
{		
	vel_ = (vel_ + (targetPos_ - itemPos_).Normalized()).Normalized() * 12.0f;
	if (count_++ > 50)
	{	
		IsCollision();
	}
	if (activateFlag_) {itemPos_ += vel_;}
}

void Missile::WaitUpdate()
{

}

void Missile::IsCollision()
{
	for (const auto& colG : grndColList_)
	{
		Collision col = { Vector2DFloat{colG.first.x,colG.first.y + colG.second.y},
			Vector2DFloat{colG.first.x + colG.second.x,colG.first.y}};
		if(raycast_.RectToRectCollision(col_.min_, col_.max_,col.first, col.second ))
		{
			activateFlag_ = false;
			_draw = &Missile::ExplosionDraw;

		}
	}
}

void Missile::ExplosionDraw(Vector2DFloat offset)
{
		//DrawGraph(itemPos_.x + offset.x,
		//	(itemPos_.y + offset.y),
		//	img_[(count_ / 3) % 11], true);
	DrawRotaGraph2F(itemPos_.x + offset.x, itemPos_.y + offset.y,
		20.0f, 20.0f,
		2.5, 0.0,
		img_[(drawCount_ / 3) % 11],
		true);

	if (drawCount_++ >= 29)
	{	
		drawCount_ =0;
		explosionFlag_ = false;
	}

}

void Missile::MissileDraw(Vector2DFloat offset)
{
}


