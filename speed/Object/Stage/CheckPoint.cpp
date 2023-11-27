#include "CheckPoint.h"
#include "../../Player/Player.h"

CheckPoint::CheckPoint(std::vector<std::shared_ptr<Player>>& players):players_(players)
{
	checkPoints_.push_back({ 0.0f,1600.0f });
	checkPoints_.push_back({ 2433.0f,1700.0f });
	checkPoints_.push_back({ 3739.0f,157.0f });
	checkPoints_.push_back({ 690.0f,153.0f });


	checkPointColList_.push_back(Collision(vec{ 0.0f,1000.0f }, vec{ 800.0f,1500.0f }));
	checkPointColList_.push_back(Collision(vec{ 2172.0f,520.0f }, vec{ 2454.0f,1600.0f }));
	checkPointColList_.push_back(Collision(vec{ 3566.0f,0.0f }, vec{ 4014,155.0f }));
	checkPointColList_.push_back(Collision(vec{ 0.0f,0.0f }, vec{ 864.0f,155.0f }));



	currentPoint_ = 0;
}

CheckPoint::~CheckPoint()
{
}

void CheckPoint::Update()
{
	for (const auto& player : players_)
	{
		if (rayCast_.CheckCollision(checkPointColList_[currentPoint_], player->pos_))
		{
			if (currentPoint_ < 3)
			{
				currentPoint_++;
			}
			else
			{
				currentPoint_=0;
			}
		}
		//IsCheckPoint(player->pos_);
	}

}

void CheckPoint::Draw(Vector2DFloat pos)

{
	for (const auto& player : players_)
	{
		if (rayCast_.CheckCollision(checkPointColList_[currentPoint_], player->pos_))
		{
			DrawStringF(0.0f, 290.0f, "チェックポイントをに接触",0xff0000);
		}
	}

	DrawBoxAA(checkPointColList_[currentPoint_].first.x+ pos.x, checkPointColList_[currentPoint_].first.y + pos.y,
		checkPointColList_[currentPoint_].second.x+ pos.x, checkPointColList_[currentPoint_].second.y + pos.y, 0xff0000,false,5.0f);
	DrawFormatString(0, 300, 0xffffff, "現在のチェックポイントのナンバー:%d", currentPoint_);
	DrawFormatString(0, 320, 0xffffff, "現在のチェックポイント:%f,%f", checkPoints_[currentPoint_].x, checkPoints_[currentPoint_].y);
	DrawCircle(checkPoints_[currentPoint_].x + pos.x, checkPoints_[currentPoint_].y + pos.y,20, 0xff0000, true);

}

bool CheckPoint::IsCheckPoint(Vector2DFloat player)
{
	//if ()
	//{
	//	return false;
	//}
	return true;
}

Vector2DFloat CheckPoint::GetCheckPoint() const
{
	return checkPoints_[currentPoint_];
}
