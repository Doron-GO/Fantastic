#include<DxLib.h>
#include "OutSide.h"
#include"Camera.h"
#include"../../Player/Player.h"

OutSide::OutSide(Camera& camera, std::vector< std::shared_ptr<Player >>& players) :camera_(camera), players_(players),
minPos_({0.0f,0.0f}), maxPos_({160.0f, 100.0f})
{
	_phase = &OutSide::ExplosionUpdate;

}

OutSide::~OutSide()
{
}

void OutSide::Update()
{
	//minPos_ .x+= 0.5f;
	//minPos_ .y+= 0.5f;
	//maxPos_.x -= 0.5f;
	//maxPos_.y -= 0.5f;

	IsDead();

	(this->*_phase)();
}

void OutSide::ExplosionUpdate()
{
	if (upperPos_.y >= 0)
	{
		upperVec_.y = -4.0f;
	}




}

void OutSide::Draw()
{
	auto target = camera_.GetTargetPos();

	//auto min = minPos_ - (maxPos_ /2.0f);
	//auto max = maxPos_+ maxPos_/2.0f;
	//auto min = minPos_ ;
	//auto max = maxPos_;

	minPos_ =target + Vector2DFloat{-800.0f,-500.0f};
	maxPos_ = target + Vector2DFloat{ 800.0f,500.0f };

	//Œ©‚¹‚©‚¯‚Ì˜g
	//DrawBox(min.x, min.y,
	//	max.x, max.y , 0x0000ff, false);

	if (isExploding_)
	{
		for (auto& b : bombs_)
		{
			b.frame_++;
			if (frame_>60)
			{
				b.isDead = true;
			}
		}
		bombs_.remove_if([](const Bomb& b)
			{
				return b.isDead;
			});
	}


}

void OutSide::IsDead()
{
 	for (const auto& player : players_)
	{
		if (!IsOutSide(player->GetPos()))
		{
			player->Dead();
			upperPos_ = player->GetPos();
			lowerPos_ = player->GetPos();
		}
		else
		{
			player->Alive();
		}
	}
}

bool OutSide::IsOutSide(Vector2DFloat pos)
{

	return (minPos_.x <= pos.x && pos.y>= minPos_.y&&
		maxPos_.x>=pos.x && maxPos_.y>=pos.y);
}
