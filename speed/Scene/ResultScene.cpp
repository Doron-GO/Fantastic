#include "ResultScene.h"
#include"Transition/TileTransitor.h"

ResultScene::ResultScene(SceneMng& manager, int n, Transitor& transit) :Scene(manager,n,  transit)
{
	sceneTransitor_.Start();
	titleImg_ = LoadGraph("Src/Img/Title.png");

}

void ResultScene::Update(Input& input)
{sceneTransitor_.Update();
}	


void ResultScene::Draw()
{
	ClearDrawScreen();

	DrawGraph(0, 0, titleImg_, true);

	sceneTransitor_.Draw();



}
