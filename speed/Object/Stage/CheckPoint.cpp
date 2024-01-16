#include "CheckPoint.h"
#include "../../Player/Player.h"

CheckPoint::CheckPoint(std::vector<std::shared_ptr<Player>> players, PointColList checkpoint)
	:players_(players), checkPointColList2_(checkpoint)
{
	// x9800
	// y0
	// x0
	//y 3000
	// x9800
	//y 3000
	//x0
	//y3000

	////一番右
	//checkPoints_.push_back({ 9800.0f,0.0f });
	////一番上
	//checkPoints_.push_back({ 0.0f,0.0f });
	////一番左
	//checkPoints_.push_back({ 0.0f,0.0f });
	////一番下
	//checkPoints_.push_back({ 0.0f,3000.0f });
	////一番右
	//checkPoints_.push_back({ 9800.0f,0.0f });
	////一番下
	//checkPoints_.push_back({ 0.0f,3000.0f });
	////一番左
	//checkPoints_.push_back({ 0.0f,0.0f });
	////一番下
	//checkPoints_.push_back({ 0.0f,3000.0f });

	//一番右
	checkPoints3_.push_back(CHECKPOINT{ false,{9800.0f,0.0f} });
	//一番↑
	checkPoints3_.push_back(CHECKPOINT{ true,{0.0f,0.0f} });
	//一番左
	checkPoints3_.push_back(CHECKPOINT{ false,{0.0f,0.0f} });
	//一番下
	checkPoints3_.push_back(CHECKPOINT{ true,{0.0f,3000.0f} });
	//一番右
	checkPoints3_.push_back(CHECKPOINT{ false,{9800.0f,0.0f} });
	//一番下
	checkPoints3_.push_back(CHECKPOINT{ true,{0.0f,3000.0f} });
	//一番左
	checkPoints3_.push_back(CHECKPOINT{ false,{0.0f,0.0f} });
	//一番下
	checkPoints3_.push_back(CHECKPOINT{ true,{0.0f,3000.0f} });


	currentPoint_ = 0;
}

CheckPoint::~CheckPoint()
{
}

void CheckPoint::Update()
{
	for (const auto& player : players_)
	{
		if (player->IsAlive())
		{
			if (rayCast_.RectToRectCollision(checkPointColList2_[currentPoint_].first, checkPointColList2_[currentPoint_].second,
			player->col_.min_, player->col_.max_))

				{
					if (currentPoint_ < 6)
					{
						currentPoint_++;
					}
					else
					{
						currentPoint_ = 0;
					}
					break;
				}
		}
	}
}

void CheckPoint::Draw(Vector2DFloat pos)

{
	for (const auto& player : players_)
	{
		if (player->IsAlive())
		{
			if (rayCast_.CheckCollision(checkPointColList2_[currentPoint_], player->pos_))
			{
				DrawStringF(0.0f, 290.0f, "チェックポイントをに接触", 0xff0000);
			}
		}
	}
	DrawBoxAA(checkPointColList2_[currentPoint_].first.x+ pos.x, checkPointColList2_[currentPoint_].first.y + pos.y,
		checkPointColList2_[currentPoint_].second.x+ pos.x, checkPointColList2_[currentPoint_].second.y + pos.y, 0xff0000,false,5.0f);
	DrawFormatString(0, 300, 0xffffff, "現在のチェックポイントのナンバー:%d", currentPoint_);
	//DrawFormatString(0, 320, 0xffffff, "現在のチェックポイント:%f,%f", checkPoints_[currentPoint_].x, checkPoints_[currentPoint_].y);
//	DrawCircle(checkPoints_[currentPoint_].x + pos.x, checkPoints_[currentPoint_].y + pos.y,20, 0xff0000, true);
}

Vector2DFloat CheckPoint::GetCheckPoint() const
{
	return checkPoints_[currentPoint_];
}

CHECKPOINT CheckPoint::GetCheckPoint2() const
{
	return checkPoints3_[currentPoint_];
}
