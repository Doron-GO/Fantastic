#include <string>
#include "../Application.h"
#include "../common/Flame.h"
#include "../Utility/AsoUtility.h"
#include "../Input/Keybord.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "Common/AnimationController.h"
#include "Common/Transform.h"
#include "Common/Capsule.h"
#include "Common/Collider.h"
#include "Player.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

Player::Player(void) : resourceManager_(ResourceManager::GetInstance())
{
	phase_ = &Player::UpdateGround;
	VECTOR gravityDir_ = { 0.0f,-1.0f,0.0f };
	gravity_ = VScale(gravityDir_, 3500.0f);
	gMag_ = Magnitude(gravity_);

}

Player::~Player(void)
{
}

void Player::Init(void)
{
		//controller_ = std::make_unique<Keybord>();
	

	//モデル情報を格納
	transform_.SetModel(
		resourceManager_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	//モデルの設定
	//transform_.scl = { 0.5f,0.5f ,0.5f };
	transform_.scl = { 1.0f,1.0f ,1.0f };
	transform_.pos = { 120.0f, 120.0f, 100.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(0.0f), 0.0f });
	transform_.Update();
	
	capsule_ = std::make_unique<Capsule>(&transform_);
	capsule_->SetRelativePosTop({ 0.0f,65.0f,0.0f });
	capsule_->SetRelativePosDown({ 0.0f,40.0f,0.0f });
	capsule_->SetRadius(20.0f);

	endPos_ = { 4200.0f,2200.0f,1200.0f };


	// アニメーションの設定
	AnimationInit();

}

void Player::Update(float delta, VECTOR pos)
{	
	gravityNorm_ = Normalized(gravity_);		//重力の正規化ベクトル
	swingGravityNorm = Normalized(swingGravity);

	//controller_->Update();
	(this->*phase_)(delta);
	//CalcGravityPow();
	Collision();
	ProcessMove();
	ProcessJump();

	transform_.Update();
	animationController_->Update();

}

void Player::UpdatePendulum(float delta)
{

	Swing(delta);

	//transform_.Update();
}

void Player::UpdateGround(float delta)
{

	//衝突判定
	transform_.quaRot = Quaternion{ 1.0f,0.0f,0.0f,0.0f };
	transform_.quaRot = transform_.quaRot.Mult(pendulumRotY_);

}

bool Player::Start(VECTOR pos ,VECTOR end)
{
	//transform_.pos = pos;
	//元のやつ
	endPos_ = end;
	stringV_ = VSub(transform_.pos,endPos_);//支点から錘へののベクトル
	length_ = Magnitude(stringV_);				//ヒモの長さ
	auto x = Dot(gravityNorm_, stringV_);//重力ベクトルと錘から重力軸までのベクトルが交わる点と支点の大きさ
	yNorm_ = stringV_ - gravityNorm_ * x;//錘から重力軸までのベクトル
	auto y = Magnitude(yNorm_);					//錘から重力軸までの正規化済みベクトル
	yNorm_ = Normalized(yNorm_);
	theta_ = atan2f(y, x);				//支点の近接二辺のそれぞれの大きさを使用して角度を出す


	//試しに変えている
	auto jk = Normalized(stringV_);
	swingGravity = VECTOR{ 1.0f,-5.0f,-0.2f };
	swingGravityNorm = Normalized(swingGravity);
	//swingGravityNorm = VECTOR{ 1.0f,jk.y,0.2f };


	auto x2 = Dot(swingGravityNorm, stringV_);//重力ベクトルと錘から重力軸までのベクトルが交わる点と支点の大きさ
	swingYnorm_ = stringV_ - swingGravityNorm * x2;//錘から重力軸までのベクトル
	auto y2 = Magnitude(swingYnorm_);				//錘から重力軸までの正規化済みベクトル
	swingYnorm_=Normalized(swingYnorm_);
	
	theta_ = atan2f(y2, x2);				//支点の近接二辺のそれぞれの大きさを使用して角度を出す
	auto gravity_ = VScale(swingGravityNorm, 3500.0f);
	
	gMag_ = Magnitude(gravity_);
	omega_ = -1.0f;									//角速度は0で初期化

	isStarted_ = true;
	transform_.Update();

	return true;
}

void Player::SetFollowTarget(Camera* target)
{
	camera_= target;

}

void Player::AddCollider(Collider* collider)
{
	mColliders.emplace_back(collider);
}

void Player::Swing(float delta)
{
	//if (!isStarted_)
	//{
	//	return;
	//}

	auto coe = -gMag_ / length_;				//kの係数

	//ルンゲ=クッタ法の計算
	auto k1a = delta * coe * sin(theta_);
	auto m1a = delta * omega_;

	auto k2a = delta * coe * sin(theta_ + m1a / 2.0f);
	auto m2a = delta * (omega_ + k1a / 2.0f);

	auto k3a = delta * coe * sin(theta_ + m2a / 2.0f);
	auto m3a = delta * (omega_ + k2a / 2.0f);

	auto k4a = delta * coe * sin(theta_ + m3a);
	auto m4a = delta * (omega_ + k3a);

	omega_ += (k1a + 2.0f * k2a + 2.0f * k3a + k4a) / 6.0f;		//角速度の加算
	theta_ += (m1a + 2.0f * m2a + 2.0f * m3a + m4a) / 6.0f;		//ｚ角度の加算

	auto stv = Normalized(stringV_);
	auto stv2 = Normalized(gravity_);
	auto stv3 = Normalized(swingGravityNorm);
	stv3 = VScale(stv, 3500.0f);
	transform_.pos = endPos_ + length_ * cos(theta_) * Normalized(swingGravityNorm) + length_ * sin(theta_) * swingYnorm_;
	//transform_.pos = endPos_ + length_ * cos(theta_) * Normalized(gravity_) + length_ * sin(theta_) * yNorm_;


	//stringV_ = VSub(transform_.pos, endPos_);
	//auto stNorm = Normalized(stringV_);
	//auto dot = Dot(movePow_, stNorm);
	//auto stDot1= dot* stNorm;
	//auto stDot2 = stDot1 * (-2.0f);
	//movePow_ = stDot2;
	//transform_.pos = movedPos_;

	//カメラ向いている方向に対しての前後左右の重力を傾ける
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();
	//重力方向の操作
	if (CheckHitKey(KEY_INPUT_D))
	{
		swingGravity = VAdd(swingGravity, VScale(transform_.GetUp(), 0.02f));
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		swingGravity = VAdd(swingGravity, VScale(transform_.GetDown(), 0.02f));
	}
	if (CheckHitKey(KEY_INPUT_P))
	{
		//phase_ = &Player::Flying;
		dir_ = transform_.GetForward();
	}

}

void Player::Draw(void)
{
	// モデルの描画
	MV1DrawModel(transform_.modelId);

	auto fw = VAdd(transform_.pos, VScale(transform_.GetForward(), 60.0f));
	DrawLine3D(transform_.pos, fw, 0x00ff00);
	auto pa = VAdd(transform_.pos, VScale(quaPaddir.GetForward(), 60.0f));
	DrawLine3D(transform_.pos, pa, 0x00ff00);

	auto  lol = VECTOR{ 1.0f,1400.0f,0.2f };
	//swingGravity = VECTOR{lol.x,-0.4f,lol.z };

	auto kj = VAdd(endPos_, VScale(swingGravity, 600.0f));
	auto kj2 = VAdd(transform_.pos, VScale(swingGravity, 600.0f));
	DrawLine3D(endPos_, kj, 0x00ff00);
	DrawLine3D(transform_.pos, kj2, 0x00ff00);

	DrawFormatString(50, 530, 0xffffff, "mJumpPow=%f", jumpPow_.y);
	DrawFormatString(50, 470, 0xffffff, "step:%f", stepJump_);
	DrawFormatString(50, 550, 0xffffff, "movePow_:%f,%f,%f", movePow_.x, movePow_.y, movePow_.z);
	VECTOR klk = { 4200.0f,2200.0f,1200.0f };

	DrawSphere3D(klk, 60.0f, 20.0f, 0xff0000, 0xffffff, true);
	capsule_->Draw();
	//デバッグ表示
	DrawDebug();
}

void Player::Release(void)
{
	//モデルの解放
	MV1DeleteModel(transform_.modelId);
}

Transform* Player::GetTransform(void)
{
	//モデル基本情報の取得
	return &transform_;
}

void Player::DrawDebug(void)
{
	//デバッグ表示
	DrawFormatString(0, 32, 0xffffff, "プレイヤー座標:%f,%f,%f",
		transform_.pos.x, transform_.pos.y, transform_.pos.z);
}

void Player::AnimationInit(void)
{
	std::string path = Application::PATH_MODEL + "Player/";

	animationController_ = new AnimationController(transform_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE, path + "Idle.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::RUN, path + "FastRun.mv1", 30.0f);
	animationController_->Add((int)ANIM_TYPE::JUMP, path + "Jump.mv1", 24.0f);
	animationController_->Add((int)ANIM_TYPE::SWING, path + "Start_Swing.mv1", 20.0f);
	animationController_->Play((int)ANIM_TYPE::IDLE);

}

void Player::ProcessMove()
{
	//移動量をゼロに
	movePow_ = VECTOR{ 0.0f,0.0f,0.0f };

	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();

	float rotRad = 0;
	float speed = 20.0f;

	//移動方向
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	auto& ins = InputManager::GetInstance();
	const auto& stick = InputManager::GetInstance().GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	stick.AKeyLX;	// 出力値（－1000～1000）左～右＋
	stick.AKeyLY;	// 出力値（－1000～1000）上～下＋
	// ほしい値は－1～1なので1000で割る
	float padX = stick.AKeyLX / 1000.0f;
	// 上下反転させる
	float padY = stick.AKeyLY * -1.0f / 1000.0f;

	// Y軸なし方向ベクトル
	VECTOR padDir = {};
	padDir = { padX,0.0f,padY };

	float  rad = atan2f(padX,padY );
	// 正規化
	padDir = AsoUtility::VNormalize(padDir);
	cameraRot =cameraRot.Normalized();

	//アナログスティックの倒している方向のQuaternion
	Quaternion quaPadDir = Quaternion::AngleAxis(rad,cameraRot.GetUp());
	//カメラの回転と合成したクォータニオン
	Quaternion goal2 = Quaternion::Mult(cameraRot, quaPadDir);


	if (!AsoUtility::EqualsVZero(padDir))
	{
		if (!isJump_)//ジャンプ状態じゃなくスティック入力があれば
		{	//アニメーションをRUNに
			animationController_->Play(static_cast<int>(ANIM_TYPE::RUN));
		}
		movePow_ = VScale(goal2.GetForward(), speed);	//移動量　= 方向　×　スピード
		goalQuaRot_ = Quaternion::LookRotation(goal2.GetForward());
	}
	else
	{
		if (!isJump_)
		{
			animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
		}
	}

	if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
	{
		Start(transform_.pos,endPos_);
		phase_ = &Player::UpdatePendulum;
	}

	Rotate();	//回転(ゴールに対して、到達させたい)


}

void Player::ProcessJump()
{
	auto& ins = InputManager::GetInstance();

	bool isHit = ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN);
	if (isHit)
	{
		if (!isJump_)
		{
			animationController_->Play(static_cast<int>(ANIM_TYPE::JUMP), true, 2.0f, 17.0f);

			animationController_->SetEndLoop(12.0f, 17.0f, 3.0f);
		}
		isJump_ = true;
	}
	else
	{
		//stepJump_ = TIME_JUMP_IN;
	}

	if (isJump_)
	{
		stepJump_ += SceneManager::GetInstance().GetDeltaTime();
		if (stepJump_ < TIME_JUMP_IN)
		{
			jumpPow_ = VScale(VECTOR{ 0.0f,1.0f,0.0f }, POW_JUMP);
		}
	}
	if (!isHit)
	{
		stepJump_ = TIME_JUMP_IN;
	}

}

void Player::CalcGravityPow()
{
	//重力方向
	VECTOR dirGravity = gravityNorm_;

	//重力の強さ
	float gravityPow = 25.0f;

	//重力				方向×力
	VECTOR gravity = VScale(dirGravity, gravityPow);

	//上下の移動量に重力を加える
	jumpPow_ = VAdd(jumpPow_, gravity);

	//dotは内積
	float dot = VDot(dirGravity, jumpPow_);
	if (dot >= 0.0f)
	{
		//重力方向と反対方向でなければ，ジャンプ食を打ちける
		jumpPow_ = gravity;
	}

}

void Player::Collision()
{
		//現在座標を起点に移動後座標を決める
	movedPos_ = VAdd(transform_.pos, movePow_);
	CollisionCupsule();
	CollisionGravity();
	CollisionSphere();
	transform_.pos = movedPos_;

}

void Player::CollisionCupsule(void)
{	//カプセルを移動
	Transform trans = Transform(transform_);
	trans.pos = movedPos_;
	trans.Update();
	Capsule cap = capsule_->Copy(&trans);

	for (auto c : mColliders)
	{
		auto hits = MV1CollCheck_Capsule(c->modelId_,-1,
			cap.GetPosTop(),
			cap.GetPosDown(),
			cap.GetRadius()
		);


		//検出したポリゴンとの衝突情報を消す
		MV1CollResultPolyDimTerminate(hits);

		for (int i = 0; i < hits.HitNum; i++)
		{
			//1つ1つの衝突情報を取得する
			auto hit = hits.Dim[i];

			for (int tryCnt = 0; tryCnt < 10; tryCnt++)
			{
				//処理負荷を減らすために、１ポリゴンと
				//カプセルの衝突判定しする
				int pHit = HitCheck_Capsule_Triangle(
					cap.GetPosTop(),
					cap.GetPosDown(),
					cap.GetRadius(),
					hit.Position[0],//3つの頂点
					hit.Position[1],
					hit.Position[2]
				);

				if (pHit)
				{
					//衝突している
					float pow = 4.0f; //ちょっとだけ動かす
					hit.Normal;		  //衝突したポリゴンの法線
					int p = MV1GetFrameParent(c->modelId_, hit.FrameIndex);
					movedPos_ = VAdd(movedPos_, VScale(hit.Normal, pow));
					//カプセルを移動させる
					trans.pos = movedPos_;
					trans.Update();
					cap = capsule_->Copy(&trans);
					continue;
				}
				//衝突していないのでループ中断
			}
		}

	}

}

void Player::CollisionSphere(void)
{
	for (auto c : mColliders)
	{
		//地面(ステージモデル)との衝突				　
		auto hits = MV1CollCheck_Sphere(c->modelId_, -1, transform_.pos, 50.0f);
		//検出したポリゴンとの衝突情報を消す
		MV1CollResultPolyDimTerminate(hits);

		for (int i = 0; i < hits.HitNum; i++)
		{
			break;
		}

	}
}

void Player::CollisionGravity(void)
{
	movedPos_ = VAdd(movedPos_, jumpPow_);

	VECTOR dirGravity = gravityNorm_;

	VECTOR dirUpGravity = -gravityNorm_;

	//足元　movePow
	float CheckPow = 10.0f;

	//足元のちょっと上の座標
	auto gravHitUp = VAdd(movedPos_, VScale(dirUpGravity, 25.0f));
	gravHitUp = VAdd(gravHitUp, VScale(dirUpGravity, CheckPow * 2.0f));

	//始点
	//足元のちょっと下の座標
	auto gravHitDown = VAdd(movedPos_, VScale(dirGravity, CheckPow));

	for (auto c : mColliders)
	{
		//地面(ステージモデル)との衝突				　↓始点		↓終点
		auto hit = MV1CollCheck_Line(c->modelId_, -1, gravHitUp, gravHitDown);
		//落下時だけ足元の当たり判定
		jumpDot_ = VDot(dirGravity, VNorm(jumpPow_));

		if (hit.HitFlag > 0 && jumpDot_ > 0.9f)
		{

			//傾斜判定用に衝突情報をとっておく
			//mHitNormal = hit.Normal;
			//mHitPos = hit.HitPosition;

			//衝突している
			float dis = 2.0f;
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, dis));
			//抵抗力の代わりに
			jumpPow_ = AsoUtility::VECTOR_ZERO;

			if (isJump_)
			{
				////着地モーション
				//mAnimationController->Play(
				//	static_cast<int>(ANIM_TYPE::JUMP), false,
				//	29.0f, 45.0f, false, true);
			}
			//ジャンプ終了
			isJump_ = false;
			stepJump_ = 0.0f;
		}

	}


}

void Player::SetGoalRotate(float rotRad)
{
	VECTOR cameraAngle =
		SceneManager::GetInstance().GetCamera()->GetAngles();

	Quaternion axis =
		Quaternion::AngleAxis(cameraAngle.y + rotRad, AsoUtility::AXIS_Y);

	//現在設定されている回転との角度差を求める
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	//しきい値
	if (angleDiff > 0.1)
	{
		//mStepRot = TIME_ROT;
	}

	//到達したい回転を決める
	goalQuaRot_ = axis;


}

void Player::SetGoalRotate2(VECTOR vec)
{
	Quaternion axis =
		Quaternion::AngleAxis(vec.y, AsoUtility::AXIS_Y);

}

void Player::Rotate(void)
{
	pendulumRotY_ = Quaternion::Slerp(pendulumRotY_, goalQuaRot_, 0.2);
}

float Player::Magnitude(VECTOR pos) const
{
	return sqrtf(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);
}

float Player::Dot(const VECTOR& a, const VECTOR& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

VECTOR Player::Normalized(VECTOR& v)
{
	float mag = Magnitude(v);
	return VECTOR{ v.x / mag, v.y / mag, v.z / mag };
}

VECTOR operator*(const VECTOR& v, float scale)
{
	return{ v.x * scale, v.y * scale ,v.z * scale };
}

VECTOR operator*(float scale, const VECTOR& v)
{
	return{ v.x * scale, v.y * scale ,v.z * scale };
}

VECTOR operator+(const VECTOR& va, const VECTOR vb)
{
	return{ va.x + vb.x, va.y + vb.y ,va.z + vb.z };
}

VECTOR operator-(const VECTOR& va, const VECTOR vb)
{
	return { va.x - vb.x, va.y - vb.y,va.z - vb.z };
}

VECTOR operator-(const VECTOR& v)
{
	return { -v.x,-v.y ,-v.z };
}
