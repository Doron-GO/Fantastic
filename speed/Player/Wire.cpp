#include<DxLib.h>
#include "Wire.h"
#include"Player.h"

Wire::Wire(Player& player ):player_(player)
{
	_phase = &Wire::AnchoringPhase;

}

Wire::~Wire()
{
}

void Wire::Update()
{
	delta_ = timeManager_.Update()*2;
	(this->*_phase)();

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
		DrawFormatStringF(0, 180, 0xffffff, "delta:%f", delta_);
		DrawFormatStringF(0, 200, 0xffffff, "�x�_pos_:x%f:y%f", fulcrum_.x,fulcrum_.y);
		DrawFormatStringF(0, 220, 0xffffff, "angle_:%f", angle_);

	}
}

void Wire::SwingPhase()
{
	auto coe = -gMag_ / length_;				//k�̌W�� k�͌X���炵��
	//�����Q=�N�b�^�@�̌v�Z
	auto k1 = delta_ * coe * sin(theta_);
	auto m1 = delta_ * omega_;

	auto k2 = delta_ * coe * sin(theta_ + m1 / 2.f);
	auto m2 = delta_ * (omega_ + k1 / 2.f);

	auto k3 = delta_ * coe * sin(theta_ + m2 / 2.f);
	auto m3 = delta_ * (omega_ + k2 / 2.f);

	auto k4 = delta_ * coe * sin(theta_ + m3);
	auto m4 = delta_ * (omega_ + k3);

	omega_ += (k1 + 2.f * k2 + 2.f * k3 + k4) / 6.0f;		//�p���x�̉��Z
	theta_ += (m1 + 2.f * m2 + 2.f * m3 + m4) / 6.0f;		//�p�x�̉��Z

	//�p�x���琎�̈ʒu�𓱏o
	player_.pos_ = fulcrum_ + length_ * cos(theta_) *  gravity_.Normalized() + length_ * sin(theta_) * yNorm_;


}

void Wire::TestSwingPhase()
{
	auto gravity = 0.5f;
	//�A���O���������Ă�
	angle_ = atan2f(player_.pos_.x - fulcrum_.x, player_.pos_.y - fulcrum_.y);
	V += gravity * sinf(angle_);
	vel_ = { -V * cosf(angle_),V * sinf(angle_) };

	Vector2DFloat vel = { vel_.x,vel_.y };
	player_.pos_ += vel;		//vel�����Z
	player_.pos_ = fulcrum_ + (player_.pos_ - fulcrum_).Normalized() * length_;

	if (player_.pos_.y <= fulcrum_.y)
	{
		V = vel.Magnitude();
		vel = { -vel.y,vel.y * 0.1f };
	}

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

	//�����Q�N�b�^�@�̂��߂̏����ݒ�

	gravity_ = player_.GetMoveVec();
	gMag_ = gravity_.Magnitude();
	auto lVec = player_.pos_ - fulcrum_;	//�x�_�����̃x�N�g��(�R)

	length_ = lVec.Magnitude();							//�R�̒���

	auto grNorm = gravity_.Normalized();			//�d�͂̐��K���ς݃x�N�g��
	auto x = grNorm.Dot(lVec);					//�d�̓x�N�g���Ɛ�����d�͎��܂ł̃x�N�g���������_�Ǝx�_�̑傫��

	yNorm_ = lVec - grNorm * x;										//������d�͎��܂ł̃x�N�g��
	auto y = yNorm_.Magnitude();								//������d�͎��܂ł̋���
	yNorm_.Normalize();												//������d�͎��܂ł̐��K���ς݃x�N�g��

	theta_ = atan2f(y, x);											//�x�_�̋ߐړ�ӂ̂��ꂼ��̑傫�����g�p���Ċp�x���o��
	omega_ = -(abs(player_.GetMovePow().x/2.0f));					//�p���x��0�ŏ�����  �ł����̓v���C���[��

	

	//�����ŃA���O���̏����ݒ������
	angle_ = atan2f(player_.pos_.x - fulcrum_.x, player_.pos_.y - fulcrum_.y);
	V = -2 * vel_.x * cosf(angle_);//x���̑��x

	_phase = &Wire::TestSwingPhase;
}

void Wire::EndSwing()
{
	_phase = &Wire::EndSwing;
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
