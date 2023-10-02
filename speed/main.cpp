#include<DxLib.h>
#include"Scene/SceneMng.h"
#include"Scene/GameScene.h"
#include"Input/Input.h"
using namespace std;
#include"_debug/_DebugConOut.h"
#include"_debug/_DebugDispOut.h"



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SetGraphMode(1200, 800, 32);			//640×480ドットの32bitカラーに設定
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)
	{
		return -1;
	}
	_dbgSetup(800, 600, 256);

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



