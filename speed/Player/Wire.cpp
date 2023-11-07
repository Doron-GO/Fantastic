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
		DrawFormatStringF(0, 200, 0xffffff, "支点pos_:x%f:y%f", fulcrum_.x,fulcrum_.y);
		DrawFormatStringF(0, 220, 0xffffff, "angle_:%f", angle_);

	}
}

void Wire::SwingPhase()
{
	auto coe = -gMag_ / length_;				//kの係数 kは傾きらしい
	//ルンゲ=クッタ法の計算
	auto k1 = delta_ * coe * sin(theta_);
	auto m1 = delta_ * omega_;

	auto k2 = delta_ * coe * sin(theta_ + m1 / 2.f);
	auto m2 = delta_ * (omega_ + k1 / 2.f);

	auto k3 = delta_ * coe * sin(theta_ + m2 / 2.f);
	auto m3 = delta_ * (omega_ + k2 / 2.f);

	auto k4 = delta_ * coe * sin(theta_ + m3);
	auto m4 = delta_ * (omega_ + k3);

	omega_ += (k1 + 2.f * k2 + 2.f * k3 + k4) / 6.0f;		//角速度の加算
	theta_ += (m1 + 2.f * m2 + 2.f * m3 + m4) / 6.0f;		//角度の加算

	//角度から錘の位置を導出
	player_.pos_ = fulcrum_ + length_ * cos(theta_) *  gravity_.Normalized() + length_ * sin(theta_) * yNorm_;


}

void Wire::TestSwingPhase()
{
	auto gravity = 0.5f;
	//アングルをけってい
	angle_ = atan2f(player_.pos_.x - fulcrum_.x, player_.pos_.y - fulcrum_.y);
	V += gravity * sinf(angle_);
	vel_ = { -V * cosf(angle_),V * sinf(angle_) };

	Vector2DFloat vel = { vel_.x,vel_.y };
	player_.pos_ += vel;		//velを加算
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
	

	//昔授業で作ったVectorを使っているせいでめんどくさいことになっている
}

void Wire::SetPalam()
{	
	//支点を決定
	fulcrum_ = player_.pos_;
	VECTOR pos = { fulcrum_.x,fulcrum_.y };
	VECTOR moveVec = { player_.GetDiagonallyVecVec().x,player_.GetDiagonallyVecVec().y };
	moveVec=VNorm(moveVec);
	Scale_ = VScale(moveVec, 400.0f);
	movedPos_ = VAdd(pos, Scale_);
	fulcrum_.x = movedPos_.x;
	fulcrum_.y = movedPos_.y;
	vel_ = { 0.0f,0.0f };

	//ルンゲクッタ法のための初期設定

	gravity_ = player_.GetMoveVec();
	gMag_ = gravity_.Magnitude();
	auto lVec = player_.pos_ - fulcrum_;	//支点→錘のベクトル(紐)

	length_ = lVec.Magnitude();							//紐の長さ

	auto grNorm = gravity_.Normalized();			//重力の正規化済みベクトル
	auto x = grNorm.Dot(lVec);					//重力ベクトルと錘から重力軸までのベクトルが交わる点と支点の大きさ

	yNorm_ = lVec - grNorm * x;										//錘から重力軸までのベクトル
	auto y = yNorm_.Magnitude();								//錘から重力軸までの距離
	yNorm_.Normalize();												//錘から重力軸までの正規化済みベクトル

	theta_ = atan2f(y, x);											//支点の近接二辺のそれぞれの大きさを使用して角度を出す
	omega_ = -(abs(player_.GetMovePow().x/2.0f));					//角速度は0で初期化  でも今はプレイヤーの

	

	//ここでアングルの初期設定をする
	angle_ = atan2f(player_.pos_.x - fulcrum_.x, player_.pos_.y - fulcrum_.y);
	V = -2 * vel_.x * cosf(angle_);//x軸の速度

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
