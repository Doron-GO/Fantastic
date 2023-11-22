#include "CheckPoint.h"
#include "../../Player/Player.h"

CheckPoint::CheckPoint(std::vector<std::shared_ptr<Player>>& players):players_(players)
{
	checkPoints_.push_back({ 1600.0f,0.0f });
	checkPoints_.push_back({ 1600.0f,0.0f });

	checkPointColList_.push_back(Collision(vec{ 0.0f,1000.0f }, vec{ 800.0f,1500.0f }));


}

CheckPoint::~CheckPoint()
{
}

void CheckPoint::Update()
{
	for (const auto& player : players_)
	{
		if (rayCast_.CheckCollision(checkPointColList_[0], player->pos_))
		{

		}
		//IsCheckPoint(player->pos_);
	}

}

void CheckPoint::Draw(Vector2DFloat pos)

{
	for (const auto& player : players_)
	{
		if (rayCast_.CheckCollision(checkPointColList_[0], player->pos_))
		{
			DrawStringF(0.0f, 290.0f, "チェックポイントをに接触",0xff0000);
		}
	}

	DrawBoxAA(checkPointColList_[0].first.x+ pos.x, checkPointColList_[0].first.y + pos.y,
		checkPointColList_[0].second.x+ pos.x, checkPointColList_[0].second.y + pos.y, 0xff0000,false,5.0f);


}

bool CheckPoint::IsCheckPoint(Vector2DFloat player)
{
	//if ()
	//{
	//	return false;
	//}
	return true;
}
