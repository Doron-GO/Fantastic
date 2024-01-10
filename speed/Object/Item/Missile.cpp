#include<DxLib.h>
#include "Missile.h"

Missile::Missile()
{
	count_ = 0;
	_update = &Missile::WaitUpdate;
	_draw = &Missile::MissileDraw;
	type_ = ITEM_TYPE::MISSILE;
	LoadDivGraph("Src/Img/Explosion.png", 11, 11, 1, 32, 31, img_);
	explosionFlag_ = false;

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

	DrawCircle(itemPos_.x+offset.x, itemPos_.y + offset.y, 20, 0xff0000);
	DrawBox(col_.min_.x + offset.x, col_.min_.y + offset.y, col_.max_.x + offset.x, col_.max_.y + offset.y, 0xffaaff, false);
	DrawCircle(itemPos_.x, itemPos_.y , 20, 0xff0000);
	DrawBox(col_.min_.x , col_.min_.y , col_.max_.x , col_.max_.y , 0xffaaff, false);

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
	if (!activateFlag_ && _update == &Missile::ActivateUpdate)
	{
		explosionFlag_ = true;
		_draw = &Missile::ExplosionDraw;
		return true;
	}
	return false;
}

void Missile::ActivateUpdate()
{		
	vel_ = (vel_ + (targetPos_ - itemPos_).Normalized()).Normalized() * 12.0f;
	itemPos_ += vel_;
	if (count_ > 200)
	{
		activateFlag_ = false;
	}
}

void Missile::WaitUpdate()
{

}

void Missile::ExplosionDraw(Vector2DFloat offset)
{
	if (count_ <= 31)
	{
		//DrawGraph(itemPos_.x + offset.x,
		//	(itemPos_.y + offset.y),
		//	img_[(count_ / 3) % 11], true);
		DrawRotaGraph2F(itemPos_.x + offset.x, itemPos_.y + offset.y,
			20.0f, 20.0f,
			2.5, 0.0,
			img_[(count_ / 3) % 11],
			true);

		count_++;
	}
	else
	{
		explosionFlag_ = false;
	}

}

void Missile::MissileDraw(Vector2DFloat offset)
{
	int a;
}


