#include<DxLib.h>
#include "OutSide.h"
#include"Camera.h"
#include"../../Player/Player.h"

Vector2DFloat screenSize ={1600.0f,1000.0f};

OutSide::OutSide(Camera& camera, std::vector< std::shared_ptr<Player >>& players) :camera_(camera), players_(players),
minPos_({ 0.0f,0.0f }), maxPos_({ 1600.0f, 1000.0f })
{
	LoadDivGraph("Src/Img/Explosion.png",11, 11, 1, 32, 31, bombImg_);
	LoadDivGraph("Src/Img/BigExplosion.png",8, 8, 1, 32, 32, bigBombImg_);
	//bombImg_ = LoadGraph("Src/Img/Explosion.png");
}
OutSide::~OutSide()
{
}

void OutSide::Update()
{
	IsDead();
}

void OutSide::ExplosionUpdate()
{

}

void OutSide::Draw(Vector2DFloat offset)
{
	auto target = camera_.GetTargetPos();
	DrawFormatStringF(0.0f, 400.0f, 0xffffff, "upperPos_:x %f, y %f,", upperPos_.x, upperPos_.y);
	DrawFormatStringF(0.0f, 440.0f, 0xffffff, "lowerPos_:x %f, y %f,", lowerPos_.x, lowerPos_.y);

	minPos_ =target + Vector2DFloat{-800.0f,-500.0f};
	maxPos_ = target + Vector2DFloat{ 800.0f,500.0f };


	if (isExploding_)
	{
		if (upperVec_.y < 0 && upperPos_.y <= 0)
		{
			upperVec_ = { 20.0f,0.0f };
		}

		if (upperVec_.x> 0 && upperPos_.x >= screenSize.x)
		{
			upperVec_ = { 0.0f,20.0f };
		}
		if (upperVec_.y > 0 && upperPos_.y >= screenSize.y)
		{
			upperVec_ = {-20.0f ,0.0f};
		}	
		if (upperVec_.x < 0 && upperPos_.x <= 0.0f)
		{
			upperVec_ = { 0.0f ,-20.0f };
		}


		upperPos_ += upperVec_;
		if (lowerVec_.y > 0 && lowerPos_.y >= screenSize.y)
		{
			lowerVec_ = { 20.0f ,0.0f };
		}
		if (lowerVec_.x > 0 && lowerPos_.x >= screenSize.x)
		{
			lowerVec_ = { 0.0f,-20.0f };
		}
		if (lowerVec_.y < 0 && lowerPos_.y<= 0.0f)
		{
			lowerVec_ = { -20.0f ,0.0f };
		}
		if (lowerVec_.x < 0 && lowerPos_.x<= 0.0f)
		{
			lowerVec_ = { 0.0f ,20.0f };
		}

		lowerPos_ += lowerVec_;

		//ˆê’èŽžŠÔ‚²‚Æ‚É”š”­‚³‚¹‚é
		if (frame_ % 5 == 0)
		{
			bombs_.emplace_back(upperPos_);
			bombs_.emplace_back(lowerPos_);
		}

		//ã‚©‚ç‚Ì”š’e‚Æ‰º‚©‚ç‚Ì”š’e‚ªd‚È‚Á‚½‚ç‚Ç‚Á‚¿‚àÁ‚·
		if(upperPos_ ==lowerPos_)
		{
			bombs_.clear();
			isExploding_ = false;
			bigExploding_ = true;
			bigFrame_ = 0;
		}
		frame_++;
	}

	if (isExploding_)
	{
		DrawRotaGraph2F(upperPos_.x, upperPos_.y,
			17.5, 16.0f,
			3.5, 0.0,
			bombImg_[frame_ / 2],
			true, 0, 0);

		DrawRotaGraph2F(lowerPos_.x, lowerPos_.y,
			17.5, 16.0f,
			3.5, 0.0,
			bombImg_[frame_ / 2],
			true, 0, 0);

		for (auto& b : bombs_)
		{
			DrawRotaGraph2F(b.pos_.x, b.pos_.y,
				17.5, 16.0f,
				3.5, 0.0,
				bombImg_[(b.frame_/2)],
				true, 0, 0);

		}
		for (auto& b : bombs_)
		{
			b.frame_++;
			if (frame_>220)
			{
				b.isDead = true;
			}
		}
		bombs_.remove_if([](const Bomb& b)
		{
			return b.isDead;
		});
	}

	if (bigExploding_)
	{

		DrawRotaGraph2F(lowerPos_.x, lowerPos_.y,
			16.0f, 16.0f,
			8.5, 0.0,
			bigBombImg_[bigFrame_ / 4],
			true, 0, 0);
		bigFrame_++;
	}
	if (bigFrame_ > 360)
	{
		bigExploding_ =false;
	}

}

void OutSide::IsDead()
{
 	for (const auto& player : players_)
	{
		if (player->IsAlive())
		{
			if (!IsOutSide(player->GetPos()))
			{
				UpDownORLeftRight(player->GetPos());
				player->Dead();
				isExploding_ = true;
				frame_ = 0;
			}
		}
	}
}

bool OutSide::IsOutSide(Vector2DFloat pos)
{
	return (minPos_.x < pos.x && pos.y> minPos_.y&&
		maxPos_.x>pos.x && maxPos_.y>pos.y);
}

//bool OutSide::UpDownORLeftRight(Vector2DFloat pos)
//{
//	if (minPos_.x > pos.x || maxPos_.x<pos.x)
//	{
//		return false; 
//	}
//	else if (pos.y < minPos_.y || maxPos_.y < pos.y)
//	{
//		return true;
//	}
//}
void OutSide::UpDownORLeftRight(Vector2DFloat pos)
{
	if (pos.y < minPos_.y || maxPos_.y < pos.y)
	{
		UpORDown(pos);
	}
	else if (minPos_.x > pos.x || maxPos_.x<pos.x)
	{
		LeftORRight(pos);
	}
}

void OutSide::UpORDown(Vector2DFloat pos)
{
	Vector2DFloat up;
	Vector2DFloat low;

	if (pos.y < minPos_.y )
	{
		up = { pos.x ,0.0f };
		upperVec_ = { 20.0f,0.0f };
		low = { pos.x, 0.0f };
		lowerVec_ = { -20.0f ,0.0f };
	}
	else if(maxPos_.y < pos.y)
	{
		up = { pos.x ,1000.0f };
		upperVec_ = { 20.0f,0.0f };
		low = { pos.x, 1000.0f };
		lowerVec_ = { -20.0f ,0.0f };
	}
	upperPos_ = up;
	lowerPos_ = low;
}

void OutSide::LeftORRight(Vector2DFloat pos)
{
	Vector2DFloat up;
	Vector2DFloat low;

	if (pos.x >= maxPos_.x)
	{
		up = { screenSize.x ,pos.y };
		upperVec_ = { 0.0f,-20.0f };
		low = { screenSize.x, pos.y };
		lowerVec_ = { 0.0f ,20.0f };
	}
	else if (minPos_.x >= pos.x)
	{
		up = { 0.0f ,pos.y };
		upperVec_ = { 0.0f,-20.0f };
		low = {0.0f, pos.y };
		lowerVec_ = { 0.0f ,20.0f };
	}
	upperPos_ = up;
	lowerPos_ = low;
}
