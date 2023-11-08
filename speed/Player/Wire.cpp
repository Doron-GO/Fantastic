#include<DxLib.h>
#include "Wire.h"
#include"Player.h"

Wire::Wire(Player& player ):player_(player), pow_(0.0f)
{
	_phase = &Wire::AnchoringPhase;

}

Wire::~Wire()
{
}

void Wire::Update()
{
	(this->*_phase)();
	Pump();
}

void Wire::Draw(Vector2DFloat cameraPos)
{
	auto pPos = player_.pos_;
	pPos += cameraPos;
	DrawCircle(fulcrum_.x+ cameraPos.x,fulcrum_.y+ cameraPos.y,5,0xff0000);
	DrawLine(pPos.x, pPos.y, fulcrum_.x + cameraPos.x, fulcrum_.y + cameraPos.y, 0xffffff);

	if (player_.padNum_ == 1)
	{
		DrawFormatStringF(0, 160, 0xffffff, "DiagonallyVecVec:x.%f,y.%f", player_.GetDiagonallyVecVec().x, player_.GetDiagonallyVecVec().y);
		DrawFormatStringF(0, 200, 0xffffff, "�x�_pos_:x%f:y%f", fulcrum_.x,fulcrum_.y);
		DrawFormatStringF(0, 220, 0xffffff, "angle_:%f", angle_);
	}
}


void Wire::SwingPhase()
{
	auto gravity = 0.5f;
	//�A���O���������Ă�
	angle_ = atan2f(player_.pos_.x - fulcrum_.x, player_.pos_.y - fulcrum_.y);
	v_ += gravity * sinf(angle_);
	vel_ = { -v_ * cosf(angle_),v_ * sinf(angle_) };

	Vector2DFloat vel = { vel_.x,vel_.y };
	player_.pos_ += vel;		//vel�����Z
	player_.pos_ = fulcrum_ + (player_.pos_ - fulcrum_).Normalized() * length_;//������␳

	if (player_.pos_.y <= fulcrum_.y + -150.0f)
	{
		StartSwingJump();
	}
}

void Wire::SwingJump()
{
	_phase = &Wire::EndSwingPhase;
	player_.movePow_.x = (vel_.x / 2.0f);
	player_.movePow_.y = (vel_.y / 2.0f);
}

void Wire::EndSwingPhase()
{

}

void Wire:: AnchoringPhase()
{
	//�̎��Ƃō����Vector���g���Ă��邹���ł߂�ǂ��������ƂɂȂ��Ă���
}

void Wire::SetPalam()
{	
	//�x�_������
	fulcrum_ = player_.pos_;
	VECTOR pos = { fulcrum_.x,fulcrum_.y };
	VECTOR moveVec = { player_.GetDiagonallyVecVec().x,player_.GetDiagonallyVecVec().y };
	moveVec=VNorm(moveVec);
	Scale_ = VScale(moveVec, 400.0f);
	movedPos_ = VAdd(pos, Scale_);
	fulcrum_.x = movedPos_.x;
	fulcrum_.y = movedPos_.y;
	vel_ = { 0.0f,0.0f };
	auto lVec = player_.pos_ - fulcrum_;	//�x�_�����̃x�N�g��(�R)

	length_ = lVec.Magnitude();							//�R�̒���
	vel_.x = player_.movePow_.x;	//�����x�I��

	//�����ŃA���O���̏����ݒ������
	angle_ = atan2f(player_.pos_.x - fulcrum_.x, player_.pos_.y - fulcrum_.y);
	v_ = -2 * vel_.x * cosf(angle_);//x���̑��x


	if (player_.dir_LR_ == Player::DIR_LR::LEFT)
	{
		pow_ = 0.3f;
	}
	else
	{
		pow_ = -0.3f;
	}
	_phase = &Wire::SwingPhase;

}

void Wire::StartSwingJump()
{
	_phase = &Wire::EndSwingPhase;

	player_.StartSwingJump();
}

void Wire::EndSwing()
{
	_phase = &Wire::EndSwingPhase;
}

void Wire::Pump()
{
	if (_phase==&Wire::SwingPhase)
	{
		v_ += pow_;
	}
}

bool Wire::IsHitHook()
{
	for (const auto& col :player_. wallcolList_)
	{
		Raycast::Ray ray = { fulcrum_,moveVec_ };
		if (rayCast_.CheckCollision(ray, col, fulcrum_))
		{		
			return false;
		}
	}
	return true;
}
