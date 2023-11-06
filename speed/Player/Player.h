#pragma once
#include<map>
#include<string>
#include"../Vector2D.h"
#include"../Object/AnimMng.h"
#include"../Common/Collision.h"
#include"../Common/Raycast.h"
#include"../Input/Input.h"
#include"../Object/Camera.h"

class Wire;

enum class DIR_LR
{
	RIGHT,
	LEFT
};

class Player
{
public:
	 Player(int playerNum);
	 ~Player();
	 void Init(GrndColList colList, WallColList wallColList);
	 void Update(Input& input);
	 void Draw(Vector2DFloat cameraPos);//����ɃI�t�Z�b�g�l��n���`������炷�悤�ɂ���
	 const Vector2DFloat GetPos();

	 int headFlag_;//�������擪���ǂ���

	GrndColList grndColList_;//���ɃM�~�b�N�̂Ȃ������蔻��
	WallColList wallcolList_;//�ǃW�����v���ł��铖���蔻��


	Vector2DFloat GetDiagonallyVecVec();
	Vector2DFloat pos_;//�L�����̍��W

private:

	void (Player::* _phase)(Input& input);
	void (Player::* _draw)();

	void IdlePhase(Input& input);
	void MovePhase(Input& input);//���E�ړ��F�W�����v
	void JumpPhese(Input& input);//�W�����v
	void FallPhase(Input& input);//������
	void WallSlidePhese(Input& input);//�ǂ��藎��
	void WallJumpPhese(Input& input);//�ǃW�����v
	//�����̒��S���� true �������ĂȂ�:false �������Ă�
	bool Collision();
	//��������@true �������ĂȂ�:false �������Ă�
	bool Collision(Vector2DFloat movevec);


	bool ColWallSlide(Vector2DFloat movevec);

	void IdleDraw();
	void JumpDraw();
	void MoveDraw();

	void Move(Input& input);	//���E�ړ�

	void Anchoring(Input& input);//�t�b�N���΂�:�i�s�����̎΂ߏ�
	void Jump(Input& input);

	std::map<std::string, int> imgkey_;
	
	AnimStr animeStr_;
	Input input_;
	int padNum_;//���������Ԗڂ�PAD���g���Ă��邩

	DIR_LR dir_LR_;//�L�����N�^�[�̌���
	Vector2DFloat center_;//�L�����̒��S���W
	Raycast rayCast_;

	Vector2DFloat cameraPos_;//�J�����̍��W
	Vector2DFloat movePow_;	//�ړ������
	Vector2DFloat moveVec_;	//�����Ă������
	Vector2DFloat diagonallyVec_;	//�����Ă������



	std::unique_ptr<Wire> wire_;

	float slideY_ = -35.0f;	
	int test= 0xffffff;
};

