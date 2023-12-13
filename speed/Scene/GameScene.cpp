#include<DxLib.h>
#include "GameScene.h"
#include"../Object/Manager/ImageMng.h"


GameScene::GameScene(SceneMng& manager):Scene(manager)
{
	SetDrawScreen(DX_SCREEN_BACK);
	screenID_ = MakeScreen(1600.0f, 1000.0f, true);
	camera_ = std::make_unique<Camera>();	
	stage_ = std::make_unique<Stage>(players_);
	for (int playerNum = 1; playerNum <= GetJoypadNum(); playerNum++)
	{
		std::shared_ptr<Player> player;
		player = std::make_shared<Player>(playerNum);
		player->Init(stage_->GetColList(),stage_->GetWallColList(),
						stage_->GetWireColList());
		players_.push_back(player);
	}
	stage_->Init();
	checkPoint_ = std::make_unique<CheckPoint>(players_);
	new_LeadNum_ = PLAYER_NUM::P_1;
	old_LeadNum_ = PLAYER_NUM::P_1;
	last_Num_ = PLAYER_NUM::P_1;
	camera_->ReConnect(players_[0]);
	camera_->Init(stage_->GetWorldArea() * stage_-> GetTileSize());//�J������������
	outSide_ = std::make_unique<OutSide>(*camera_, players_, GetJoypadNum());
}

void GameScene::Update(Input& input)
{
	camera_->Update();
	for (const auto& player : players_)
	{
		if (player->IsAlive())
		{
			player->Update(input);
		}	
	}
	outSide_->Update();
	checkPoint_->Update();
	DecideOnTheBeginning();
	DrawOwnScreen();
}

void GameScene::Draw()
{
}

void GameScene::DecideOnTheBeginning()
{
	testDistance_.clear();
	//�v���C���[�ƃ`�F�b�N�|�C���g�Ƃ̋������i�[���Ă���B
	for (auto& p : players_)
	{
		iD_.first = (p->padNum_) - 1;
		iD_.second = players_[(p->padNum_) - 1]->GetPos().distance(checkPoint_->GetCheckPoint());
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
				//�v���C���[N���v���C���[N+1���O��������A�v���C���[N��擪�ɂ���B 
				if (testDistance_[num1].second < testDistance_[num2].second)
				{
					if (testDistance_[num1].second < testDistance_[static_cast<int>(new_LeadNum_)].second)
					{
						new_LeadNum_ = static_cast<PLAYER_NUM>(testDistance_[num1].first);
					}
				}
				else
				{//�Ō���̃v���C���[���E�����Ă�����Ƃ肠�����ʂ̃v���C���[���Ō�������ɂ���
					if (!(players_[static_cast<int>(last_Num_)]->IsAlive()))
					{
						last_Num_ = static_cast<PLAYER_NUM>(testDistance_[num1].first);
					}
					//�O�Ō���̃v���C���[����낾������
					if (testDistance_[static_cast<int>(last_Num_)].second <
						testDistance_[static_cast<int>(num1)].second)
					{
						last_Num_ = static_cast<PLAYER_NUM>(testDistance_[num1].first);
					}
				}
			}
		}
	}
	//�O�̃t���[���̐擪�v���C���[�ƍ��̐擪�v���C���[������Ă�����A
	//�J�����Ǐ]�Ώۂ�ύX����B
	if (old_LeadNum_ != new_LeadNum_)
	{
		camera_->ReConnect(players_[static_cast<int>(new_LeadNum_)]);
		camera_->PhaseChanging(static_cast<int>(new_LeadNum_));
	}
	old_LeadNum_ = new_LeadNum_;
}

void GameScene::DrawOwnScreen()
{
	stage_->Draw(camera_->GetPos());
	for (const auto& player : players_)
	{
		if (player->IsAlive())
		{
			player->Draw(camera_->GetPos());
		}
	}
	outSide_->Draw(camera_->GetPos());
	checkPoint_->Draw(camera_->GetPos());
	DrawFormatStringF(0, 140, 0xffffff, "camera:%f,%f", camera_->GetPos().x, camera_->GetPos().y);
	DrawFormatStringF(0, 580, 0xffffff, "�Ō����:%d", static_cast<int>(last_Num_));
	DrawFormatStringF(0, 560, 0xffffff, "�擪��:%d", static_cast<int>(new_LeadNum_));
}
