#include<DxLib.h>
#include "Wire.h"
#include"Player.h"

Wire::Wire(Player& player ):player_(player)
{
	_phase = &Wire::AnchoringPhase;

}

Wire::~Wire()
{
}

void Wire::Update()
{
	(this->*_phase)();

}

void Wire::Draw(Vector2DFloat cameraPos)
{
	auto pPos = player_.pos_;
	pPos += cameraPos;
	DrawCircle(pos_.x+ cameraPos.x,pos_.y+ cameraPos.y,5,0xff0000);
	DrawLine(pPos.x, pPos.y, pos_.x + cameraPos.x, pos_.y + cameraPos.y, 0xffffff);

	DrawFormatStringF(0, 160, 0xffffff, "DiagonallyVecVec:x.%f,y.%f", player_.GetDiagonallyVecVec().x, player_.GetDiagonallyVecVec().y);
}

void Wire::SwingPhase()
{

}

void Wire:: AnchoringPhase()
{
	

	//êÃéˆã∆Ç≈çÏÇ¡ÇΩVectorÇégÇ¡ÇƒÇ¢ÇÈÇπÇ¢Ç≈ÇﬂÇÒÇ«Ç≠Ç≥Ç¢Ç±Ç∆Ç…Ç»Ç¡ÇƒÇ¢ÇÈ
}

void Wire::SetPalam()
{
	pos_ = player_.pos_;
	VECTOR pos = { pos_.x,pos_.y };
	VECTOR moveVec = { player_.GetDiagonallyVecVec().x,player_.GetDiagonallyVecVec().y };

	moveVec=VNorm(moveVec);
	movePow_ = VScale(moveVec, 100.0f);
	movedPos_ = VAdd(pos, movePow_);

	pos_.x = movedPos_.x;
	pos_.y = movedPos_.y;

	_phase = &Wire::SwingPhase;
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
