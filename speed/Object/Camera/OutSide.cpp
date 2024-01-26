#include<DxLib.h>
#include "OutSide.h"
#include"Camera.h"
#include"../../Player/Player.h"
#include"DangerZoneSmaller.h"

Vector2DFloat screenSize ={1600.0f,1000.0f};

OutSide::OutSide(Camera& camera, int playerCount) :camera_(camera), playerCount_(playerCount), minPos_({0.0f,0.0f}), maxPos_({1600.0f, 1000.0f}), minScale_({-800.0f,-500.0f}), maxScale_({800.0f,500.0f}),
conclusion_(false)
{
	LoadDivGraph("Src/Img/Explosion.png",11, 11, 1, 32, 32, bombImg_);
	LoadDivGraph("Src/Img/BigExplosion.png",8, 8, 1, 32, 32, bigBombImg_);
	ExplosionSound_ = LoadSoundMem("Src/Sound/Explosion.mp3");
	dangerZone_ = std::make_unique<DangerZoneSmaller>(maxScale_, minScale_);
	_phase = &OutSide::Follow;

	//bombImg_ = LoadGraph("Src/Img/Explosion.png");
}
OutSide::~OutSide()
{
}

void OutSide::Update(std::vector<std::shared_ptr<Player>> players)
{
	(this->*_phase)();

	//auto target = camera_.GetTargetPos();
	//auto target = camera_.GetPos();
	dangerZone_->Update();
	minPos_ = outsidePos_ + minScale_;
	maxPos_ = outsidePos_ + maxScale_;
	IsDead(players);
	if (isExploding_)
	{
		//画面左上になったら右に行く
		if (upperPos_.y < 0.0f && upperPos_.x <= 0.0f)
		{
			upperVec_ = { 20.0f,0.0f };
		}
		//画面右上になったら下に行く
		if (upperPos_.x >= screenSize.x && upperPos_.y <= 0.0f)
		{
			upperVec_ = { 0.0f,20.0f };
		}
		//画面右下になったら左に行く
		if (upperPos_.y >= screenSize.y && upperPos_.x >= screenSize.x)
		{
			upperVec_ = { -20.0f ,0.0f };
		}
		//画面左下に行ったら上に行く
		if (upperPos_.x <= 0.0f && upperPos_.y >= screenSize.y)
		{
			upperVec_ = { 0.0f ,-20.0f };
		}
		upperPos_ += upperVec_;

		//lowerは左回り
		//画面右上だったら左に行く
		if (lowerPos_.y <= 0.0f && lowerPos_.x >= screenSize.x)
		{
			lowerVec_ = { -20.0f ,0.0f };
		}
		//画面右下なら上に行く
		if (lowerPos_.x >= screenSize.x && lowerPos_.y >= screenSize.y)
		{
			lowerVec_ = { 0.0f,-20.0f };
		}
		//画面左下なら右に行く
		if (lowerPos_.y >= screenSize.y && lowerPos_.x <= 0.0f)
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
		if ((frame_) % 3 == 0)
		{
			bombs_.emplace_back(upperPos_);
			bombs_.emplace_back(lowerPos_);
			//PlaySoundMem(ExplosionSound_, DX_PLAYTYPE_BACK, true);
		}
		//上からの爆弾と下からの爆弾が重なったらどっちも消す
		if (lowerPos_.distance(upperPos_)<30)
		{
			//StartJoypadVibration(padNum_, 1000, 400);
			bombs_.clear();
			isExploding_ = false;
			bigExploding_ = true;
			bigFrame_ = 0;
		}
		frame_++;
		for (auto& b : bombs_)
		{
			b.frame_++;
			if (b.frame_ > 30)
			{
				b.isDead = true;
			}
		}
		bombs_.remove_if([](const Bomb& b){
			return b.isDead;
			});
	}
	TestSmaller();
}

void OutSide::PhaseChanging()
{
	_phase = &OutSide::Switching;
}

void OutSide::Follow()
{
	outsidePos_= camera_.GetTargetPos();
	outsideOldPos_ =outsidePos_;
}

void OutSide::Switching()
{
	if (time <= 60.0f) { time++; }
	outsidePos_.x = outsideOldPos_.x * (1.0f - time / 60.0f)+ camera_.GetTargetPos().x *  time / 60.0f;
	outsidePos_.y = outsideOldPos_.y * (1.0f - time / 60.0f) + camera_.GetTargetPos().y *  time / 60.0f;
	if (time >= 60.0f)
	{
		_phase = &OutSide::Follow;
		time = 0.0f;
	}
}

void OutSide::Draw(Vector2DFloat offset)
{
	auto camera= camera_.GetPos();
	auto min = (minPos_ + camera);
	auto max = (maxPos_ + camera);
	DrawBoxAA( min.x, min.y, max.x, max.y, 0x00ffaa, false);

	//DrawFormatStringF(0.0f, 430.0f, 0xffffff, "upperPos_:x %f, y %f,", upperPos_.x, upperPos_.y);
	//DrawFormatStringF(0.0f, 450.0f, 0xffffff, "lowerPos_:x %f, y %f,", lowerPos_.x, lowerPos_.y);
	////枠の座標の最大値
	//DrawFormatStringF(0.0f, 480.0f, 0xffffff, "maxPos_:x %f, y %f,", maxPos_.x, maxPos_.y);
	////枠の座標の最小値
	//DrawFormatStringF(0.0f, 510.0f, 0xffffff, "minPos_:x %f, y %f,", minPos_.x, minPos_.y);
	//DrawFormatStringF(0.0f, 530.0f, 0xffffff, "画面が狭まるカウント%d", dangerZone_->count_);

	if (isExploding_)
	{
		DrawRotaGraph2F(upperPos_.x, upperPos_.y,
			16.0f, 16.0f,
			5.0, 0.0,
			bigBombImg_[frame_ % 6],
			true, 0, 0);

		DrawRotaGraph2F(lowerPos_.x , lowerPos_.y ,
			16.0f, 16.0f,
			5.0, 0.0,
			bigBombImg_[frame_%6],
			true, 0, 0);
		DrawCircleAA(lowerPos_.x, lowerPos_.y, 10.0f, 20.0f, 0xff0000);
		DrawCircleAA(upperPos_.x, upperPos_.y, 10.0f, 20.0f, 0x0000ff);

		for (auto& b : bombs_)
		{
			DrawRotaGraph2F(b.pos_.x, b.pos_.y,
				18.0, 16.0f,
				4.5, 0.0,
				bombImg_[b.frame_%11],
				true, 0, 0);
		}
	}
	if (bigExploding_)
	{
		DrawRotaGraph2F(lowerPos_.x, lowerPos_.y,
			16.0f, 16.0f,
			9.5, 0.0,
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

void OutSide::IsDead(std::vector< std::shared_ptr<Player >> players)
{
 	for (const auto& player : players)
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
				dangerZone_->ResetCounter();
				playerCount_--;
				StartJoypadVibration(padNum_, 400, 300);
			}
		}
	}
	if (playerCount_<=1) {
		conclusion_ = true;
	}
}

bool OutSide::IsOutSide(Vector2DFloat pos)
{
	return (minPos_.x < pos.x && pos.y> minPos_.y&&
		maxPos_.x>pos.x && maxPos_.y>pos.y);
}

void OutSide::UpDownORLeftRight(Vector2DFloat pos)
{
	if (pos.y <= minPos_.y || maxPos_.y <= pos.y)
	{
		UpORDown(pos);
	}
	if (minPos_.x >= pos.x || maxPos_.x<=pos.x)
	{
		LeftOrRight(pos);
	}
}

void OutSide::UpORDown(Vector2DFloat pos)
{
	Vector2DFloat up;
	Vector2DFloat low;
	Vector2DFloat camera{ camera_.GetPos().x,camera_.GetPos().y };

	if (pos.y <=minPos_.y )
	{
		up = { pos.x  ,minPos_.y };
		upperVec_ = { 30.0f,0.0f };
		low = { pos.x ,minPos_.y };
		lowerVec_ = { -30.0f ,0.0f };
	}
	else if(maxPos_.y <= pos.y)
	{
		up = { pos.x  , maxPos_.y };
		upperVec_ = { -30.0f,0.0f };
		low = { pos.x, maxPos_.y };
		lowerVec_ = { 30.0f ,0.0f };
	}
	upperPos_ = up+ camera;
	lowerPos_ = low+ camera;
}

void OutSide::LeftOrRight(Vector2DFloat pos)
{
	Vector2DFloat up;
	Vector2DFloat low;
	Vector2DFloat camera{ camera_.GetPos().x,camera_.GetPos().y };

	if (pos.x >= maxPos_.x)
	{
		up = { maxPos_.x ,pos.y  };
		upperVec_ = { 0.0f,20.0f };
		low = { maxPos_.x, pos.y };
		lowerVec_ = { 0.0f ,-20.0f };
	}
	else if (minPos_.x >= pos.x)
	{
		up = { minPos_.x ,pos.y };
		upperVec_ = { 0.0f,-20.0f };
		low = { minPos_.x, pos.y };
		lowerVec_ = { 0.0f ,20.0f };
	}
	upperPos_ = up + camera;
	lowerPos_ = low + camera;
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

