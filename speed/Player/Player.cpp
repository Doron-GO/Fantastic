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
	pos_ = { 100.0f,100.0f };
	runFrame_ = 0;
	LoadDivGraphF("./Src/Img/Run.png",
		6, 1, 6, 49, 37.0f, runImg_,true);

}

void Player::Update(Input& input)
{
	Move(input);
	
}

void Player::Draw()
{

	DrawGraphF(pos_.x, pos_.y, runImg_[runFrame_/5], true);
	//DrawBoxAA(pos_.x, pos_.y, pos_.x+20.0f, pos_.y+20.0f, 0xffffff, true);
	if (runFrame_++ >= 29)
	{
		runFrame_ = 0;
	}

}

void Player::Move(Input& input)
{
	if (input.IsTrigger("right")) 
	{
		pos_.x += 2.0f;
	}
	if (input.IsTrigger("left")) 
	{
		pos_.x += -2.0f;
	}

}
