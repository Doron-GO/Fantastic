#pragma once
#include<map>
#include<string>
#include"../Vector2D.h"
class Input;

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

	std::map<std::string, int> imgkey_;
	
	//�����Ă���摜
	int runImg_[6];
	//�����摜�v�̃J�E���g�p
	int runFrame_;

	Vector2DFloat pos_;//�v���C���[�̍��W

};

