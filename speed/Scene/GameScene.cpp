#include<DxLib.h>
#include "GameScene.h"

GameScene::GameScene(SceneMng& manager):Scene(manager)
{
	player_ = std::make_unique<Player>();
}

void GameScene::Update(Input& input)
{
	player_->Update(input);
}

void GameScene::Draw()
{
	DrawString(50, 50, "GameScene", 0xffffff);

	player_->Draw();
}
