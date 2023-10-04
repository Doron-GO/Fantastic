#include<DxLib.h>
#include "Player.h"
#include"../Input/Input.h"
#include"../Obj/ImageMng.h"
#include"../_debug/_DebugConOut.h"
#include"../_debug/_DebugDispOut.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Init(ColList colList)
{
	pos_ = { 300.0f,10.0f };

	center_ = { 0.0f,13.0f };
	colList_ = colList;

	lpAnimMng.LoadAnime("Src/Img/act.list");

	animeStr_.objID_ = "Player";

	lpAnimMng.SetAnime(animeStr_, "Idle");


	moveVec_ = { 0.0f,20.0f };
	movePow_ = { 0.0f,0.0f };
	dir_LR_ = DIR_LR::LIGHT;
	_phase = &Player::FallPhase;
	_draw = &Player::MoveDraw;
}

void Player::Update(Input& input)
{
	lpAnimMng.UpdateAnime(animeStr_);


	(this->*_phase)(input);
	Move(input);
	Vector2DFloat view = { 800.0f, 600.0f };
	
	auto offset = (view / 4.0f) - pos_;
	

	//if (Collision())
	//{
	//	pos_.y += movePow_.y;

	//}


}

void Player::Draw()
{
	(this->*_draw)();

	DrawFormatStringF(0, 0, 0xffffff, "movePow_(x:%f,y%f)", movePow_.x, movePow_.y);
	DrawFormatStringF(0, 20, 0xffffff, "pos_(x:%f,y%f)", pos_.x, pos_.y);
	if (!Collision())
	{
		DrawString(0, 40, "当たった", 0xffffff);
	}
	//DrawCircle(pos_.x, pos_.y, 15, 0xffffff);
	Vector2DFloat rayCenter = { pos_ - center_ };

	DrawLine(pos_.x, rayCenter.y,
		moveVec_.x+ pos_.x, moveVec_.y+ rayCenter.y, 0x00ffff);

}

const Vector2DFloat Player::GetPos()
{
	return pos_;
}

void Player::IdlePhase(Input& input)
{

}

void Player::MovePhase(Input& input)
{
	//Move(input);
	if (Collision())
	{
		_phase = &Player::FallPhase;

	}
	Jump(input);
}

void Player::JumpPhese(Input& input)
{
	lpAnimMng.SetAnime(animeStr_, "Jump");

	movePow_.y += -0.2f;
	
	if(movePow_.y<=-6.0f)
	{
		movePow_.y = 0.0f;
		_phase = &Player::FallPhase;
	}

}

void Player::FallPhase(Input& input)
{
	lpAnimMng.SetAnime(animeStr_, "Fall");

	if (Collision())
	{
		movePow_.y += 0.2f;
	}

	if (movePow_.y >= 5.8f)
	{
		movePow_.y = 5.8f;
	}
	if (!Collision())
	{
		movePow_.y = 0.0f;
		_phase = &Player::MovePhase;
	}
}

bool Player::Collision()
{

	Vector2DFloat rayCenter = { pos_-center_};

	for (const auto& col : colList_)
	{
		Raycast::Ray ray = { rayCenter,moveVec_};

		if (rayCast_.CheckCollision(ray, col, pos_))
		{
			return false;
		}
		return true;

	}

}

void Player::IdleDraw()
{
	lpAnimMng.SetAnime(animeStr_, "Idle");

}

void Player::JumpDraw()
{
	lpAnimMng.SetAnime(animeStr_, "Jump");

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

		//ジャンプアニメーション中じゃなかったら
		if (_phase == &Player::MovePhase)
		{
			lpAnimMng.SetAnime(animeStr_, "Idle");
		}
	}
	else 
	{
		//ジャンプアニメーション中じゃなかったら
		if (_phase == &Player::MovePhase)
		{
			lpAnimMng.SetAnime(animeStr_, "Run");
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
	pos_ += movePow_;

	//pos_.x += move.x;
	//pos_.y += move.y;
}

void Player::Jump(Input& input)
{
	if (input.IsTrigger("up"))
	{
		lpAnimMng.SetAnime(animeStr_, "Jump");
		movePow_.y = -2.0f;
		_phase = &Player::JumpPhese;

	}
}
