#include "TitleScene.h"
#include "GameScene.h"
#include "SceneMng.h"
#include "ResultScene.h"
#include"Transition/TileTransitor.h"

TitleScene::TitleScene(SceneMng& manager, int n, Transitor& transit) :Scene(manager,n,transit), num_(2)
{
	sceneTransitor_.Start();
	titleImg_ = LoadGraph("Src/Img/Title.png");

}

TitleScene::~TitleScene()
{
}

void TitleScene::Update(Input& input)
{

	if (input.IsTriggerd("item"))
	{
		num_ = 3;
	}
	if (input.IsTriggerd("hook"))
	{
		num_ = 2;
	}

	if (input.IsTriggerd("jump"))
	{
		sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_,num_, sceneTransitor_));
		return;
	}
	sceneTransitor_.Update();

}

void TitleScene::Draw()
{	
	ClearDrawScreen();

	DrawGraph(0, 0, titleImg_, true);
	DrawFormatString(0, 200, 0xffffff, "%d",num_);

	sceneTransitor_.Draw();

}

