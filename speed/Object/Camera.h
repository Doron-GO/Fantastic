#pragma once
#include<memory>
#include<Dxlib.h>
#include<vector>
#include"../Vector2D.h"

class Player;

class Camera
{
public:
	Camera();
	~Camera();
	bool Init(const Vector2D& worldSize);
	void Update();

	//ƒJƒƒ‰‚Ìó‘Ô‚ğ•Ï‚¦‚é
	void PhaseChanging(int num);

	bool ReConnect(std::weak_ptr<Player> actor);
	const Vector2DFloat& GetPos(void)const;
	const Vector2DFloat& GetTargetPos(void)const;
private:

	float time = 0;

	int nextNum_=0;
	void (Camera::* _phase)();
	//ƒJƒƒ‰’Ç]ó‘Ô
	void Switching();
	//’Ç]‘ÎÛ•ÏX’†
	void Follow();

	std::weak_ptr<Player> taeget_;
	Vector2DFloat cameraPos_;
	Vector2DFloat oldPos_;
	RECT moveArea_;
};

