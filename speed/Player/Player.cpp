#include<DxLib.h>
#include<string>
#include "Player.h"
#include"../Object/ImageMng.h"
#include"../_debug/_DebugConOut.h"
#include"../_debug/_DebugDispOut.h"

Player::Player(int playerNum)
{
	padNum_ = playerNum;
}

Player::~Player()
{
}

void Player::Init(GrndColList colList, WallColList wallColList)
{
	pos_ = { 300.0f,10.0f };

	center_ = { 0.0f,11.0f };
	colList_ = colList;
	wallcolList_ = wallColList;

	//�x�X�g�ȕ��@�ł͂Ȃ����������ǁAPad�i���o�[���g����actlist��ς���
	 char num = '0'+padNum_;
	 std::string act = "Src/Img/act";//string����������
	 act += num;						 //	�������A��
	 act +=".list";					 //	�������A��

	lpAnimMng.LoadAnime(act.c_str());

	std::string objID = "Player";
	objID += num;
	animeStr_.objID_ = objID.c_str();

	lpAnimMng.SetAnime(animeStr_, "Idle");


	if (headFlag_==1)
	{
		headFlag_ = true;
	}
	else
	{
		headFlag_ = false;
	}
	moveVec_ = { 0.0f,0.0f };
	movePow_ = { 0.0f,0.0f };
	dir_LR_ = DIR_LR::LIGHT;
	_phase = &Player::FallPhase;
	_draw = &Player::MoveDraw;
	

}

void Player::Update(Input& input)
{
	lpAnimMng.UpdateAnime(animeStr_);
	input_.Update(padNum_);
	Move(input_);
	(this->*_phase)(input_);

	pos_.y += movePow_.y;
	//�ǂɓ������Ă����牡�ړ������Ȃ�
	if (Collision(moveVec_))
	{
		pos_.x += movePow_.x;
	}

}

void Player::Draw(Vector2DFloat cameraPos)
{

	auto pos = pos_ + cameraPos;

	//�f�o�b�O�p�̎��ۂ̃L�����̍��W��\��

	//DrawRotaGraph2F(pos_.x, pos_.y ,
		//	24.0f, 35.0f,
		//	1.5, 0.0,
		//	lpImageMng.GetID(animeStr_.imgKey_)[(*animeStr_.animID_)[GraphHD]],
		//	true, static_cast<int>(dir_LR_), 0);

	//���������̃L�����N�^�[�`��
	DrawRotaGraph2F(pos.x, pos.y,
		24.0f, 35.0f,
		1.5, 0.0,
		lpImageMng.GetID(animeStr_.imgKey_)[(*animeStr_.animID_)[GraphHD]],
		true, static_cast<int>(dir_LR_), 0);
	if (padNum_ == 1)
	{
		DrawFormatStringF(0, 0, 0xffffff, "movePow_(x:%f,y%f)", movePow_.x, movePow_.y);
		DrawFormatStringF(0, 20, 0xffffff, "pos_(x:%f,y%f)", pos_.x, pos_.y);

	}
	if (!Collision(moveVec_))
	{
		DrawString(0, 40, "�ǂɓ�������", 0xffffff);
	}
	if (!Collision())
	{
		DrawString(0, 60, "���ɓ�������", 0xffffff);
	}
	if (!CollWallJump(moveVec_))
	{
		DrawString(0, 70, "�W�����v�ǂɓ�������", test);
	}

	Vector2DFloat rayCenter = { pos - center_ };
	Vector2DFloat diagonallyVec = { moveVec_.x,slideY_ };

	if (_phase == &Player::MovePhase)
	{
		//��������i�s�����ɏo�郌�C
		DrawLine(pos.x, pos.y ,
			moveVec_.x+ pos.x , moveVec_.y+ pos.y , 0xff0000);
		//��������O�΂ߑO�ɐL�т郌�C
		DrawLine(pos.x, pos.y ,
			diagonallyVec.x+ pos.x , diagonallyVec.y+ pos.y , 0xff0000);
		//�������牺�ɐL�΂����C
		Vector2DFloat moveVec = { 0.0f,20.0f };
		DrawLine(rayCenter.x, rayCenter.y,
			moveVec.x + rayCenter.x, moveVec.y + rayCenter.y, 0xff0000);
	}

	if (_phase == &Player::JumpPhese)
	{
		Vector2DFloat movevec = { 0.0f,-40.0f };
		DrawLine(rayCenter.x, rayCenter.y,
			movevec.x + rayCenter.x, movevec.y + rayCenter.y, 0xff0000);
	}
	if (_phase == &Player::FallPhase)
	{
		Vector2DFloat movevec = { 0.0f,22.0f };
		DrawLine(rayCenter.x, rayCenter.y,
			movevec.x + rayCenter.x, movevec.y + rayCenter.y, 0xff0000);
	}
}

const Vector2DFloat Player::GetPos()
{
	return pos_;
}

void Player::IdlePhase(Input& input)
{

}

void Player::MovePhase(Input& input)
{
	//Move(input);
	//���������Ȃ�������t�H�[���ɂ���
	Vector2DFloat movevec = { 0.0f,8.2f };
	if ((Collision(movevec)))
	{
		_phase = &Player::FallPhase;
	}
	Jump(input);
}

void Player::JumpPhese(Input& input)
{
	lpAnimMng.SetAnime(animeStr_, "Jump");	
	Vector2DFloat movevec={ 0.0f,-40.0f };

	//�W�����v���x���ő�ɒB�����������́A�n�ʂɐڒn������
	if((movePow_.y<=-13.0f)||!(Collision(movevec)))
	{
		//���̈ړ���0�ɂ��ăt�H�[�����Ă�
		movePow_.y = 0.0f;
		_phase = &Player::FallPhase;
	}
	//�łȂ����y�̈ړ��ʂ����Z����
	else
	{
		movePow_.y += -0.2f;
		movePow_.y += -0.3f;
	}
	
}

void Player::FallPhase(Input& input)
{
	lpAnimMng.SetAnime(animeStr_, "Fall");

	//�������x�����𒴂����猈�܂����l�ɂ���
	if (movePow_.y >= 8.0f)
	{
		movePow_.y = 8.0f;
	}	

	Vector2DFloat movecec = { 0.0f,movePow_.y };

	//���Ɠ������Ă��Ȃ�������
	if (Collision(movecec))
	{
		movePow_.y += 0.2f;
	}
	movecec = { 0.0f,movePow_.y };
	//�ڒn������n��ړ����[�h�ɂ���
	if (!Collision(movecec))
	{	
		movePow_.y = 0.0f;
		_phase = &Player::MovePhase;
	}

}

void Player::WallJumpPhese(Input& input)
{
	movePow_.y=2.0f;


}

bool Player::Collision()
{
	Vector2DFloat rayCenter = { pos_-center_};
	Vector2DFloat moveVec = { 0.0f,16.0f };
	for (const auto& col : colList_)
	{
		Raycast::Ray ray = { rayCenter,moveVec};

		if (rayCast_.CheckCollision(ray, col,pos_))
		{
			//TRACE("��������\n");
			return false;
		}
	}
	return true;

}

bool Player::Collision(Vector2DFloat movevec)
{
	Vector2DFloat rayCenter = { pos_ };
	//���ۂ̓����蔻�背�C�̕`��
	
	//_dbgDrawLine(
	//	rayCenter.x, rayCenter.y,
	//	rayCenter.x + movevec.x, rayCenter.y + movevec.y,
	//	0x00ff00
	//);

	for (const auto& col : colList_)
	{
		Raycast::Ray ray = { rayCenter,movevec };

		if (rayCast_.CheckCollision(ray, col, pos_ ))
		{
			return false;
		}
	}
	return true;
}

bool Player::CollWallJump(Vector2DFloat movevec)
{
	float wallX=0.0f;
	Vector2DFloat rayCenter = { pos_ - center_ };
	for (const auto& col : wallcolList_)
	{
		Raycast::Ray ray = { rayCenter,movevec };
		if (rayCast_.CheckCollision(ray, col, pos_))
		{
			wallX = (col.first.x)+32.0f ;
			if (rayCenter.x> wallX)
			{
				test = 0xff0000;
			}
			else
			{
				test = 0x0000ff;
			}
			return false;
		}
	}	
	return true;
}

void Player::IdleDraw()
{
	lpAnimMng.SetAnime(animeStr_, "Idle");

}

void Player::JumpDraw()
{
	lpAnimMng.SetAnime(animeStr_, "Jump");

}

void Player::MoveDraw()
{

}

void Player::Move(Input& input)
{
	float speed = 0.2f;
	//�E�Ƃ͍��L�[��������Ă��Ȃ��Ƃ�
	if (!input.IsPrassed("right")&&!input.IsPrassed("left"))
	{
		//�ړ��ʂ�0.1���傫��������
		if (movePow_.x >= 0.1f) 
		{
			movePow_.x -= speed;
		}
		//�ړ��ʂ�-0.1��菬����������
		if (movePow_.x <= -0.1f)
		{
			movePow_.x += speed;
		}
		//�W�����v�A�j���[�V����������Ȃ�������
		if (_phase == &Player::MovePhase)
		{
			lpAnimMng.SetAnime(animeStr_, "Idle");
		}
	}
	else 
	{
		//�W�����v�A�j���[�V����������Ȃ�������
		if (_phase == &Player::MovePhase)
		{
			lpAnimMng.SetAnime(animeStr_, "Run");
		}
	}

	//�X���C�f�B���O�{�_����������Ă��Ȃ���
	if (!input.IsPrassed("c"))
	{

		//�E�L�[
		if (input.IsPrassed("right"))
		{
			dir_LR_ = DIR_LR::LIGHT;
			movePow_.x += 0.2f;
			moveVec_ = { 22.0f,0.0f };

		}
		//���L�[
		if (input.IsPrassed("left"))
		{
			dir_LR_ = DIR_LR::LEFT;
			movePow_.x -= 0.2f;
			moveVec_ = { -22.0f,0.0f };

		}
		//�ړ����x�����𒴂���ƍő呬�x�ɌŒ肷��
		if (movePow_.x <= -8.0f)
		{
			movePow_.x = -8.0f;
		}
		else if (movePow_.x >= 8.0f)
		{
			movePow_.x = 8.0f;
		}
	}

	//�ǂɓ�������������x���O�ɂ��� 1:�����̑O�� 2:��΂ߑO 
	Vector2DFloat diagonallyVec = { moveVec_.x,slideY_ };
	if (!Collision(moveVec_)|| !Collision(diagonallyVec))
	{
		movePow_.x = 0.0f;
	}
	//�w������ǂɓ���������
	Vector2DFloat backVec = { -(moveVec_.x/2.0f),0.0f };
	if (!Collision(backVec))
	{
		movePow_.x = -(backVec.x/11.0f);
	}
	auto bacVec2= backVec;
	bacVec2.y = slideY_;

	if (!Collision(bacVec2))
	{
		movePow_.x = -(backVec.x/11.0f);
	}
	if (!CollWallJump(backVec))
	{
		movePow_.x = -(backVec.x / 11.0f);
	}
	if (!CollWallJump(backVec))
	{
		movePow_.x = -(backVec.x / 11.0f);
	}

	if (_phase == &Player::JumpPhese|| _phase == &Player::FallPhase)
	{
		if (!CollWallJump(moveVec_) || !CollWallJump(diagonallyVec))
		{
			movePow_.x = 0.0f;
			_phase = &Player::WallJumpPhese;
		}

	}

	//����������Ȃ��Ƃ�
	if (!(_phase == &Player::FallPhase)&&!(_phase == &Player::JumpPhese))
	{
		//�X���C�f�B���O�{�^����������Ă�����
		if (input.IsPrassed("c"))
		{
			lpAnimMng.SetAnime(animeStr_, "Slide");
			//�X���C�f�B���O���͌�������
			if (movePow_.x >= 0.05f) { movePow_.x -= 0.1f;}
			if (movePow_.x <= -0.05f)  {movePow_.x += 0.1f;}
			slideY_ = -15.0f;
		}
		else{ slideY_ = -35.0f; }
	}

}

void Player::Jump(Input& input)
{
	//��L�[���������Ƃ�
	if (input.IsPrassed("jump"))
	{
		lpAnimMng.SetAnime(animeStr_, "Jump");
		movePow_.y = 0.0f;
		_phase = &Player::JumpPhese;

	}
}
