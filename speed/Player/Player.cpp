#include<DxLib.h>
#include "Player.h"
#include"../Input/Input.h"
#include"../Obj/ImageMng.h"

Player::Player()
{
	Init();
}

Player::~Player()
{
}

void Player::Init()
{
	pos_another_way = { 100.0f,100.0f };
	runFrame_ = 0;
	LoadDivGraphF("./Src/Img/Run.png",
		6, 1, 6, 49, 37.0f, runImg_,true);

	moveVec_ = { 1.0f,0.0f };
	movePow_ = { 0.0f,0.0f };
	dir_LR_ = DIR_LR::LIGHT;
}

void Player::Update(Input& input)
{
	Move(input);
	
}

void Player::Draw()
{

	
 	//DrawRotaGraph2F(pos_.x, pos_.y,
		//24.0f, 35.0f,
		//1.5, 0.0,
		//runImg_[runFrame_ / 5],
		//true, static_cast<int>(dir_LR_),0);
 	DrawRotaGraph2F(pos_another_way.x, pos_another_way.y,
		24.0f, 35.0f,
		1.5, 0.0,
		runImg_[runFrame_ / 5],
		true, static_cast<int>(dir_LR_),0);
	
	//DrawGraphF(pos_.x, pos_.y, runImg_[runFrame_/5], true);
	//DrawBoxAA(pos_.x, pos_.y, pos_.x+20.0f, pos_.y+20.0f, 0xffffff, true);
	if (runFrame_++ >= 29)
	{
		runFrame_ = 0;
	}

	DrawFormatStringF(0, 0, 0xffffff, "movePow_(x:%f,y%f)", movePow_.x, movePow_.y);
	//DrawFormatStringF(0, 20, 0xffffff, "pos_(x:%f,y%f)", pos_.x, pos_.y);
	DrawFormatStringF(0, 20, 0xffffff, "pos_(x:%f,y%f)", pos_another_way.x, pos_another_way.y);

	//DrawLine(pos_.x, pos_.y, (moveVec_.x*100)+ pos_.x, moveVec_.y+ pos_.y, 0x00ffff);
	DrawLine(pos_another_way.x, pos_another_way.y,
		(moveVec_.x*100)+ pos_another_way.x, moveVec_.y+ pos_another_way.y, 0x00ffff);
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


	auto move= VScale(moveVec_, movePow_.x);
	//pos_.x += move.x;
	//pos_.y += move.y;
	pos_another_way.x += move.x;
	pos_another_way.y += move.y;


}

void Player::Jump(Input& input)
{
}
