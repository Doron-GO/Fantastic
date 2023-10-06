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

	offset_ = (view / 3.0f) - pos_;
	moveVec_ = { 0.0f,20.0f };
	movePow_ = { 0.0f,0.0f };
	dir_LR_ = DIR_LR::LIGHT;
	_phase = &Player::FallPhase;
	_draw = &Player::MoveDraw;
}

void Player::Update(Input& input)
{
	offset_ = (view / 3.0f) - pos_;

	lpAnimMng.UpdateAnime(animeStr_);

	(this->*_phase)(input);
	Move(input);
	
	pos_ += movePow_;

}

void Player::Draw()
{
	//(this->*_draw)();

	Vector2DFloat view = { 800.0f, 600.0f };



	DrawRotaGraph2F(pos_.x+ offset_.x, pos_.y + offset_.y,
		24.0f, 35.0f,
		1.5, 0.0,
		lpImageMng.GetID(animeStr_.imgKey_)[(*animeStr_.animID_)[GraphHD]],
		true, static_cast<int>(dir_LR_), 0);


	DrawFormatStringF(0, 0, 0xffffff, "movePow_(x:%f,y%f)", movePow_.x, movePow_.y);
	DrawFormatStringF(0, 20, 0xffffff, "pos_(x:%f,y%f)", pos_.x, pos_.y);
	if (!Collision())
	{
		DrawString(0, 40, "当たった", 0xffffff);
	}
	//DrawCircle(pos_.x, pos_.y, 15, 0xffffff);
	Vector2DFloat rayCenter = { pos_ - center_ };

	DrawLine(pos_.x+ offset_.x, rayCenter.y + offset_.y,
		moveVec_.x+ pos_.x + offset_.x, moveVec_.y+ rayCenter.y + offset_.y, 0x00ffff);

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
	//もし床がなかったらフォールにする
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
	if(movePow_.y<=-13.0f)
	{
		movePow_.y = 0.0f;
		_phase = &Player::FallPhase;
	}
	movePow_.y += -0.3f;

}

void Player::FallPhase(Input& input)
{
	lpAnimMng.SetAnime(animeStr_, "Fall");

	//床と当たっていなかったら
	if (Collision())
	{
		movePow_.y += 0.2f;
	}

	//落下速度が一定を超えたら決まった値にする
	if (movePow_.y >= 5.8f)
	{
		movePow_.y = 5.8f;
	}
	//接地したら地上移動モードにする
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

		if (rayCast_.CheckCollision(ray, col,pos_+offset_))
		{

			return false;
		}
	}
	return true;

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

	//DrawRotaGraph2F(pos_.x, pos_.y,
	//	24.0f, 35.0f,
	//	1.5, 0.0,
	//	lpImageMng.GetID(animeStr_.imgKey_)[(*animeStr_.animID_)[GraphHD]],
	//	true, static_cast<int>(dir_LR_), 0);

}

void Player::Move(Input& input)
{
	float speed = 0.2f;
	//右もしくは左キーが押されていないとき
	if (!input.IsTrigger("right")&&!input.IsTrigger("left"))
	{
		//移動量が0.1より大きかったら
		if (movePow_.x >= 0.1f) 
		{
			movePow_.x -= speed;
		}
		//移動量が-0.1より小さかったら
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

	//スライディングボダンが押されていない時
	if (!input.IsTrigger("slide"))
	{
		//右キー
		if (input.IsTrigger("right")) 
		{
			dir_LR_ = DIR_LR::LIGHT;
			movePow_.x += 0.2f;
			if (movePow_.x >= 8.0f)
			{
				movePow_.x = 8.0f;
			}
		}
		//左キー
		if (input.IsTrigger("left")) 
		{
			dir_LR_ = DIR_LR::LEFT;
			movePow_.x -= 0.2f;
			if (movePow_.x <= -8.0f)
			{
				movePow_.x = -8.0f;
			}
		}
	}

	//落下中じゃないとき
	if (!(_phase == &Player::FallPhase)&&!(_phase == &Player::JumpPhese))
	{
		//スライディングボタンが押されていたら
		if (input.IsTrigger("slide"))
		{
			lpAnimMng.SetAnime(animeStr_, "Slide");
			//スライディング中は減速する
			if (movePow_.x >= 0.05f)
			{
				movePow_.x -= 0.1f;
			}
			if (movePow_.x <= -0.05f)
			{
				movePow_.x += 0.1f;
			}
		}
	}

}

void Player::Jump(Input& input)
{
	//上キーを押したとき
	if (input.IsTrigger("up"))
	{
		lpAnimMng.SetAnime(animeStr_, "Jump");
		movePow_.y = 0.0f;
		_phase = &Player::JumpPhese;

	}
}
