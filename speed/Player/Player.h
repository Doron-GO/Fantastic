#pragma once
#include<map>
#include<string>
#include"../Vector2D.h"
#include"../Obj/AnimMng.h"
#include"../Common/Collision.h"
#include"../Common/Raycast.h"
#include"../Input/Input.h"
#include"../Obj/Camera.h"
enum class DIR_LR
{
	LIGHT,
	LEFT
};

class Player
{
public:
	 Player(int playerNum);
	 ~Player();
	 void Init(ColList colList,int headTest);
	 void Update(Input& input);
	 void Draw();
	 const Vector2DFloat GetPos();

	 int headFlag_;//�������擪���ǂ���

private:

	void (Player::* _phase)(Input& input);
	void (Player::* _draw)();

	void IdlePhase(Input& input);
	void MovePhase(Input& input);//���E�ړ��F�W�����v
	void JumpPhese(Input& input);//�W�����v
	void FallPhase(Input& input);//������

	bool Collision();
	bool Collision(Vector2DFloat movevec);

	void IdleDraw();
	void JumpDraw();
	void MoveDraw();


	//���E�ړ�
	void Move(Input& input);
	void Jump(Input& input);

	std::map<std::string, int> imgkey_;
	
	AnimStr animeStr_;

	Input input_;

	int padNum_;//���������Ԗڂ�PAD���g���Ă��邩

	DIR_LR dir_LR_;//�L�����N�^�[�̌���
	Vector2DFloat pos_;//�L�����̍��W
	Vector2DFloat oldPos_;//�O�̃L�������W
	Vector2DFloat center_;//�L�����̒��S���W
	ColList colList_;
	Raycast rayCast_;

	Vector2DFloat cameraPos_;//�J�����̍��W

	Vector2DFloat view = { 1200.0f, 800.0f };

	Vector2DFloat offset_;

	

	Vector2DFloat movePow_;
	Vector2DFloat moveVec_;

	//VECTOR movePow_;
	//VECTOR moveVec_;
};

