#pragma once
#include"../../Vector2D.h"
#include"../../Common/Collision.h"
#include<vector>
#include<list>
#include<memory>

class Camera;
class Player;


//âÊñ í[ÇÃîöî≠ÇçÏÇÈÇΩÇﬂÇÃç\ë¢ëÃ
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
	
	OutSide(Camera& camera,std::vector< std::shared_ptr<Player >>& players);
	~OutSide();

	void Update();
	void ExplosionUpdate();
	void Draw();

	//âÊñ äOÇ©Ç«Ç§Ç©ÇîªíË
	void IsDead();
	bool IsOutSide(Vector2DFloat pos);

	void (OutSide::* _phase)();


private:

	Camera& camera_;
	std::vector<std::shared_ptr<Player>>& players_;
	Vector2DFloat minPos_;
	Vector2DFloat maxPos_;


	Vector2DFloat upperPos_;
	Vector2DFloat upperVec_;
	Vector2DFloat lowerPos_;
	Vector2DFloat lowerVec_;
	std::list<Bomb> bombs_;

	bool isExploding_ = false;
	int frame_;

};

