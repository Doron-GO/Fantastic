#include<DxLib.h>
#include "OutSide.h"
#include"Camera.h"
#include"../../Player/Player.h"
#include"DangerZoneSmaller.h"

Vector2DFloat screenSize ={1600.0f,1000.0f};

OutSide::OutSide(Camera& camera, int playerCount) :camera_(camera), playerCount_(playerCount),
minPos_({0.0f,0.0f}), maxPos_({1600.0f, 1000.0f}), minScale_({-800.0f,-500.0f}),
maxScale_({800.0f,500.0f}),conclusion_(false)
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
	minPos_ = outsidePos_ + minScale_;
	maxPos_ = outsidePos_ + maxScale_;

	if (!singlePlayFlag_)
	{
		dangerZone_->Update();
		IsDead(players);

	}
	if (isExploding_)
	{
		//画面左上になったら右に行く
		if (upperPos_.y < minPos_.y && upperPos_.x <= minPos_.x)
		{
			upperVec_ = { 20.0f,0.0f };
		}
		//画面右上になったら下に行く
		if (upperPos_.x >= maxPos_.x && upperPos_.y <= minPos_.y)
		{
			upperVec_ = { 0.0f,20.0f };
		}
		//画面右下になったら左に行く
		if (upperPos_.y >= maxPos_.y && upperPos_.x >= maxPos_.x)
		{
			upperVec_ = { -20.0f ,0.0f };
		}
		//画面左下に行ったら上に行く
		if (upperPos_.x <= minPos_.x && upperPos_.y >= maxPos_.y)
		{
			upperVec_ = { 0.0f ,-20.0f };
		}
		upperPos_ += upperVec_;

		//lowerは左回り
		//画面右上だったら左に行く
		if (lowerPos_.y <= minPos_.y && lowerPos_.x >= maxPos_.x)
		{
			lowerVec_ = { -20.0f ,0.0f };
		}
		//画面右下なら上に行く
		if (lowerPos_.x >= maxPos_.x && lowerPos_.y >= maxPos_.y)
		{
			lowerVec_ = { 0.0f,-20.0f };
		}
		//画面左下なら右に行く
		if (lowerPos_.y >= maxPos_.y && lowerPos_.x <= minPos_.x)
		{
			lowerVec_ = { 20.0f ,0.0f };
		}
		//画面左上なら下に行く
		if (lowerPos_.y <= minPos_.y && lowerPos_.x <= minPos_.x)
		{
			lowerVec_ = { 0.0f ,20.0f };
		}
		lowerPos_ += lowerVec_;

		//一定時間ごとに爆発させる、あと音も出す
		if ((frame_) % 3 == 0)
		{
			bombs_.emplace_back(upperPos_);
			bombs_.emplace_back(lowerPos_);
			PlaySoundMem(ExplosionSound_, DX_PLAYTYPE_BACK, true);
		}
		//上からの爆弾と下からの爆弾が重なったらどっちも消す
		if (lowerPos_.distance(upperPos_)<40)
		{
			PlaySoundMem(ExplosionSound_, DX_PLAYTYPE_BACK, true);

			StartJoypadVibration(padNum_, 1000, 400);
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
	DrawBoxAA( min.x, min.y, max.x, max.y, 0xff0000, false);

	if (isExploding_)
	{
		DrawRotaGraph2F(upperPos_.x+camera.x, upperPos_.y+camera.y,
			16.0f, 16.0f,
			5.0, 0.0,
			bigBombImg_[frame_ % 6],
			true, 0, 0);

		DrawRotaGraph2F(lowerPos_.x + camera.x, lowerPos_.y + camera.y,
			16.0f, 16.0f,
			5.0, 0.0,
			bigBombImg_[frame_%6],
			true, 0, 0);
		DrawCircleAA(lowerPos_.x, lowerPos_.y, 10.0f, 20.0f, 0xff0000);
		DrawCircleAA(upperPos_.x, upperPos_.y, 10.0f, 20.0f, 0x0000ff);

		for (auto& b : bombs_)
		{
			DrawRotaGraph2F(b.pos_.x + camera.x, b.pos_.y + camera.y,
				18.0, 16.0f,
				4.5, 0.0,
				bombImg_[b.frame_%11],
				true, 0, 0);
		}
	}
	if (bigExploding_)
	{
		DrawRotaGraph2F(lowerPos_.x + camera.x, lowerPos_.y + camera.y,
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

	DrawBoxAA(0.0f ,0.0f,screenSize.x, min.y, 0x000000, true);
	DrawBoxAA(0.0f, screenSize.y, screenSize.x, max.y, 0x000000, true);
	DrawBoxAA(0.0f, 0.0f, min.x, screenSize.y, 0x000000, true);
	DrawBoxAA(screenSize.x, 0.0f, max.x, screenSize.y, 0x000000, true);

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

void OutSide::SinglePlay()
{
	singlePlayFlag_=true;
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
	upperPos_ = up;
	lowerPos_ = low;
}

void OutSide::LeftOrRight(Vector2DFloat pos)
{
	Vector2DFloat up;
	Vector2DFloat low;

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
	upperPos_ = up ;
	lowerPos_ = low ;
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

