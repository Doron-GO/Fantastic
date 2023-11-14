#pragma once
#include<map>
#include<string>
#include"../Vector2D.h"
#include"../Object/Manager/AnimMng.h"
#include"../Common/Collision.h"
#include"../Common/Raycast.h"
#include"../Input/Input.h"
#include"../Object/Camera.h"

class Wire;

class Player
{
public:

	enum class DIR_LR
	{
		RIGHT,
		LEFT
	};
	enum class PHASE
	{
		FALL,
		JUMP,
		MOVE,
		WALLSLIDE,
		WALLJUMP,
		SWING,
		SWINGJUMP
	};

	 Player(int playerNum);
	 ~Player();
	 void Init(ColList colList, ColList wallColList, ColList wireColList);
	 void Update(Input& input);

	//����ɃI�t�Z�b�g�l��n���`������炷�悤�ɂ���
	 void Draw(Vector2DFloat cameraPos);


	ColList grndColList_;//���ɃM�~�b�N�̂Ȃ������蔻��
	ColList wallcolList_;//�ǃW�����v���ł��铖���蔻��

	const Vector2DFloat GetPos();
	const Vector2DFloat GetDiagonallyVecVec();
	const Vector2DFloat GetMoveVec();
	const Vector2DFloat GetMovePow();
	Vector2DFloat pos_;//�L�����̍��W
	Vector2DFloat movePow_;	//�ړ������
	int padNum_;//���������Ԗڂ�PAD���g���Ă��邩
	DIR_LR dir_LR_;//�L�����N�^�[�̌���

	//SwingJump��ԂɕύX
	void StartSwingJump();
	//Swing��ԂɕύX
	void StartSwing();

private:

	PHASE phase_;
	std::string now_;

	void DebugPhaseCheck();

	void (Player::* _phase)(Input& input);

	//�v���C���[�̏��
	void IdlePhase(Input& input);
	void MovePhase(Input& input);//���E�ړ��F�W�����v
	void JumpPhese(Input& input);//�W�����v
	void FallPhase(Input& input);//������
	void WallSlidePhese(Input& input);//�ǂ��藎��
	void WallJumpPhese(Input& input);//�ǃW�����v
	void SwingPhese(Input& input);//�X�C���O���
	void SwingJumpPhese(Input& input);//�X�C���O�W�����v���
	void AnchoringPhese(Input& input);//�X�C���O�W�����v���


	//�����蔻��n
	void MoveColision();
	//�����̒��S���� true �������ĂȂ�:false �������Ă�
	bool CollisionDown();
	//��������@true �������ĂȂ�:false �������Ă�
	bool CollisionVec(Vector2DFloat movevec);
	bool ColWallSlide(Vector2DFloat movevec);
	bool IsWall();

	void Dead();


	//�v���C���[�̃A�N�V����
	void Move(Input& input);	//���E�ړ�
	void Anchoring(Input& input);//�t�b�N���΂�:�i�s�����̎΂ߏ�
	void Jump(Input& input);

	std::map<std::string, int> imgkey_;
	
	AnimStr animeStr_;
	Input input_;

	Vector2DFloat center_;//�L�����̒��S���W
	Raycast rayCast_;

	Vector2DFloat cameraPos_;//�J�����̍��W
	Vector2DFloat moveVec_;	//�����Ă������
	Vector2DFloat diagonallyVec_;//�����Ă�������̎΂ߏ�
	Vector2DFloat up_;	//�����Ă������

	std::unique_ptr<Wire> wire_;

	bool AnchoringFlag_;
	bool aliveFlag_;

	float slideY_ = -35.0f;	
	int test= 0xffffff;
};

