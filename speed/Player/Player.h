#pragma once
#include<map>
#include<string>
#include"../Vector2D.h"
#include"../Obj/AnimMng.h"

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
	 void Init();
	 void Update(Input& input);
	 void Draw();
private:

	void (Player::* _phase)(Input& input);
	void (Player::* _draw)();

	void IdlePhase(Input& input);
	void MovePhase(Input& input);//���E�ړ��F�W�����v
	void JumpPhese(Input& input);//�W�����v
	void FallPhase(Input& input);//������

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

	VECTOR movePow_;
	VECTOR moveVec_;
};

