#include<DxLib.h>
#include "Wire.h"
#include"Player.h"

Wire::Wire(Player& player ):player_(player)
{
}

Wire::~Wire()
{
}

void Wire::Update()
{
	(this->*_phase)();

}

void Wire::Draw()
{
	auto pPos = player_.GetPos();
	DrawCircle(pos_.x,pos_.y,5,0xff0000);
	DrawLine(pPos.x, pPos.y, pos_.x, pos_.y, 0xffffff);
}

void Wire::SwingPhase()
{

}

void Wire:: AnchoringPhase()
{
	

	//êÃéˆã∆Ç≈çÏÇ¡ÇΩVectorÇégÇ¡ÇƒÇ¢ÇÈÇπÇ¢Ç≈ÇﬂÇÒÇ«Ç≠Ç≥Ç¢Ç±Ç∆Ç…Ç»Ç¡ÇƒÇ¢ÇÈ
	VECTOR pos = { pos_.x,pos_.y };
	pos = VAdd(pos, movePow_);

	pos_.x = pos.x;
	pos_.y = pos.y;
}

void Wire::SetPalam()
{

	pos_ = player_.GetPos();
	moveVec_ = player_.GetMoveVec();
	VECTOR moveVec = { player_.GetMoveVec().x,player_.GetMoveVec().y };

	movePow_ = VScale(moveVec, 3.0f);
	_phase = &Wire::AnchoringPhase;
}

bool Wire::IsHitHook()
{
	for (const auto& col :player_. wallcolList_)
	{
		Raycast::Ray ray = { pos_,moveVec_ };
		if (rayCast_.CheckCollision(ray, col, pos_))
		{		
			return false;
		}
	}

	return true;
}
