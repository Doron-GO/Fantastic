#include<DxLib.h>
#include "Player.h"
#include"../Input/Input.h"
#include"../Obj/ImageMng.h"
#include"../_debug/_DebugConOut.h"
#include"../_debug/_DebugDispOut.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Init(ColList colList)
{
	pos_ = { 300.0f,10.0f };

	center_ = { 0.0f,13.0f };
	colList_ = colList;

	lpAnimMng.LoadAnime("Src/Img/act.list");

	animeStr_.objID_ = "Player";

	lpAnimMng.SetAnime(animeStr_, "Idle");

	offset_ = (view / 3.0f) - pos_;
	moveVec_ = { 0.0f,0.0f };
	movePow_ = { 0.0f,0.0f };
	dir_LR_ = DIR_LR::LIGHT;
	_phase = &Player::FallPhase;
	_draw = &Player::MoveDraw;
}

void Player::Update(Input& input)
{
	offset_ = (view / 3.0f) - pos_;

	lpAnimMng.UpdateAnime(animeStr_);

	(this->*_phase)(input);
	Move(input);


	//�ǂɓ������Ă����牡�ړ������Ȃ�
	if (Collision(moveVec_))
	{
		pos_.x += movePow_.x;
	}
	pos_.y += movePow_.y;

}

void Player::Draw()
{
	//(this->*_draw)();

	Vector2DFloat view = { 800.0f, 600.0f };



	DrawRotaGraph2F(pos_.x+ offset_.x, pos_.y + offset_.y,
		24.0f, 35.0f,
		1.5, 0.0,
		lpImageMng.GetID(animeStr_.imgKey_)[(*animeStr_.animID_)[GraphHD]],
		true, static_cast<int>(dir_LR_), 0);


	DrawFormatStringF(0, 0, 0xffffff, "movePow_(x:%f,y%f)", movePow_.x, movePow_.y);
	DrawFormatStringF(0, 20, 0xffffff, "pos_(x:%f,y%f)", pos_.x, pos_.y);
	if (!Collision(moveVec_))
	{
		DrawString(0, 40, "��������", 0xffffff);
	}
	//DrawCircle(pos_.x, pos_.y, 15, 0xffffff);
	Vector2DFloat rayCenter = { pos_ - center_ };

	DrawLine(pos_.x+ offset_.x, pos_.y + offset_.y,
		moveVec_.x+ pos_.x + offset_.x, moveVec_.y+ pos_.y + offset_.y, 0x00ffff);

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
	if (Collision())
	{
		_phase = &Player::FallPhase;
	}
	Jump(input);
}

void Player::JumpPhese(Input& input)
{
	lpAnimMng.SetAnime(animeStr_, "Jump");
	
	Vector2DFloat movevec={ 0.0f,-40.0f };

	if((movePow_.y<=-13.0f)||!(Collision(movevec)))
	{
		movePow_.y = 0.0f;
		_phase = &Player::FallPhase;
	}
	else
	{
		movePow_.y += -0.2f;
		movePow_.y += -0.3f;
	}
	

}

void Player::FallPhase(Input& input)
{
	lpAnimMng.SetAnime(animeStr_, "Fall");

	//���Ɠ������Ă��Ȃ�������
	if (Collision())
	{
		movePow_.y += 0.2f;
	}

	//�������x�����𒴂����猈�܂����l�ɂ���
	if (movePow_.y >= 5.8f)
	{
		movePow_.y = 5.8f;
	}
	//�ڒn������n��ړ����[�h�ɂ���
	if (!Collision())
	{
		movePow_.y = 0.0f;
		_phase = &Player::MovePhase;
	}
}

bool Player::Collision()
{
	Vector2DFloat rayCenter = { pos_-center_};
	Vector2DFloat moveVec = { 0.0f,20.0f };
	for (const auto& col : colList_)
	{
		Raycast::Ray ray = { rayCenter,moveVec};

		if (rayCast_.CheckCollision(ray, col,pos_+offset_))
		{

			return false;
		}
	}
	return true;

}

bool Player::Collision(Vector2DFloat movevec)
{
	Vector2DFloat rayCenter = { pos_  };
	_dbgDrawLine(
		rayCenter.x, rayCenter.y,
		rayCenter.x + movevec.x, rayCenter.y + movevec.y,
		0x00ff00
	);

	for (const auto& col : colList_)
	{
		Raycast::Ray ray = { rayCenter,movevec };

		if (rayCast_.CheckCollision(ray, col, pos_ + offset_))
		{

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

	//DrawRotaGraph2F(pos_.x, pos_.y,
	//	24.0f, 35.0f,
	//	1.5, 0.0,
	//	lpImageMng.GetID(animeStr_.imgKey_)[(*animeStr_.animID_)[GraphHD]],
	//	true, static_cast<int>(dir_LR_), 0);

}

void Player::Move(Input& input)
{
	float speed = 0.2f;
	//�E�������͍��L�[��������Ă��Ȃ��Ƃ�
	if (!input.IsTrigger("right")&&!input.IsTrigger("left"))
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
	if (!input.IsTrigger("slide"))
	{

		//�E�L�[
		if (input.IsTrigger("right"))
		{
			dir_LR_ = DIR_LR::LIGHT;
			movePow_.x += 0.2f;
			moveVec_ = { 20.0f,-30.0f };

		}
		
		if (input.IsTrigger("left"))
		{
			dir_LR_ = DIR_LR::LEFT;
			movePow_.x -= 0.2f;
			moveVec_ = { -20.0f,-30.0f };

		}
		//���L�[
		if (movePow_.x <= -8.0f)
		{
			movePow_.x = -8.0f;
		}
		else if (movePow_.x >= 8.0f)
		{
			movePow_.x = 8.0f;
		}
	}
	//�ǂɓ�������������x���O�ɂ���
	if (!Collision(moveVec_))
	{
		movePow_.x = 0.0f;
	}

	//����������Ȃ��Ƃ�
	if (!(_phase == &Player::FallPhase)&&!(_phase == &Player::JumpPhese))
	{
		//�X���C�f�B���O�{�^����������Ă�����
		if (input.IsTrigger("slide"))
		{
			lpAnimMng.SetAnime(animeStr_, "Slide");
			//�X���C�f�B���O���͌�������
			if (movePow_.x >= 0.05f)
			{
				movePow_.x -= 0.1f;
			}
			if (movePow_.x <= -0.05f)
			{
				movePow_.x += 0.1f;
			}
		}
	}

}

void Player::Jump(Input& input)
{
	//��L�[���������Ƃ�
	if (input.IsTrigger("up"))
	{
		lpAnimMng.SetAnime(animeStr_, "Jump");
		movePow_.y = 0.0f;
		_phase = &Player::JumpPhese;

	}
}
