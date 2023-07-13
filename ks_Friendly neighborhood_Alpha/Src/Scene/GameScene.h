#pragma once
#include "SceneBase.h"
class SceneManager;
class Controller;
class Stage;
class SkyDome;
class Player;
class Grid;
class SwingPoint;


class GameScene 
	: public SceneBase
{

public:

	GameScene(void);
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void DrawDebug(void);
	void Release(void) override;
	void Load(void);
private:
	//�X�e�[�W
	std::unique_ptr<Stage> stage_;

	//�O���b�h��
	std::unique_ptr<Grid> grid_;
	bool isLoaded_;


	std::unique_ptr<SwingPoint> swi_;

	bool isLoaded ;
	//�w�i
	std::unique_ptr<SkyDome> skyDome_;

	//�v���C���[
	Player* player_;

};
