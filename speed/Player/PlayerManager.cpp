#include "PlayerManager.h"
#include "Player.h"

PlayerManager::PlayerManager():new_LeadNum_(PLAYER_NUM::P_1), old_LeadNum_(PLAYER_NUM::P_1), last_Num_(PLAYER_NUM::P_1)
{
}

PlayerManager::~PlayerManager()
{
}


void PlayerManager::Init(int playerNum, ColList gruound, ColList Wall, ColList wire)
{
	for (int Num = 1; Num <= playerNum; Num++)
	{
		std::shared_ptr<Player> player;
		player = std::make_shared<Player>(Num);
		player->Init(gruound, Wall, wire);
		players_.push_back(player);
	}

}

void PlayerManager::Update(Input& input)
{
	for (const auto& player : players_)
	{
		if (player->IsAlive())
		{
			player->Update(input);
		}
	}
}

void PlayerManager::Draw(Vector2DFloat cameraPos)
{
	for (const auto& player : players_)
	{
		if (player->IsAlive())
		{
			player->Draw(cameraPos);
		}
	}
}

const Vector2DFloat& PlayerManager::GetPlayerPos(int playerNum)
{
	return players_[playerNum]->pos_;
}

const Players PlayerManager::GetPlayers()
{
	return players_;
}

void PlayerManager::DecideOnTheBeginning(Vector2DFloat checkPoint)
{
	testDistance_.clear();
	//プレイヤーとチェックポイントとの距離を格納している。
	for (auto& p : players_)
	{
		iD_.first = (p->padNum_) - 1;
		iD_.second = players_[(p->padNum_) - 1]->GetPos().distance(checkPoint);
		testDistance_.push_back(iD_);
	}
	for (auto& p1 : players_)
	{
		for (auto& p2 : players_)
		{
			if (p1->IsAlive() && p2->IsAlive())
			{
				auto num1 = (p1->padNum_) - 1;
				auto num2 = (p2->padNum_) - 1;
				//プレイヤーNがプレイヤーN+1より前だったら、プレイヤーNを先頭にする。 
				if (testDistance_[num1].second < testDistance_[num2].second)
				{
					if (testDistance_[num1].second < testDistance_[(int)new_LeadNum_].second)
					{
						new_LeadNum_ = static_cast<PLAYER_NUM>(testDistance_[num1].first);
					}
				}
				else
				{//最後尾のプレイヤーが脱落していたらとりあえず別のプレイヤーを最後尾扱いにする
					if (!(players_[(int)last_Num_]->IsAlive()))
					{
						last_Num_ = static_cast<PLAYER_NUM>(testDistance_[num1].first);
					}
					//前最後尾のプレイヤーより後ろだったら
					if (testDistance_[(int)last_Num_].second <
						testDistance_[num1].second)
					{
						last_Num_ = static_cast<PLAYER_NUM>(testDistance_[num1].first);
					}
				}
			}
		}
	}

}

const PlayerManager:: PLAYER_NUM PlayerManager::GetOldLeadNum()
{
	return old_LeadNum_;
}

const PlayerManager:: PLAYER_NUM PlayerManager::GetNewLeadNum()
{
	return new_LeadNum_;
}

const PlayerManager:: PLAYER_NUM PlayerManager::GetLastLeadNum()
{
	return last_Num_;
}

void PlayerManager::SetOld()
{
	old_LeadNum_ = new_LeadNum_;

}
