#include<DxLib.h>
#include "GameScene.h"
#include "TitleScene.h"
#include "SceneMng.h"
#include"../Object/Manager/ImageMng.h"
#include"Transition/TileTransitor.h"
#include"../Object/Time/TimeCount.h"
#include"../Object/Time/DeltaTime.h"


GameScene::GameScene(SceneMng& manager, int n, Transitor& transit):Scene(manager, n, transit), playerNum_(n),_update(&GameScene::MultiPlayUpdate),num_(0)
{

	sceneTransitor_.Start();
	camera_ = std::make_unique<Camera>();	
	stage_ = std::make_unique<Stage>();
	outSide_ = std::make_unique<OutSide>(*camera_, playerNum_);

	playerManager_ = std::make_unique<PlayerManager>(outSide_->conclusion_, *stage_->GetBlocks());
	playerManager_->Init(playerNum_, stage_->GetColList(), stage_->GetWallColList(), stage_->GetWireColList());
	//camera_->Init(stage_->GetWorldArea() * stage_-> GetTileSize());//カメラを初期化
	stage_->Init(playerManager_->GetPlayers());
	checkPoint_ = std::make_unique<CheckPoint>(playerManager_->GetPlayers(), stage_->CheckPointGetColList());
	timeCount_ = std::make_unique<TimeCount>(*checkPoint_);
	camera_->ReConnect(playerManager_->GetPlayers()[(int)playerManager_->GetNewLeadNum()]);
	if (playerNum_ ==1)
	{
		outSide_->SinglePlay();
		playerManager_->SinglePlay();
		checkPoint_->SetSingleMode();
		_update = &GameScene::SinglePlayUpdate;
		timeCount_->SinglePlay();
	}
	GoImg_ = LoadGraph("Src/Img/Go.png");
	ReadyImg_ = LoadGraph("Src/Img/Ready.png");
	sound_[0] = LoadSoundMem("Src/Sound/ready.mp4");
	sound_[1] = LoadSoundMem("Src/Sound/「ゴー」.mp3");
	sound_[2] = LoadSoundMem("Src/Sound/カーソル移動5.mp3");
	sound_[3] = LoadSoundMem("Src/Sound/決定ボタンを押す33.mp3");

	LoadDivGraph("Src/Img/Select2.png", 2, 1, 2, 420, 40, selectImg_);

	camera_->Update();

	deltaTime.Reset();
	startTime_ = deltaTime.GetElapsedTime();

}

GameScene::~GameScene()
{
}

void GameScene::Update(Input& input)
{
	deltaTime.update();
	auto elapsed = deltaTime.GetElapsedTime();
	(this->*_update)(input, elapsed);
	if (outSide_->conclusion_ || checkPoint_->IsGoal())
	{
		if (input.IsTriggerd("down"))
		{
			if (num_ < 1)
			{
				num_++;
				PlaySoundMem(sound_[2], DX_PLAYTYPE_BACK);
			}
		}
		if (input.IsTriggerd("up"))
		{
			if (num_ > 0)

			{
				num_--;		
				PlaySoundMem(sound_[2], DX_PLAYTYPE_BACK);

			}
		}
		if (input.IsTriggerd("jump"))
		{
			if(num_ == 0)
			{
				PlaySoundMem(sound_[3], DX_PLAYTYPE_BACK);
				sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_, playerNum_, sceneTransitor_));
				return;
			}			
			else	{
				PlaySoundMem(sound_[3], DX_PLAYTYPE_BACK);
				sceneManager_.ChangeScene(std::make_shared<TitleScene>(sceneManager_, 0, sceneTransitor_));
				return;
			}

		}
	}
	sceneTransitor_.Update();


}

void GameScene::Draw()
{

	ClearDrawScreen();
	stage_->Draw(camera_->GetPos());
	outSide_->Draw(camera_->GetPos());
	playerManager_->Draw(camera_->GetPos());
	checkPoint_->Draw(camera_->GetPos());
	auto newLeder = playerManager_->GetNewLeadNum();
	auto Last = playerManager_->GetLastLeadNum();
	timeCount_->Draw();
	auto elapsed = deltaTime.GetElapsedTime();

	if (!(startTime_ +0.6f> elapsed)&& elapsed <= startTime_ + 1.5f)
	{
		if (!CheckSoundMem(sound_[0]))
		{
			PlaySoundMem(sound_[0], DX_PLAYTYPE_BACK, true);
		}

		DrawRotaGraph2F(screenSize_.x / 2.0f,0.0f+ (screenSize_.y / 3.0f) - elapsed * 40,
			288.0f,33.0f, 2.0f, 0.0f, ReadyImg_, true);

	}

	if (!(startTime_ + 1.5f > elapsed) && elapsed <= startTime_ + 2.0f)
	{
		if (!CheckSoundMem(sound_[1]))
		{
			PlaySoundMem(sound_[1], DX_PLAYTYPE_BACK, true);
		}
		DrawRotaGraph2F(screenSize_.x / 2.0f, 0.0f+(screenSize_.y / 3.0f) ,
			225.0f, 80.0f, 1.0f, 0.0f, GoImg_, true);
	}

	if (outSide_->conclusion_|| checkPoint_->IsGoal())
	{
		for (int num = 0; num < 2; num++)
		{

			if ((num  == num_))
			{
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			else
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60);
			}
			DrawRotaGraph2F(800.0f, 800.0f + ((screenSize_.y / 10)) * num, 210.0f, 40.0f, 2.0, 0.0, selectImg_[num], true);
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	sceneTransitor_.Draw();
}

void GameScene::DecideOnTheBeginning()
{
	playerManager_->DecideOnTheBeginning2(checkPoint_->GetCheckPoint2());
	auto newLeder= playerManager_->GetNewLeadNum();
	auto oldLeder =playerManager_->GetOldLeadNum();
	//前のフレームの先頭プレイヤーと今の先頭プレイヤーが違っていたら、
	//カメラ追従対象を変更する。
	if (oldLeder != newLeder)
	{
		camera_->ReConnect(playerManager_->GetPlayers()[(int)newLeder]);
		camera_->PhaseChanging((int)newLeder);
		outSide_->PhaseChanging();
	}
	playerManager_->SetOld();
}

void GameScene::MultiPlayUpdate(Input& input, float elapsedTime)
{
	camera_->Update();
	outSide_->Update(playerManager_->GetPlayers());
	stage_->Update();
	if (elapsedTime >= startTime_+2.0f)
	{		
		timeCount_->Update(elapsedTime);
		checkPoint_->Update();
		playerManager_->Update(input);
		DecideOnTheBeginning();
	}
}

void GameScene::SinglePlayUpdate(Input& input, float elapsedTime)
{
	camera_->Update();
	outSide_->Update(playerManager_->GetPlayers());
	stage_->Update();
	if (elapsedTime >= startTime_+ 2.0f)
	{		
		timeCount_->Update(elapsedTime);
		if (checkPoint_->IsGoal())
		{
			playerManager_->Goal();
		}
		checkPoint_->Update();
		playerManager_->Update(input);
		DecideOnTheBeginning();	
	}
}

