#include "TitleScene.h"
#include "GameScene.h"
#include "SceneMng.h"
#include "ResultScene.h"

TitleScene::TitleScene(SceneMng& manager, int n) :Scene(manager,n), num_(2)
{
	titleImg_ = LoadGraph("Src/Img/Title.png");
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
		sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_,num_));
	}
}

void TitleScene::Draw()
{	DrawGraph(0, 0, titleImg_, true);
	DrawFormatString(0, 200, 0xffffff, "%d",num_);
}

void TitleScene::DrawOwnScreen(void)
{



}
