#include<DxLib.h>
#include "Player.h"
#include"../Input/Input.h"
#include"../Obj/ImageMng.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Init(ColList colList)
{
	pos_ = { 100.0f,100.0f };

	colList_ = colList;

	lpAnimMng.LoadAnime("Src/Img/act.list");

	animeStr_.objID_ = "Player";

	lpAnimMng.SetAnime(animeStr_, "Idle");


	moveVec_ = { 1.0f,0.0f };
	movePow_ = { 0.0f,0.0f };
	dir_LR_ = DIR_LR::LIGHT;
	_phase = &Player::MovePhase;
	_draw = &Player::MoveDraw;
}

void Player::Update(Input& input)
{
	lpAnimMng.UpdateAnime(animeStr_);

	(this->*_phase)(input);
	Collision();
	pos_.x += movePow_.x;
	pos_.y += movePow_.y;


}

void Player::Draw()
{
	(this->*_draw)();

	DrawFormatStringF(0, 0, 0xffffff, "movePow_(x:%f,y%f)", movePow_.x, movePow_.y);
	DrawFormatStringF(0, 20, 0xffffff, "pos_(x:%f,y%f)", pos_.x, pos_.y);

	DrawLine(pos_.x, pos_.y,
		(moveVec_.x*100)+ pos_.x, moveVec_.y+ pos_.y, 0x00ffff);
}

void Player::IdlePhase(Input& input)
{

}

void Player::MovePhase(Input& input)
{
	Move(input);
	Jump(input);
}

void Player::JumpPhese(Input& input)
{




}

void Player::FallPhase(Input& input)
{


}

bool Player::Collision()
{
	Vector2DFloat rayCenter = { pos_ + center_ };

	for (const auto& col : colList_)
	{
		Raycast::Ray ray = { rayCenter,moveVec_ };

		if (rayCast_.CheckCollision(ray, col))
		{
			return false;
		}
		return true;

	}

}

void Player::IdleDraw()
{

}

void Player::JumpDraw()
{

}

void Player::MoveDraw()
{


	DrawRotaGraph2F(pos_.x, pos_.y,
		24.0f, 35.0f,
		1.5, 0.0,
		lpImageMng.GetID(animeStr_.imgKey_)[(*animeStr_.animID_)[GraphHD]],
		true, static_cast<int>(dir_LR_), 0);

}

void Player::Move(Input& input)
{
	float speed = 0.2f;
	if (!input.IsTrigger("right")&&!input.IsTrigger("left"))
	{
		if (movePow_.x >= 0.1f) 
		{
			movePow_.x -= speed;
		}
		if (movePow_.x <= -0.1f)
		{
			movePow_.x += speed;
		}
		lpAnimMng.SetAnime(animeStr_, "Idle");
	}
	else 
	{
		lpAnimMng.SetAnime(animeStr_, "Run");
	}

	if (input.IsTrigger("right")) 
	{
		dir_LR_ = DIR_LR::LIGHT;
		movePow_.x += 0.2f;
		if (movePow_.x >= 6.0f)
		{
			movePow_.x = 6.0f;
		}
	}
	if (input.IsTrigger("left")) 
	{
		dir_LR_ = DIR_LR::LEFT;
		movePow_.x -= 0.2f;
		if (movePow_.x <= -6.0f)
		{
			movePow_.x = -6.0f;

		}
	}

	//pos_.x += move.x;
	//pos_.y += move.y;
}

void Player::Jump(Input& input)
{

	if (input.IsTrigger("down"))
	{
		if (movePow_.y >= -6.0f)
		{
			movePow_.y -= 0.2f;
		}
	}
	else if(movePow_.y>=0.0f)
	{
		movePow_.y += 0.2f;

	}


}
