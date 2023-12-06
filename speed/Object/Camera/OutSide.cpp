#include<DxLib.h>
#include "OutSide.h"
#include"Camera.h"
#include"../../Player/Player.h"
#include"DangerZoneSmaller.h"

Vector2DFloat screenSize ={1600.0f,1000.0f};

OutSide::OutSide(Camera& camera, std::vector< std::shared_ptr<Player >>& players, int playerCount) :camera_(camera), players_(players),
playerCount_(playerCount), minPos_({0.0f,0.0f}), maxPos_({1600.0f, 1000.0f}), minScale_({-800.0f,-500.0f}), maxScale_({800.0f,500.0f})
{
	LoadDivGraph("Src/Img/Explosion.png",11, 11, 1, 32, 31, bombImg_);
	LoadDivGraph("Src/Img/BigExplosion.png",8, 8, 1, 32, 32, bigBombImg_);
	ExplosionSound_ = LoadSoundMem("Src/Sound/Explosion.mp3");
	dangerZone_ = std::make_unique<DangerZoneSmaller>(maxScale_, minScale_);
	//bombImg_ = LoadGraph("Src/Img/Explosion.png");
}
OutSide::~OutSide()
{
}

void OutSide::Update()
{
	auto target = camera_.GetTargetPos();
	minPos_ = target + minScale_;
	maxPos_ = target + maxScale_;

	dangerZone_->Update();
	IsDead();
	TestSmaller();
}

void OutSide::ExplosionUpdate()
{
}

void OutSide::Draw(Vector2DFloat offset)
{
	auto camera= camera_.GetPos();
	auto min = (minPos_ + camera);
	auto max = (maxPos_ + camera);
	DrawBoxAA( min.x, min.y, max.x, max.y, 0x00ffaa, false);


	DrawFormatStringF(0.0f, 430.0f, 0xffffff, "upperPos_:x %f, y %f,", upperPos_.x, upperPos_.y);
	DrawFormatStringF(0.0f, 450.0f, 0xffffff, "lowerPos_:x %f, y %f,", lowerPos_.x, lowerPos_.y);
	//枠の座標の最大値
	DrawFormatStringF(0.0f, 480.0f, 0xffffff, "maxPos_:x %f, y %f,", maxPos_.x, maxPos_.y);
	//枠の座標の最小値
	DrawFormatStringF(0.0f, 510.0f, 0xffffff, "minPos_:x %f, y %f,", minPos_.x, minPos_.y);
	//実際の枠

	if (isExploding_)
	{
		//画面左上になったら右に行く
		if (upperPos_.y < 0.0f && upperPos_.x <= 0.0f)
		{
			upperVec_ = { 20.0f,0.0f };
		}
		//画面右上になったら下に行く
		if (upperPos_.x>= screenSize.x && upperPos_.y <=0.0f)
		{
			upperVec_ = { 0.0f,20.0f };
		}
		//画面右下になったら左に行く
		if (upperPos_.y >= screenSize.y && upperPos_.x >= screenSize.x)
		{
			upperVec_ = {-20.0f ,0.0f};
		}
		//画面左下に行ったら上に行く
		if (upperPos_.x <= 0.0f && upperPos_.y >= screenSize.y)
		{
			upperVec_ = { 0.0f ,-20.0f };
		}
		upperPos_ += upperVec_;
		
		//lowerは左回り
		//画面右上だったら左に行く
		if (lowerPos_.y <= 0.0f&& lowerPos_.x >= screenSize.x)
		{
			lowerVec_ = { -20.0f ,0.0f };
		}
		//画面右下なら上に行く
		if ( lowerPos_.x >= screenSize .x&& lowerPos_.y>= screenSize.y)
		{
			lowerVec_ = { 0.0f,-20.0f };
		}
		//画面左下なら右に行く
		if (  lowerPos_.y>= screenSize.y&& lowerPos_.x <=0.0f)
		{
			lowerVec_ = { 20.0f ,0.0f };
		}
		//画面左上なら下に行く
		if (lowerPos_.y <= 0.0f && lowerPos_.x <= 0.0f)
		{
			lowerVec_ = { 0.0f ,20.0f };
		}
		lowerPos_ += lowerVec_;

		//一定時間ごとに爆発させる、あと音も出す
		if (frame_ % 4 == 0)
		{
			bombs_.emplace_back(upperPos_);
			bombs_.emplace_back(lowerPos_);
			//PlaySoundMem(ExplosionSound_, DX_PLAYTYPE_BACK, true);
		}	

		//上からの爆弾と下からの爆弾が重なったらどっちも消す
		if(upperPos_ ==lowerPos_)
		{			
			//StartJoypadVibration(padNum_, 1000, 400);
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
			16.0f, 16.0f,
			3.5, 0.0,
			bombImg_[frame_ / 2],
			true, 0, 0);

		DrawCircleAA(upperPos_.x, upperPos_.y, 10.0f, 20.0f, 0xffaaaa);

		DrawRotaGraph2F(lowerPos_.x, lowerPos_.y,
			16.0f, 16.0f,
			3.5, 0.0,
			bombImg_[frame_ / 2],
			true, 0, 0);
		DrawCircleAA(lowerPos_.x, lowerPos_.y, 10.0f, 20.0f, 0xffaaaa);

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
			if (frame_>240)
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
	if (bigFrame_ > 100)
	{
		bigExploding_ =false;
		upperPos_ = { 0.0f,0.0f };
		lowerPos_ = { 0.0f,0.0f };
		bigFrame_ = 0;

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
				padNum_ = player->padNum_;
				isExploding_ = true;
				frame_ = 0;
				playerCount_--;
				//StartJoypadVibration(padNum_, 400, 300);
			}
		}
	}
}

bool OutSide::IsOutSide(Vector2DFloat pos)
{
	return (minPos_.x < pos.x && pos.y> minPos_.y&&
		maxPos_.x>pos.x && maxPos_.y>pos.y);
}

void OutSide::UpDownORLeftRight(Vector2DFloat pos)
{
	if (pos.y < minPos_.y || maxPos_.y < pos.y)
	{
		UpORDown(pos);
	}
	else if (minPos_.x > pos.x || maxPos_.x<pos.x)
	{
		LeftOrRight(pos);
	}
}

void OutSide::UpORDown(Vector2DFloat pos)
{
	Vector2DFloat up;
	Vector2DFloat low;

	if (pos.y <minPos_.y )
	{
		up = { pos.x ,0.0f };
		upperVec_ = { 20.0f,0.0f };
		low = { pos.x, 0.0f };
		lowerVec_ = { -20.0f ,0.0f };
	}
	else if(maxPos_.y < pos.y)
	{
		up = { pos.x ,1000.0f };
		upperVec_ = { -20.0f,0.0f };
		low = { pos.x, 1000.0f };
		lowerVec_ = { 20.0f ,0.0f };
	}
	upperPos_ = up;
	lowerPos_ = low;
}

void OutSide::LeftOrRight(Vector2DFloat pos)
{
	Vector2DFloat up;
	Vector2DFloat low;
	if (pos.x > maxPos_.x)
	{
		up = { screenSize.x ,pos.y };
		upperVec_ = { 0.0f,20.0f };
		low = { screenSize.x, pos.y };
		lowerVec_ = { 0.0f ,-20.0f };
	}
	else if (minPos_.x > pos.x)
	{
		up = { 0.0f ,pos.y };
		upperVec_ = { 0.0f,-20.0f };
		low = {0.0f, pos.y };
		lowerVec_ = { 0.0f ,20.0f };
	}
	upperPos_ = up;
	lowerPos_ = low;
}

void OutSide::TestSmaller()
{
	if (CheckHitKey(KEY_INPUT_U))
	{
		dangerZone_->Activated();
	}
}

const int OutSide::NumberOfSurvivors()
{
	return playerCount_;
}
