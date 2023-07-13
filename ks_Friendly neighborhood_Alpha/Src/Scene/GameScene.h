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
	//ステージ
	std::unique_ptr<Stage> stage_;

	//グリッド線
	std::unique_ptr<Grid> grid_;
	bool isLoaded_;


	std::unique_ptr<SwingPoint> swi_;

	bool isLoaded ;
	//背景
	std::unique_ptr<SkyDome> skyDome_;

	//プレイヤー
	Player* player_;

};
