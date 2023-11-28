#pragma once
#include"../../Vector2D.h"
#include"../../Common/Collision.h"
#include<vector>
#include<list>
#include<memory>

class Camera;
class Player;


//��ʒ[�̔�������邽�߂̍\����
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
	void Draw(Vector2DFloat offset);

	//��ʊO���ǂ����𔻒�
	void IsDead();
	bool IsOutSide(Vector2DFloat pos);

	//��ʂ���͂ݏo�������A�㉺����o���̂����E����o���̂�
	//true:�㉺�@false: ���E
	bool UpDownORLeftRight(Vector2DFloat pos);

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
	int bombImg_[11];
	int frame_;

};

