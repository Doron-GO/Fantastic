#pragma once
#include"../../Vector2D.h"
#include"../../Common/Collision.h"
#include<vector>
#include<list>
#include<memory>

class Camera;
class DangerZoneSmaller;
class Player;


//画面端の爆発を作るための構造体
struct Bomb
{
	Vector2DFloat pos_ = {};
	int frame_ = 0;
	bool isDead = false;
	Bomb(const Vector2DFloat& p) :pos_(p) {}
};

class OutSide
{
public:
	
	OutSide(Camera& camera,std::vector< std::shared_ptr<Player >>& players, int playerCount);
	~OutSide();

	void Update();
	void ExplosionUpdate();
	void Draw(Vector2DFloat offset);

	//画面外かどうかを判定
	void IsDead();
	bool IsOutSide(Vector2DFloat pos);

	//画面からはみ出した時、上下から出たのか左右から出たのか
	//true:上下　false: 左右
	void UpDownORLeftRight(Vector2DFloat pos);
	void UpORDown(Vector2DFloat pos);
	void LeftOrRight(Vector2DFloat pos);

	void TestSmaller(); 
	const int NumberOfSurvivors();

private:

	Camera& camera_;

	std::unique_ptr<DangerZoneSmaller> dangerZone_;
	std::vector<std::shared_ptr<Player>>& players_;
	Vector2DFloat minPos_;
	Vector2DFloat maxPos_;

	Vector2DFloat minScale_;
	Vector2DFloat maxScale_;

	Vector2DFloat upperPos_;
	Vector2DFloat upperVec_;
	Vector2DFloat lowerPos_;
	Vector2DFloat lowerVec_;
	std::list<Bomb> bombs_;

	bool isExploding_ = false;
	bool bigExploding_ = false;

	int ExplosionSound_;
	int padNum_;
	int bombImg_[11];
	int bigBombImg_[8];
	int frame_;
	int bigFrame_;

	int playerCount_;

};

