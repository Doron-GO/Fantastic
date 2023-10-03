#pragma once
#include<map>
#include<string>
#include"../Vector2D.h"
#include"../Obj/AnimMng.h"
#include"../Common/Collision.h"
#include"../Common/Raycast.h"

class Input;

enum class DIR_LR
{
	LIGHT,
	LEFT
};

class Player
{
public:
	 Player();
	 ~Player();
	 void Init(ColList colList);
	 void Update(Input& input);
	 void Draw();
	 const Vector2DFloat GetPos();
private:

	void (Player::* _phase)(Input& input);
	void (Player::* _draw)();

	void IdlePhase(Input& input);
	void MovePhase(Input& input);//���E�ړ��F�W�����v
	void JumpPhese(Input& input);//�W�����v
	void FallPhase(Input& input);//������

	bool Collision();

	void IdleDraw();
	void JumpDraw();
	void MoveDraw();


	//���E�ړ�
	void Move(Input& input);
	void Jump(Input& input);

	std::map<std::string, int> imgkey_;
	

	AnimStr animeStr_;


	DIR_LR dir_LR_;//�L�����N�^�[�̌���

	Vector2DFloat pos_;//�L�����̍��W
	Vector2DFloat center_;//�L�����̒��S���W
	ColList colList_;
	Raycast rayCast_;

	Vector2DFloat movePow_;
	Vector2DFloat moveVec_;

	//VECTOR movePow_;
	//VECTOR moveVec_;
};

