#pragma once
#include<map>
#include<string>
#include"../Vector2D.h"
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

	//���E�ړ�
	void Move(Input& input);
	void Jump(Input& input);

	std::map<std::string, int> imgkey_;
	
	//�����Ă���摜
	int runImg_[6];
	//�����摜�v�̃J�E���g�p
	int runFrame_;


	DIR_LR dir_LR_;//�L�����N�^�[�̌���

	Vector2DFloat pos_another_way;//�L�����̍��W

	VECTOR pos_;
	VECTOR movePow_;
	VECTOR moveVec_;
};

