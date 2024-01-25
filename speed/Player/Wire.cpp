#include<DxLib.h>
#include "Wire.h"
#include"Player.h"

Wire::Wire(Player& player, ColList& list):player_(player), pow_(0.0f),col(list), _phase(&Wire::StandbyPhase)
{
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
	if (_phase== &Wire::SwingPhase|| _phase == &Wire::AnchoringPhase)
	{
		DrawCircle(fulcrum_.x+ cameraPos.x,fulcrum_.y+ cameraPos.y,5,0xff0000);
		DrawLine(pPos.x, pPos.y, fulcrum_.x + cameraPos.x, fulcrum_.y + cameraPos.y, 0xffffff);
	}

	//if (player_.padNum_ == 1)
	//{
	//	DrawFormatStringF(0, 200, 0xffffff, "DiagonallyVecVec:x.%f,y.%f", player_.GetDiagonallyVecVec().x, player_.GetDiagonallyVecVec().y);
	//	DrawFormatStringF(0, 220, 0xffffff, "支点pos_:x%f:y%f", fulcrum_.x,fulcrum_.y);
	//	DrawFormatStringF(0, 240, 0xffffff, "angle_:%f", angle_);
	//}
}


void Wire::SwingPhase()
{
	auto gravity = 0.5f;
	//アングルをけってい
	angle_ = atan2f(player_.pos_.x - fulcrum_.x, player_.pos_.y - fulcrum_.y);
	v_ += gravity * sinf(angle_);
	vel_ = { -v_ * cosf(angle_),v_ * sinf(angle_) };
	Vector2DFloat vel = { vel_.x,vel_.y };
	if (_phase == &Wire::SwingPhase)
	{
		player_.pos_ += vel;		//velを加算
		player_.pos_ = fulcrum_ + (player_.pos_ - fulcrum_).Normalized() * length_;//長さを補正
	}
	if (player_.pos_.y <= fulcrum_.y + -150.0f)
	{
		SwingJump();
		player_.StartSwingJump();
	}
}

void Wire::StandbyPhase()
{
	fulcrum_ = player_.pos_;

}

void Wire::SwingJump()
{
	player_.movePow_.x = (vel_.x / 1.3f);
	player_.movePow_.y = (vel_.y / 1.3f);
	_phase = &Wire::EndSwingPhase;
}

void Wire::ChangeStandby()
{
	_phase = &Wire::StandbyPhase;

}

void Wire::EndSwingPhase()
{

}

void Wire:: AnchoringPhase()
{	
	fulcrum_pos = VAdd(fulcrum_pos, Scale_);
	fulcrum_.x = fulcrum_pos.x;
	fulcrum_.y = fulcrum_pos.y;
	if (!IsHitHook())
	{
		SetSwingPalam();
	}
}

void Wire::SetSwingPalam()
{	
	////支点を決定
	//fulcrum_ = player_.pos_;
	// fulcrum_pos = { fulcrum_.x,fulcrum_.y };
	//VECTOR moveVec = { player_.GetDiagonallyVecVec().x,player_.GetDiagonallyVecVec().y };
	//moveVec=VNorm(moveVec);
	//Scale_ = VScale(moveVec, 400.0f);
	//movedPos_ = VAdd(fulcrum_pos, Scale_);
	//fulcrum_.x = movedPos_.x;
	//fulcrum_.y = movedPos_.y;
	//vel_ = { 0.0f,0.0f };

	auto lVec = player_.pos_ - fulcrum_;	//支点→錘のベクトル(紐)

	length_ = lVec.Magnitude();							//紐の長さ
	vel_.x = player_.movePow_.x;	//初速度的な
	//ここでアングルの初期設定をする
	angle_ = atan2f(player_.pos_.x - fulcrum_.x, player_.pos_.y - fulcrum_.y);
	v_ = -2 * vel_.x * cosf(angle_);//x軸の速度

	if (player_.dir_LR_ == Player::DIR_LR::LEFT)
	{
		pow_ = 0.15f;
	}
	else
	{
		pow_ = -0.15f;
	}
	_phase = &Wire::SwingPhase;
	player_.StartSwing();
}

void Wire::SetAnchorPalam()
{
	fulcrum_ = player_.pos_;
	VECTOR moveVec = { player_.GetDiagonallyVecVec().x,
		(-35.0f)+17.0f};
	moveVec_.x = moveVec.x/2.0f;
	moveVec_.y = moveVec.y/2.0f;
	moveVec = VNorm(moveVec);
	Scale_ = VScale(moveVec, 30.0f);
	fulcrum_pos.x = fulcrum_.x;
	fulcrum_pos.y = fulcrum_.y;	
	_phase = &Wire::AnchoringPhase;
}

void Wire::StartSwingJump()
{
	player_.StartSwingJump();
	_phase = &Wire::EndSwingPhase;
}

void Wire::StartSwing()
{
	_phase = &Wire::SwingPhase;
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
	for (const auto& col : col)
	{	
		//"x":2720,"y":2400

		if (rayCast_.CheckCollision( col, fulcrum_))
		{		
			return false;
		}
	}
	return true;
}
