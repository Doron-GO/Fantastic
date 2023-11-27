#pragma once
#include<memory>
#include<Dxlib.h>
#include<vector>
#include"../../Vector2D.h"

class Player;

class Camera
{
public:
	Camera();
	~Camera();
	bool Init(const Vector2D& worldSize);
	void Update();

	//カメラの状態を変える
	void PhaseChanging(int num);

	bool ReConnect(std::weak_ptr<Player> actor);
	const Vector2DFloat& GetPos(void)const;
	const Vector2DFloat& GetTargetPos(void)const;
private:

	float time = 0;

	int nextNum_=0;
	void (Camera::* _phase)();
	//カメラ追従状態
	void Switching();
	//追従対象変更中
	void Follow();
	//ズーム
	void Zoom();

	std::weak_ptr<Player> taeget_;
	Vector2DFloat cameraPos_;
	Vector2DFloat oldPos_;
	RECT moveArea_;
};

