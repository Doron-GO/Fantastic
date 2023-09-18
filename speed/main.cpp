#include<DxLib.h>
#include"Scene/SceneMng.h"
#include"Scene/GameScene.h"
#include"Input/Input.h"
using namespace std;



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	SetDrawScreen(DX_SCREEN_BACK);
	SceneMng sceneManager;
	sceneManager.ChangeScene(make_shared<GameScene>(sceneManager));
	Input input;



	while (ProcessMessage() != -1)
	{
		ClearDrawScreen();
		input.Update();
		sceneManager.Update(input);
		sceneManager.Draw();
		ScreenFlip();
	}
	DxLib_End();
	return 0;
}



