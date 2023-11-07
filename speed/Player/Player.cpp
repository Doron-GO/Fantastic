#include<DxLib.h>
#include<string>
#include "Player.h"
#include"Wire.h"
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
	dir_LR_ = DIR_LR::RIGHT;
	_phase = &Player::FallPhase;
	_draw = &Player::MoveDraw;
	
	wire_ = std::make_unique<Wire>(*this);
}

void Player::Update(Input& input)
{
	lpAnimMng.UpdateAnime(animeStr_);
	input_.Update(padNum_);
	Move(input_);
	Anchoring(input_);
	
	(this->*_phase)(input_);

	wire_->Update();
	if (!(_phase == &Player::SwingPhese))
	{
		pos_.y += movePow_.y;
		//壁に当たっていたら横移動させない
		if (Collision(moveVec_))
		{
			pos_.x += movePow_.x;
		}

	}

}

void Player::Draw(Vector2DFloat cameraPos)
{

	auto pos = pos_ + cameraPos;

	//デバッグ用の実際のキャラの座標を表示

	//DrawRotaGraph2F(pos_.x, pos_.y ,
	//		24.0f, 35.0f,
	//		1.5, 0.0,
	//		lpImageMng.GetID(animeStr_.imgKey_)[(*animeStr_.animID_)[GraphHD]],
	//		true, static_cast<int>(dir_LR_), 0);

	//見せかけのキャラクター描画
	DrawRotaGraph2F(pos.x, pos.y,
		24.0f, 35.0f,
		1.5, 0.0,
		lpImageMng.GetID(animeStr_.imgKey_)[(*animeStr_.animID_)[GraphHD]],
		true, static_cast<int>(dir_LR_), 0);
	if (padNum_ == 1)
	{
		DrawFormatStringF(0, 0, 0xffffff, "movePow_(x:%f,y%f)", movePow_.x, movePow_.y);
		DrawFormatStringF(0, 30, 0xffffff, "moveVec_(x:%f,y%f)", moveVec_.x, moveVec_.y);
		DrawFormatStringF(0, 50, 0xffffff, "pos_(x:%f,y%f)", pos_.x, pos_.y);

	}
	if (!Collision(moveVec_))
	{
		DrawString(0, 120, "壁に当たった", 0xffffff);
	}
	if (!Collision())
	{
		DrawString(0, 90, "床に当たった", 0xffffff);
	}
	if (!ColWallSlide(moveVec_))
	{
		DrawString(0, 70, "ジャンプ壁に当たった", test);
	}

	Vector2DFloat rayCenter = { pos - center_ };
	Vector2DFloat diagonallyVec = { moveVec_.x,slideY_ };

	if (_phase == &Player::MovePhase)
	{
		//足元から進行方向に出るレイ
		DrawLine(pos.x, pos.y ,
			moveVec_.x+ pos.x , moveVec_.y+ pos.y , 0xff0000);
		//足元から前斜め前に伸びるレイ
		DrawLine(pos.x, pos.y ,
			diagonallyVec.x+ pos.x , diagonallyVec.y+ pos.y , 0xff0000);
		//足元から下に伸ばすレイ
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
	wire_->Draw(cameraPos);
}

const Vector2DFloat Player::GetPos()
{
	return pos_;
}

Vector2DFloat Player::GetDiagonallyVecVec()
{
	return diagonallyVec_;
}

Vector2DFloat Player::GetMoveVec()
{
	return moveVec_;
}

Vector2DFloat Player::GetMovePow()
{
	return movePow_;
}

void Player::IdlePhase(Input& input)
{

}

void Player::MovePhase(Input& input)
{
	//Move(input);
	//もし床がなかったらフォールにする
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

	//ジャンプ高度が最大に達したもしくは、頭をぶつけたら
	if((movePow_.y<=-13.0f)||!(Collision(movevec)))
	{
		//ｙの移動量0にしてフォールを呼ぶ
		movePow_.y = 0.0f;
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
	lpAnimMng.SetAnime(animeStr_, "Fall");

	//落下速度が一定を超えたら決まった値にする
	if (movePow_.y >= 8.0f)
	{
		movePow_.y = 8.0f;
	}	

	Vector2DFloat movecec = { 0.0f,movePow_.y };

	//床と当たっていなかったら
	if (Collision(movecec))
	{
		movePow_.y += 0.2f;
	}
	movecec = { 0.0f,movePow_.y };
	//接地したら地上移動モードにする
	if (!Collision(movecec))
	{	
		movePow_.y = 0.0f;
		_phase = &Player::MovePhase;
	}

}

void Player::WallSlidePhese(Input& input)
{
	 diagonallyVec_ = { moveVec_.x,slideY_ };
	Jump(input);

	//壁にくっついていなかったらフォール状態に移行する
	if (!(_phase == &Player::WallJumpPhese))
	{
		if ((ColWallSlide(moveVec_)||ColWallSlide(diagonallyVec_))&&Collision())
		{
			_phase = &Player::FallPhase;
		}
	}
	//もし地面に足がついたら
	if (!Collision())
	{
		movePow_.y = 0.0f;
		_phase = &Player::MovePhase;
	}
	else
	{
		lpAnimMng.SetAnime(animeStr_, "WallSlide");
		movePow_.y = 2.0f;
	}
}

void Player::WallJumpPhese(Input& input)
{
	lpAnimMng.SetAnime(animeStr_, "Jump");
	 diagonallyVec_ = { moveVec_.x,slideY_ };
	Vector2DFloat movevec = { 0.0f,-40.0f };
	//ジャンプ高度が最大に達したもしくは、地面に接地したら
	if ((movePow_.y <= -12.0f) || !(Collision(movevec)))
	{
		//ｙの移動量0にしてフォールを呼ぶ
		movePow_.y = 0.0f;
		_phase = &Player::FallPhase;
	}
	//でなければyの移動量を加算する
	else
	{
		movePow_.y += -0.2f;
		movePow_.y += -0.3f;
		movePow_.x += (moveVec_.x / 30.0f);

	}
	if ((!ColWallSlide(moveVec_) || !ColWallSlide(diagonallyVec_)) )
	{
		movePow_.x = 0.0f;
		_phase = &Player::WallSlidePhese;
	}


}

void Player::SwingPhese(Input& input)
{
	if (input.IsPrassed("jump"))
	{
		lpAnimMng.SetAnime(animeStr_, "Jump");
		movePow_.y = 0.0f;
		wire_->EndSwing();
		_phase = &Player::FallPhase;

	}
}

bool Player::Collision()
{
	Vector2DFloat rayCenter = { pos_-center_};
	Vector2DFloat moveVec = { 0.0f,16.0f };
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

bool Player::Collision(Vector2DFloat movevec)
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

bool Player::ColWallSlide(Vector2DFloat movevec)
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
	//右とは左キーが押されていないとき
	if (!input.IsPrassed("right")&&!input.IsPrassed("left"))
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
			if (0.40f >= movePow_.x&& movePow_.x >= 0.02f)
			{
				movePow_.x = 0.0f;
			}
		}
		if (dir_LR_ == DIR_LR::LEFT)
		{
			if(-0.40f<=movePow_.x && movePow_.x <= -0.02f)
			{
				movePow_.x = 0.0f;
			}
		}
		//ジャンプアニメーション中じゃなかったら
		if (_phase == &Player::MovePhase)
		{
			lpAnimMng.SetAnime(animeStr_, "Idle");
		}
	}
	else 
	{
		//ジャンプアニメーション中じゃなかったら
		if (_phase == &Player::MovePhase)
		{
			lpAnimMng.SetAnime(animeStr_, "Run");
		}
	}

	//スライディングボダンが押されていない時
	if (!input.IsPrassed("c"))
	{
		if (!(_phase == &Player::WallJumpPhese))
		{
			//右キー
			if (input.IsPrassed("right"))
			{
				dir_LR_ = DIR_LR::RIGHT;
				movePow_.x += 0.2f;
				moveVec_ = { 18.0f,0.0f };

			}
			//左キー
			if (input.IsPrassed("left"))
			{
				dir_LR_ = DIR_LR::LEFT;
				movePow_.x -= 0.2f;
				moveVec_ = { -18.0f,0.0f };

			}
		}

		//移動速度が一定を超えると最大速度に固定する
		if (movePow_.x <= -8.0f)
		{
			movePow_.x = -8.0f;
		}
		else if (movePow_.x >= 8.0f)
		{
			movePow_.x = 8.0f;
		}
	}

	//壁に当たったら加速度を０にする 1:自分の前方 2:上斜め前 
	 diagonallyVec_ = { moveVec_.x,slideY_ };
	if (!Collision(moveVec_)|| !Collision(diagonallyVec_))
	{
		movePow_.x = 0.0f;
	}
	//背中から壁に当たったら
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
	if (!ColWallSlide(backVec))
	{
		movePow_.x = -(backVec.x / 11.0f);
	}
	if (!ColWallSlide(backVec))
	{
		movePow_.x = -(backVec.x / 11.0f);
	}

	if (_phase == &Player::JumpPhese|| _phase == &Player::FallPhase)
	{
		if ((!ColWallSlide(moveVec_)||!ColWallSlide(diagonallyVec_)) && Collision())
		{
			movePow_.x = 0.0f;
			_phase = &Player::WallSlidePhese;
		}
	}

	//落下中じゃないとき
	if (!(_phase == &Player::FallPhase)&&!(_phase == &Player::JumpPhese))
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
	if (input.IsPrassed("hook"))
	{
		if(!(_phase == &Player::SwingPhese))
		wire_->SetPalam();
		_phase = &Player::SwingPhese;
	}
}

void Player::Jump(Input& input)
{
	//上キーを押したとき
	if (input.IsPrassed("jump"))
	{
		lpAnimMng.SetAnime(animeStr_, "Jump");
		movePow_.y = 0.0f;
		//壁すり状態なら壁ジャンプ
		if ((_phase == &Player::WallSlidePhese))
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

