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

	// �摜
	int imgTitle_;
	int imgPush_;
	int p;
	//�O���b�h��
	std::unique_ptr<Grid> grid_;
	Transform transform_;	//�v���C���[�̃��f������

	// �w�i
	std::unique_ptr<SkyDome> skyDome_;
	std::unique_ptr<SwingPoint> swi_;

};
