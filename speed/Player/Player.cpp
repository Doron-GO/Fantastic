#include<DxLib.h>
#include<string>
#include "Player.h"
#include"Wire.h"
#include"../Object/Manager/ImageMng.h"
#include"../_debug/_DebugConOut.h"
#include"../_debug/_DebugDispOut.h"

Player::Player(int playerNum):dir_LR_(DIR_LR::RIGHT),phase_(PHASE::FALL),
aliveFlag_(true), padNum_(playerNum)
{
	AnchoringFlag_ = false;

}

Player::~Player()
{
}

void Player::Init(ColList colList, ColList wallColList, ColList wireColList)
{
	pos_ = { 404.0f-padNum_*-20.0f,0.0f };

	center_ = { 0.0f,12.0f };
	grndColList_ = colList;
	wallcolList_ = wallColList;

	//ベストな方法ではないかもだけど、Padナンバーを使ってactlistを変える
	 char num = '0'+padNum_;
	 std::string act = "Src/Img/act";//string文字列を作る
	 act += num;						 //	文字列を連結
	 act +=".list";					 //	文字列を連結

	lpAnimMng.LoadAnime(act.c_str());

	std::string objID = "Player";
	objID += num;
	animeStr_.objID_ = objID.c_str();

	lpAnimMng.SetAnime(animeStr_, "Idle");


	moveVec_ = { 0.0f,0.0f };
	movePow_ = { 0.0f,0.0f };
	up_ = { 0.0f,-50.0f };
	_phase = &Player::FallPhase;
	
	wire_ = std::make_unique<Wire>(*this, wireColList);
}

void Player::Update(Input& input)
{
	lpAnimMng.UpdateAnime(animeStr_);
	input_.Update(padNum_);
	
	Anchoring(input_);

	Move(input_);
	wire_->Update();

	(this->*_phase)(input_);

	if (!(_phase == &Player::SwingPhese))
	{
		pos_.y += movePow_.y;

		//壁に当たっていたら横移動させない
		if (CollisionVec(moveVec_))
		{
			pos_.x += movePow_.x;
		}
	}

}

void Player::Draw(Vector2DFloat cameraPos)
{

	auto pos = pos_ + cameraPos;

	//デバッグ用の実際のキャラの座標を表示

	DrawRotaGraph2F(pos_.x, pos_.y ,
			24.0f, 35.0f,
			1.5, 0.0,
			lpImageMng.GetID(animeStr_.imgKey_)[(*animeStr_.animID_)[GraphHD]],
			true, static_cast<int>(dir_LR_), 0);

	//見せかけのキャラクター描画
	DrawCircle(pos.x, pos.y, 5.0f, 0xffffff);
	DrawCircle(cameraPos.x, cameraPos.y, 5.0f, 0x00ff00);
	DrawRotaGraph2F(pos.x, pos.y,
		24.0f, 35.0f,
		1.5, 0.0,
		lpImageMng.GetID(animeStr_.imgKey_)[(*animeStr_.animID_)[GraphHD]],
		true, static_cast<int>(dir_LR_), 0);
	if (padNum_ == 1)
	{
		DrawFormatStringF(0, 0, 0xffffff, "movePow_(x:%f,y%f)", movePow_.x, movePow_.y);
		DrawFormatStringF(0, 20, 0xffffff, "moveVec_(x:%f,y%f)", moveVec_.x, moveVec_.y);
		DrawFormatStringF(0, 40, 0xffffff, "pos_(x:%f,y%f)", pos_.x, pos_.y);
		DebugPhaseCheck();
		DrawString(0, 120, now_.c_str(), 0xffffff);

		DrawLine(pos.x-center_.x, pos.y - center_.y,
			pos.x + center_.x, pos.y + center_.y, 0xff0000);
	if (!CollisionVec(moveVec_))
	{
		DrawString(0, 100, "壁に当たった", 0xffffff);
	}
	if (!CollisionDown())
	{
		DrawString(0, 80, "床に当たった", 0xffffff);
	}
	if (!ColWallGrab(moveVec_))
	{
		DrawString(0, 60, "ジャンプ壁に当たった", test);
	}

	}
	char num= '0' + padNum_;
	std::string pp ="死んだ";
	std::string p = "Player";
	p += num;
	p += pp;
	if (!aliveFlag_)
	{
		DrawStringF(0, 160+(padNum_*10), p.c_str() , 0xffffff);
	}

	DrawFormatStringF(0, 200 + padNum_*70+70, 0xffffff, padNum_+ "pos_(x:%f,y%f)", pos_.x, pos_.y);


	//DrawString(0,160,"%s", )
	Vector2DFloat rayCenter = { pos - center_ };
	Vector2DFloat diagonallyVec = { moveVec_.x,slideY_ };


	Vector2DFloat moveVec = { 0.0f,27.0f };

	DrawLine(rayCenter.x, rayCenter.y,
		rayCenter.x + moveVec.x, rayCenter.y + moveVec.y, 0x00ff00);

	wire_->Draw(cameraPos);
}

const Vector2DFloat Player::GetPos()
{
	return pos_;
}

const Vector2DFloat Player::GetDiagonallyVecVec()
{
	return diagonallyVec_;
}

const Vector2DFloat Player::GetMoveVec()
{
	return moveVec_;
}

const Vector2DFloat  Player::GetMovePow()
{
	return movePow_;
}

void Player::StartSwingJump()
{
	_phase = &Player::SwingJumpPhese;
}

void Player::DebugPhaseCheck()
{
	switch (phase_)
	{
	case Player::PHASE::FALL:
		now_ = "FallPhase";
		break;
	case Player::PHASE::JUMP:
		now_ = "JumpPhase";
		break;
	case Player::PHASE::MOVE:
		now_ = "MovePhase";
		break;
	case Player::PHASE::WALLGRAB:
		now_ = "WallSlidePhase";
		break;
	case Player::PHASE::WALLJUMP:
		now_ = "WallJumpPhase";
		break;
	case Player::PHASE::SWING:
		now_ = "SwingPhase";
		break;
	case Player::PHASE::SWINGJUMP:
		now_ = "SwingFallPhase";
		break;
	}
}

void Player::IdlePhase(Input& input)
{

}

void Player::MovePhase(Input& input)
{
	phase_ = Player::PHASE::MOVE;
	//Move(input);
	//もし床がなかったらフォールにする
	Vector2DFloat movevec = { 0.0f,15.2f };
	if (CollisionDown())
	{
		_phase = &Player::FallPhase;
	}
	Jump(input);
}

void Player::JumpPhese(Input& input)
{
	phase_ = Player::PHASE::JUMP;

	lpAnimMng.SetAnime(animeStr_, "Jump");	

	//ジャンプ高度が最大に達したもしくは、頭をぶつけたら
	if((movePow_.y<=-6.0f)||!(CollisionVec(up_)))
	{
		//ｙの移動量0にしてフォールを呼ぶ
		//movePow_.y = 0.0f;
		_phase = &Player::FallPhase;
	}
	//でなければyの移動量を加算する
	else
	{
		movePow_.y += -0.2f;
		movePow_.y += -0.3f;
	}
}

void Player::FallPhase(Input& input)
{
	phase_ = Player::PHASE::FALL;

	lpAnimMng.SetAnime(animeStr_, "Fall");	
	Vector2DFloat movecec = { 0.0f,movePow_.y };
	//床と当たっていなかったら
	if (!CollisionVec(movecec))
	{
		movePow_.y = 0.0f;
		_phase = &Player::MovePhase;

	}
	else
	{
		movePow_.y += 0.2f;
	}
	//落下速度が一定を超えたら決まった値にする
	if (movePow_.y >= 8.0f)
	{
		movePow_.y = 8.0f;
	}
}

void Player::WallGrabPhese(Input& input)
{
	phase_ = Player::PHASE::WALLGRAB;

	diagonallyVec_ = { moveVec_.x,slideY_ };
	Jump(input);

	//壁にくっついていなかったらフォール状態に移行する
	if (!(_phase == &Player::WallJumpPhese))
	{
		if (!IsWall())
		{
			moveVec_ = -(moveVec_);
			_phase = &Player::FallPhase;
		}
	}
	Vector2DFloat movecec = { 0.0f,movePow_.y };

	//壁つかまり中に頭を打ったらそれ以上は上がらない
	if (!CollisionVec(up_))
	{
		movePow_.y = 0.0f;
	}
	//もし地面に足がついたら
	if ( !CollisionVec(movecec))
	{
		movePow_.y = 0.0f;
		_phase = &Player::MovePhase;
	}
	else//でなければ上に上がる
	{
		lpAnimMng.SetAnime(animeStr_, "WallSlide");
		if (movePow_.y <= 4.0f)
		{
			movePow_.y += 0.1f;
		}
	}
}

void Player::WallJumpPhese(Input& input)
{
	phase_ = Player::PHASE::WALLJUMP;

	lpAnimMng.SetAnime(animeStr_, "Jump");
	diagonallyVec_ = { moveVec_.x,slideY_ };
	Vector2DFloat movevec = { 0.0f,-40.0f };
	//ジャンプ高度が最大に達したもしくは、地面に接地したら
	if ((movePow_.y <= -5.0f) || !(CollisionVec(movevec)))
	{
		_phase = &Player::FallPhase;
	}
	//でなければyの移動量を加算する
	else
	{
		movePow_.y += -0.2f;
		movePow_.y += -0.3f;
		movePow_.x = (moveVec_.x / 2.0f);
	}
	if (IsWall())
	{
		movePow_.x = 0.0f;
		_phase = &Player::WallGrabPhese;
	}
}

void Player::SwingPhese(Input& input)
{
	phase_ = Player::PHASE::SWING;

	//壁に当たったら加速度を０にする 1:自分の前方 2:上斜め前 
	diagonallyVec_ = { moveVec_.x,slideY_ };
	Vector2DFloat tt = { movePow_.x,0.0f };
	if (!ColWallGrab(tt) || !ColWallGrab(diagonallyVec_))
	{
		movePow_.y =0.0f;
		movePow_.x = 0.0f;
		_phase = &Player::WallGrabPhese;
		wire_->ChangeStandby();
	}
	if (!CollisionVec(moveVec_) || !CollisionVec(diagonallyVec_))
	{
		movePow_.x = 0.0f;
		movePow_.y= 0.0f;
		_phase = &Player::FallPhase;
		wire_->ChangeStandby();
	}

	Vector2DFloat up = { 0.0f,-60.0f };
	//地面もしくは天井に当たったらスイングをやめる
	if (!CollisionVec(up))
	{
		wire_->ChangeStandby();
		_phase = &Player::MovePhase;
		movePow_.y = 0.0f;

	}
	if (CollisionDown())
	{
		if (_phase == &Player::SwingPhese)
		{	
			if (input.IsPrassed("hook"))
			{
				lpAnimMng.SetAnime(animeStr_, "Jump");
				movePow_.y = 0.0f;
				wire_->Pump();
			}
			else
			{
				wire_->SwingJump();
				StartSwingJump();

			}
		}
	}
	if (!CollisionDown())
	{			
		_phase = &Player::MovePhase;
		wire_->ChangeStandby();
		movePow_.y = 0.0f;
		wire_->EndSwing();
	}
}

void Player::SwingJumpPhese(Input& input)
{
	phase_ = Player::PHASE::SWINGJUMP;

	lpAnimMng.SetAnime(animeStr_, "Fall");
	//落下速度が一定を超えたら決まった値にする
	if (movePow_.y >= 8.0f)
	{
		movePow_.y = 8.0f;
	}

	Vector2DFloat movecec = { 0.0f,movePow_.y };
	Vector2DFloat up = { 0.0f,-50.0f };

	//床と当たっていなかったら
	if (!CollisionVec(movecec)|| !CollisionVec(up))
	{
		movePow_.y = 0.0f;
		wire_->ChangeStandby();
		_phase = &Player::MovePhase;

	}
	else
	{
		movePow_.y += 0.2f;
	}

}

void Player::AnchoringPhese(Input& input)
{

}

void Player::MoveColision()
{
	diagonallyVec_ = { moveVec_.x,slideY_ };

	//壁つかまり状態の判定
	if (!(_phase == &Player::MovePhase) && !(_phase == &Player::WallGrabPhese))
	{
		if (IsWall() && CollisionDown())
		{
			if (!(_phase == &Player::FallPhase))
			{
				movePow_.y = -abs(movePow_.x / 1.5f);
			}
			movePow_.x = 0.0f;
			_phase = &Player::WallGrabPhese;
		}
	}

	//壁に当たったら加速度を０にする 1:自分の前方 2:上斜め前 
	diagonallyVec_ = { moveVec_.x,slideY_ };
	if (!CollisionVec(moveVec_) || !CollisionVec(diagonallyVec_))
	{
		movePow_.x = 0.0f;
	}

	//背中から壁に当たったら
	Vector2DFloat backVec = { -(moveVec_.x / 2.0f),0.0f };
	Vector2DFloat backDiagonallyVec_ = { backVec.x,slideY_ };//背中からの斜めバージョン

	if (!CollisionVec(backVec))
	{
		movePow_.x = -(backVec.x / 11.0f);
	}
	if (!CollisionVec(backDiagonallyVec_))
	{
		movePow_.x = -(backVec.x / 11.0f);
	}
	//壁ジャンプのできる壁と背中側との当たり判定
	if (!ColWallGrab(backVec))
	{
		movePow_.x = -(backVec.x / 11.0f);
	}
	if (!ColWallGrab(backVec))
	{
		movePow_.x = -(backVec.x / 11.0f);
	}

}

bool Player::CollisionDown()
{
	Vector2DFloat rayCenter = { pos_};
	Vector2DFloat moveVec = { 0.0f,20.0f };
	for (const auto& col : grndColList_)
	{
		Raycast::Ray ray = { rayCenter,moveVec};

		if (rayCast_.CheckCollision(ray, col,pos_))
		{
			//TRACE("当たった\n");
			return false;
		}
	}
	return true;
}

bool Player::CollisionVec(Vector2DFloat movevec)
{
	Vector2DFloat rayCenter = { pos_ };
	//実際の当たり判定レイの描画
	
	//_dbgDrawLine(
	//	rayCenter.x, rayCenter.y,
	//	rayCenter.x + movevec.x, rayCenter.y + movevec.y,
	//	0x00ff00
	//);

	for (const auto& col : grndColList_)
	{
		Raycast::Ray ray = { rayCenter,movevec };

		if (rayCast_.CheckCollision(ray, col, pos_ ))
		{
			return false;
		}
	}
	return true;
}

bool Player::ColWallGrab(Vector2DFloat movevec)
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

bool Player::IsWall()
{
	return (!ColWallGrab(moveVec_) || !ColWallGrab(diagonallyVec_));
}

void Player::Dead()
{
	aliveFlag_ = false;
}

void Player::Alive()
{
	aliveFlag_ = true;
}

bool Player::IsAlive()
{

	return aliveFlag_;
}


void Player::Move(Input& input)
{
	float speed = 0.2f;
	//右とは左キーが押されていないとき
	if (!input.IsPrassed("right")&&!input.IsPrassed("left"))
	{
		if (_phase == &Player::MovePhase)
		{
			//移動量が0.1より大きかったら
			if (movePow_.x >= 0.1f)
			{
				movePow_.x -= speed;
			}
			//移動量が-0.1より小さかったら
			if (movePow_.x <= -0.1f)
			{
				movePow_.x += speed;
			}
			if (dir_LR_ == DIR_LR::RIGHT)
			{
				if (0.40f >= movePow_.x && movePow_.x >= 0.02f)
				{
					movePow_.x = 0.0f;
				}
			}
			if (dir_LR_ == DIR_LR::LEFT)
			{
				if (-0.40f <= movePow_.x && movePow_.x <= -0.02f)
				{
					movePow_.x = 0.0f;
				}
			}
		}
		//ジャンプアニメーション中じゃなかったら
		if (_phase == &Player::MovePhase) { lpAnimMng.SetAnime(animeStr_, "Idle"); }
	}
	else 
	{
		//ジャンプアニメーション中じゃなかったら
		if (_phase == &Player::MovePhase) { lpAnimMng.SetAnime(animeStr_, "Run");}
	}

	//スライディングボダンが押されていない時
	if (!input.IsPrassed("c"))
	{
		if (!(_phase == &Player::WallJumpPhese)&& !(_phase == &Player::SwingPhese))
		{
			//右キー
			if (input.IsPrassed("right"))
			{
				dir_LR_ = DIR_LR::RIGHT;
				movePow_.x += 0.2f;
				moveVec_ = { 16.0f,0.0f };

			}
			//左キー
			if (input.IsPrassed("left"))
			{
				dir_LR_ = DIR_LR::LEFT;
				movePow_.x -= 0.2f;
				moveVec_ = { -16.0f,0.0f };

			}
		}

		if(_phase == &Player::SwingJumpPhese)
		{
			//移動速度が一定を超えると最大速度に固定する
			if (movePow_.x <= -12.0f) { movePow_.x = -12.0f;}
			else if (movePow_.x >= 12.0f) { movePow_.x = 12.0f;}
		}
		else
		{
			//移動速度が一定を超えると最大速度に固定する
			if (movePow_.x <= -8.0f) { movePow_.x = -8.0f; }
			else if (movePow_.x >= 8.0f) { movePow_.x = 8.0f; }
		}
	}
	//壁に当たったらのどの処理をまとめている
	MoveColision();

	//落下中じゃないとき
	if (!(_phase == &Player::FallPhase)&&!(_phase == &Player::JumpPhese) &&
		!(_phase == &Player::SwingJumpPhese))
	{
		//スライディングボタンが押されていたら
		if (input.IsPrassed("c"))
		{
			lpAnimMng.SetAnime(animeStr_, "Slide");
			//スライディング中は減速する
			if (movePow_.x >= 0.0f) 
			{
				movePow_.x -= 0.1f;
			}
			if (movePow_.x <= 0.0f) 
			{
				movePow_.x += 0.1f;
			}
			
			slideY_ = -15.0f;
		}
		else{ slideY_ = -35.0f; }
	}
}

void Player::Anchoring(Input& input)
{
	if (!(_phase == &Player::MovePhase))
	{
		if (input.IsTriggerd("hook"))
		{
			if (CollisionDown() && CollisionVec(moveVec_) && CollisionVec(diagonallyVec_))
			{
				if (!(_phase == &Player::SwingPhese) && !(_phase == &Player::WallGrabPhese))
				{
					lpAnimMng.SetAnime(animeStr_, "Jump");

					wire_->SetAnchorPalam();
					AnchoringFlag_ = true;
				}
			}
		}
	}
	if (!(_phase == &Player::SwingPhese))
	{
		if (AnchoringFlag_)
		{
			if (input.IsPrassed("hook"))
			{
				lpAnimMng.SetAnime(animeStr_, "Jump");

			}
			else
			{
				_phase = &Player::MovePhase;//これのせいでジャンプ中にフックボタン連打でふんわりジャンプする
				AnchoringFlag_ = false;
				wire_->ChangeStandby();

			}
		}
	}
}

void Player::StartSwing()
{
	_phase = &Player::SwingPhese;
	AnchoringFlag_ = false;
}

void Player::Jump(Input& input)
{
	//上キーを押したとき
	if (input.IsTriggerd("jump"))
	{
		lpAnimMng.SetAnime(animeStr_, "Jump");
		movePow_.y = 0.0f;
		//壁すり状態なら壁ジャンプ
		if ((_phase == &Player::WallGrabPhese))
		{
			moveVec_ = (-moveVec_);
			if (moveVec_.x > 0.0f)
			{
				dir_LR_ = DIR_LR::RIGHT;
			}
			else
			{
				dir_LR_ = DIR_LR::LEFT;
			}
			movePow_.x = 0.0f;
			_phase = &Player::WallJumpPhese;
		}
		else//違うなら通常ジャンプ
		{
			_phase = &Player::JumpPhese;
		}
	}
}

