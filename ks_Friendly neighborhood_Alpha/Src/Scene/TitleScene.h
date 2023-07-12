#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"

class SceneManager;
class Grid;
class SkyDome;
class SwingPoint;

class TitleScene 
	: public SceneBase
{

public:
	TitleScene(void);
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	int count_;

	// 画像
	int imgTitle_;
	int imgPush_;
	int p;
	//グリッド線
	std::unique_ptr<Grid> grid_;
	Transform transform_;	//プレイヤーのモデル制御

	// 背景
	std::unique_ptr<SkyDome> skyDome_;
	std::unique_ptr<SwingPoint> swi_;

};
