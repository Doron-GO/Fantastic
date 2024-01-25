#include "TitleScene.h"
#include "GameScene.h"
#include "SceneMng.h"
#include "ResultScene.h"
#include"Transition/TileTransitor.h"

TitleScene::TitleScene(SceneMng& manager, int n, Transitor& transit) :Scene(manager,n,transit),
num_(1),startFlag_(false)
{
	sceneTransitor_.Start();
	titleImg_ = LoadGraph("Src/Img/Title.png");
	LoadDivGraph("Src/Img/Select.png", 4, 1, 4, 266, 36,selectImg_);
	soundH_[0] = LoadSoundMem("Src/Sound/カーソル移動5.mp3");
	soundH_[1] = LoadSoundMem("Src/Sound/決定ボタンを押す33.mp3");

}

TitleScene::~TitleScene()
{
}

void TitleScene::Update(Input& input)
{	

	if (startFlag_)
	{
		if (input.IsTriggerd("down"))
		{
			if (num_ < GetJoypadNum())
			{
				num_++;
				PlaySoundMem(soundH_[0], DX_PLAYTYPE_BACK);
			}
		}
		if (input.IsTriggerd("up"))
		{
			PlaySoundMem(soundH_[0], DX_PLAYTYPE_BACK);
			if (num_ > 1)
			{
				num_--;
			}
		}
		if (input.IsTriggerd("jump"))
		{
			if (startFlag_)
			{
				PlaySoundMem(soundH_[1], DX_PLAYTYPE_BACK);
				sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_, num_, sceneTransitor_));
				return;
			}
		}
	}	
	if (!startFlag_)
	{
		if (input.IsTriggerd("jump"))
		{
			startFlag_ = true;
			PlaySoundMem(soundH_[1], DX_PLAYTYPE_BACK);

		}
	}

	sceneTransitor_.Update();
}

void TitleScene::Draw()
{	

	ClearDrawScreen();
	if (!startFlag_)
	{
		DrawGraph(0, 0, titleImg_, true);
	}

	DrawFormatString(0, 200, 0xffffff, "%d",num_);
	//DrawRotaGraph2F(800.0f, 560.0f, 208.0f, 20.0f, 1.0, 0.0, restertImg_, true);
	if (startFlag_)
	{
		for (int num=0;num<GetJoypadNum();num++)
		{

			if ((num+1 == num_))
			{						
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
			}
			else
			{			
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60);
			}
			DrawRotaGraph2F(800.0f, 400.0f+((screenSize_.y/6))*num, 133.0f, 20.0f, 3.0, 0.0, selectImg_[num], true);	
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	sceneTransitor_.Draw();

}

