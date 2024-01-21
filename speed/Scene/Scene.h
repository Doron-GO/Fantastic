#pragma once
#include"../Vector2D.h"
class Input;
class SceneMng;
/// <summary>
/// �V�[���̋K��N���X(����1�N���X
/// </summary>

class Scene
{
public:
	Scene(SceneMng& scene,int n );
	virtual void Update(Input& input) = 0;
	virtual void DrawOwnScreen(void) = 0;
	virtual void Draw() ;
protected:
	SceneMng& sceneManager_;
	int screenID_;
	Vector2DFloat drawOffset_;

};

