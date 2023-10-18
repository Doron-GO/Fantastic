#include<DxLib.h>
#include<string>
#include "Player.h"
#include"../Obj/ImageMng.h"
#include"../_debug/_DebugConOut.h"
#include"../_debug/_DebugDispOut.h"

Player::Player(int playerNum)
{
	padNum_ = playerNum;
}

Player::~Player()
{
}

void Player::Init(ColList colList)
{
	pos_ = { 300.0f,10.0f };

	center_ = { 0.0f,13.0f };
	colList_ = colList;

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
	input_.Update(padNum_);
	(this->*_phase)(input_);
	Move(input_);


	//壁に当たっていたら横移動させない
	if (Collision(moveVec_))
	{
		pos_.x += movePow_.x;
	}
	pos_.y += movePow_.y;

}

void Player::Draw()
{
	//(this->*_draw)();

	//DrawRotaGraph2F(pos_.x+ offset_.x, pos_.y + offset_.y,
	//	24.0f, 35.0f,
	//	1.5, 0.0,
	//	lpImageMng.GetID(animeStr_.imgKey_)[(*animeStr_.animID_)[GraphHD]],
	//	true, static_cast<int>(dir_LR_), 0);
	DrawRotaGraph2F(pos_.x, pos_.y ,
		24.0f, 35.0f,
		1.5, 0.0,
		lpImageMng.GetID(animeStr_.imgKey_)[(*animeStr_.animID_)[GraphHD]],
		true, static_cast<int>(dir_LR_), 0);

	DrawFormatStringF(0, 0, 0xffffff, "movePow_(x:%f,y%f)", movePow_.x, movePow_.y);
	DrawFormatStringF(0, 20, 0xffffff, "pos_(x:%f,y%f)", pos_.x, pos_.y);
	if (!Collision(moveVec_))
	{
		DrawString(0, 40, "当たった", 0xffffff);
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
	//もし床がなかったらフォールにする
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

	//ジャンプ高度が最大に達したもしくは、地面に接地したら
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

	//床と当たっていなかったら
	if (Collision())
	{
		movePow_.y += 0.2f;
	}

	//落下速度が一定を超えたら決まった値にする
	if (movePow_.y >= 8.8f)
	{
		movePow_.y = 8.8f;
	}
	//接地したら地上移動モードにする
	Vector2DFloat movecec = { 0.0f,8.0f };
	if (!Collision(movecec))
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
			//TRACE("当たった\n");
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
	//右もしくは左キーが押されていないとき
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

		//右キー
		if (input.IsPrassed("right"))
		{
			dir_LR_ = DIR_LR::LIGHT;
			movePow_.x += 0.2f;
			moveVec_ = { 22.0f,0.0f };

		}
		//左キー
		if (input.IsPrassed("left"))
		{
			dir_LR_ = DIR_LR::LEFT;
			movePow_.x -= 0.2f;
			moveVec_ = { -22.0f,0.0f };

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
	Vector2DFloat diagonallyVec = { moveVec_.x,-30.0f };
	if (!Collision(moveVec_)|| !Collision(diagonallyVec))
	{
		movePow_.x = 0.0f;
	}
	//背中から壁に当たったら
	Vector2DFloat backVec = { -(moveVec_.x/2.0f),0.0f };
	if (!Collision(backVec))
	{
		movePow_.x = -(backVec.x/11.0f);
	}


	//落下中じゃないとき
	if (!(_phase == &Player::FallPhase)&&!(_phase == &Player::JumpPhese))
	{
		//スライディングボタンが押されていたら
		if (input.IsPrassed("c"))
		{
			lpAnimMng.SetAnime(animeStr_, "Slide");
			//スライディング中は減速する
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
	//上キーを押したとき
	if (input.IsPrassed("jump"))
	{
		lpAnimMng.SetAnime(animeStr_, "Jump");
		movePow_.y = 0.0f;
		_phase = &Player::JumpPhese;

	}
}
