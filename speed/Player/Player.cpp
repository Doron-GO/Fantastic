#include<DxLib.h>
#include<string>
#include "Player.h"
#include"Wire.h"
#include"../Object/Manager/ImageMng.h"
#include"../_debug/_DebugConOut.h"
#include"../_debug/_DebugDispOut.h"

Player::Player(int playerNum, Blocks& blocks):dir_LR_(DIR_LR::RIGHT),phase_(PHASE::FALL),
aliveFlag_(true), padNum_(playerNum), damageCount_(0), blocks_(blocks)
{
	AnchoringFlag_ = false;
	doubleJump_ = true;
	ExplosionSound_[0] = LoadSoundMem("Src/Sound/Explosion.mp3");
	ExplosionSound_[1] = LoadSoundMem("Src/Sound/launcher1.mp3");

}

Player::~Player()
{
}

void Player::Init(ColList colList, ColList wallColList, ColList wireColList)
{
	pos_ = { 400.0f-padNum_*-20.0f,2720.0f };
	//pos_ = { 0.0f,0.0f };
	center_ = { 0.0f,16.0f };
	grndColList_ = colList;
	wallcolList_ = wallColList;
	//ベストな方法ではないかもだけど、Padナンバーを使ってactlistを変える
	 char num = '0'+padNum_;
	 std::string act = "Src/Img/actList/act";//string文字列を作る
	 act += num;						 //	文字列を連結
	 act +=".list";					 //	文字列を連結

	lpAnimMng.LoadAnime(act.c_str());

	std::string objID = "Player";
	objID += num;
	animeStr_.objID_ = objID.c_str();
	lpAnimMng.SetAnime(animeStr_, "Idle");

	LoadDivGraph("Src/Img/BigExplosion.png", 8, 8, 1, 32, 32, explosionImg_);

	expCount_ = 0;
	item_ = std::make_shared<ItemBase>();
	moveVec_ = { 0.0f,0.0f};
	movePow_ = { 0.0f,0.0f};
	up_ = { 0.0f,-50.0f };
	_phase = &Player::MovePhase;
	_damage = &Player::Nothing;
	itemList_ = ItemList::NON;
	wire_ = std::make_unique<Wire>(*this,wireColList);
	missileImg_ = LoadGraph("Src/Img/alamo.png");

}

void Player::Update(Input& input)
{
	input_.Update(padNum_);
	if (aliveFlag_)
	{
		(this->*_damage)();
		(this->*_phase)(input_);
		BlocksCollision();
		if (_damage == &Player::Nothing && !((_phase == &Player::WinnerPhese)|| (_phase == &Player::WinnerFallPhese)))
		{
			Move(input_);	
			Anchoring(input_);
			wire_->Update();
			if (!(itemList_ == ItemList::NON))
			{
				if (item_->IsEnd())
				{
					itemList_ = ItemList::NON;
				}		
				if (input_.IsTriggerd("item")&& !item_->IsActivate())
				{
					item_->Activate(Vector2DFloat{ pos_.x,pos_.y - 20.0f });
					ItemUse();
				}		
				item_->SetPos(Vector2DFloat{ targetPos_.x,targetPos_.y - 20.0f });
			}
		}	
		if (!(itemList_ == ItemList::NON))	{item_->Update();}
		if (!(_phase == &Player::SwingPhese))
		{
			pos_.y += movePow_.y;
			//壁に当たっていたら横移動させない
			if (_damage == &Player::Nothing &&(CollisionVec({moveVec_.x ,0.0f})||CollisionVec(diagonallyVec_)) )
			{
				pos_.x += movePow_.x;
			}
		}
		lpAnimMng.UpdateAnime(animeStr_);
	}
}

void Player::Draw(Vector2DFloat cameraPos)
{
	auto pos = pos_ + cameraPos;
	if (winFlag_)
	{
		if (expCount_ <= 34)
		{

			DrawRotaGraph2F(pos.x, pos.y ,
				15.f, 16.0f,
				10.0, 0.0,
				explosionImg_[(expCount_/4)], true);
			expCount_++;
		}
	}
	if (aliveFlag_)
	{
		//見せかけのキャラクター描画
		DrawRotaGraph2F(pos.x, pos.y+0.3f,
			24.0f, 35.0f,
			1.5, 0.0,
			lpImageMng.GetID(animeStr_.imgKey_)[(*animeStr_.animID_)[GraphHD]],
			true, static_cast<int>(dir_LR_), 0);

		TesItemDraw(cameraPos);
	}
		
	switch (itemList_)
	{
	case Player::ItemList::NON:
		now_Item_ = "NON";
		break;
	case Player::ItemList::MISSILE:
		DrawRotaGraph2F(pos.x-13.0f, pos.y-40.0f,
			4.0f, 10.0f,
			1.5, 0.0,missileImg_, true);

		now_Item_ = "MISSILS";
		break;
	case Player::ItemList::LASER:
		now_Item_ = "LASER";
		break;
	}
	DebugPhaseCheck();
	//DrawString(pos.x-10.0f, pos.y-60.0f, now_Item_.c_str(), 0xffffff);
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
		now_ = "SwingJumpPhase";
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
	if (CollisionDown())
	{
		_phase = &Player::FallPhase;
	}
	else
	{
		pos_.y = landingPos_.y;
	}
	Jump(input);
}

void Player::Nothing()
{
}

void Player::DamageMissile()
{
	if ((damageCount_++<30.0f) && (CollisionVec(up_)))
	{		
		if (damageCount_<8.0f)
		{
			movePow_.y -= 0.3f;

		}
	}
	else 
	{	//ｙの移動量0にしてフォールを呼ぶ
		_phase = &Player::FallPhase;
		_damage = &Player::Nothing;
		damageCount_ = 0;
	}

}

void Player::JumpPhese(Input& input)
{
	phase_ = Player::PHASE::JUMP;

	lpAnimMng.SetAnime(animeStr_, "Jump");	

	//ジャンプ高度が最大に達したもしくは、頭をぶつけたら
	 if(!(CollisionVec(up_)))
	{
		//ｙの移動量0にしてフォールを呼ぶ
		movePow_.y = 0.0f;
		_phase = &Player::FallPhase;
	}

	 if((movePow_.y<-5.0f))
	{
		//ｙの移動量0にしてフォールを呼ぶ
		//movePow_.y = 0.0f;
		_phase = &Player::FallPhase;
	}
	//でなければyの移動量を加算する
	else
	{
		//movePow_.y += -0.2f;
		movePow_.y += -1.0f;
	}
}

void Player::FallPhase(Input& input)
{		
	Jump(input);
	if (CollisionVec({ 0.0f,movePow_.y+0.1f }))
	{
		movePow_.y += 0.2f;
	}
	else
	{
		movePow_.y = 0.0f;
		pos_.y = landingPos_.y;
		doubleJump_ = true;
		_phase = &Player::MovePhase;
	}

	if (movePow_.y > 12.0f)
	{
		movePow_.y = 12.0f;
	}
	if (!CollisionVec(up_))
	{
		movePow_.y = 0.2f;
	}
	//CollisionDown
	//落下速度が一定を超えたら決まった値にする
	phase_ = Player::PHASE::FALL;
	lpAnimMng.SetAnime(animeStr_, "Fall");	
}

void Player::DamagePhase(Input& input)
{

}

void Player::WallGrabPhese(Input& input)
{
	phase_ = Player::PHASE::WALLGRAB;
	diagonallyVec_ = { moveVec_.x,slideY_ };
	Jump(input);	
	Vector2DFloat movecec = { 0.0f,movePow_.y };

	if ( !CollisionVec(movecec))
	{
		pos_.y = landingPos_.y;
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

	//壁にくっついていなかったらフォール状態に移行する
	if (!(_phase == &Player::WallJumpPhese))
	{
		if (!IsWall())
		{
			moveVec_ = -(moveVec_);
			_phase = &Player::FallPhase;
		}
	}
	//壁つかまり中に頭を打ったらそれ以上は上がらない
	if (!CollisionVec(up_))
	{
		movePow_.y = 0.0f;
	}
	//もし地面に足がついたら
}

void Player::WallJumpPhese(Input& input)
{
	phase_ = Player::PHASE::WALLJUMP;
	lpAnimMng.SetAnime(animeStr_, "Jump");
	diagonallyVec_ = { moveVec_.x,slideY_ };
	Vector2DFloat movevec = { 0.0f,-40.0f };
	//ジャンプ高度が最大に達したもしくは、地面に接地したら
	if (movePow_.y <= -5.0f)
	{

		_phase = &Player::FallPhase;
	}
	if (!(CollisionVec(movevec)))
	{
		_phase = &Player::FallPhase;
	}
	//でなければyの移動量を加算する
	else
	{
		movePow_.y += -0.5f;
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
	Vector2DFloat movevec = { 0.0f,1.0f };
	if (!CollisionVec(movevec))
	{
		pos_.y = landingPos_.y;
		movePow_.y = 0.0f;
		_phase = &Player::MovePhase;
		wire_->ChangeStandby();
		wire_->EndSwing();
	}

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
	if (!CollisionVec(diagonallyVec_))
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
}

void Player::SwingJumpPhese(Input& input)
{
	Jump(input);
	phase_ = Player::PHASE::SWINGJUMP;
	lpAnimMng.SetAnime(animeStr_, "Fall");
	//落下速度が一定を超えたら決まった値にする
	if (movePow_.y >= 14.0f)
	{
		movePow_.y = 14.0f;
	}	
	if (movePow_.x >0.1f&&movePow_.x >= 7.5f)
	{
		movePow_.x = 7.5f;
	}
	else if (movePow_.x <-0.1f&&movePow_.x <= -7.5f)
	{
		movePow_.x = -7.5f;
	}
	Vector2DFloat movecec = { 0.0f,movePow_.y+0.1f };
	Vector2DFloat up = { 0.0f,-50.0f };
	//床と当たっていなかったら
	if (!CollisionVec(up))
	{
		movePow_.y = 0.0f;
		doubleJump_ = true;
		wire_->ChangeStandby();
		_phase = &Player::MovePhase;
	}
	else if (!CollisionVec(movecec))
	{
		pos_.y = landingPos_.y;
		movePow_.y = 0.0f;
		doubleJump_ = true;
		wire_->ChangeStandby();
		_phase = &Player::MovePhase;
	}
	else
	{
		movePow_.y += 0.2f;
	}

}


void Player::WinnerPhese(Input& input)
{

	lpAnimMng.SetAnime(animeStr_, "Win");

}

void Player::WinnerFallPhese(Input& input)
{


	if (!CollisionVec(up_))
	{
		movePow_.y = 0.0f;
	}

	if (CollisionVec({ 0.0f,movePow_.y + 0.1f }))
	{
		movePow_.y += 0.1f;
		lpAnimMng.SetAnime(animeStr_, "Fall");

	}
	else
	{
		movePow_.y = 0.0f;
		movePow_.x = 0.0f;
		pos_.y = landingPos_.y;
		doubleJump_ = true;
		_phase = &Player::WinnerPhese;
		PlaySoundMem(ExplosionSound_[0], DX_PLAYTYPE_BACK, true);
		winFlag_ = true;
	}
}

void Player::Conclusion()
{
	if (_phase == &Player::MovePhase)
	{
		movePow_.y = 0.0f;
		movePow_.x = 0.0f;
		_phase = &Player::WinnerPhese;
		PlaySoundMem(ExplosionSound_[0], DX_PLAYTYPE_BACK, true);

		winFlag_ = true;
	}
	else
	{
		_phase = &Player::WinnerFallPhese;

	}
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
	if ( !CollisionVec(diagonallyVec_)||!CollisionVec(moveVec_,{0.0f,-16.0f}) )
	//if (!CollisionVec(diagonallyVec_))
	{
		movePow_.x = 0.0f;
	}
	//背中から壁に当たったら
	Vector2DFloat backVec = { -(moveVec_.x / 2.0f),-16.0f };
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
	Vector2DFloat rayCenter = { pos_.x,pos_.y-10.0f};
	Vector2DFloat moveVec = { 0.0f,10.0f };
	for (const auto& col : grndColList_)
	{
		Raycast::Ray ray = { rayCenter,moveVec};
		if (rayCast_.CheckCollision(ray, col,pos_))
		{
			//TRACE("当たった\n");
			Landing(col.first.y);
			return false;
		}
	}
	return true;
}

bool Player::CollisionVec(Vector2DFloat movevec)
{
	//実際の当たり判定レイの描画
	//_dbgDrawLine(
	//	rayCenter.x, rayCenter.y,
	//	rayCenter.x + movevec.x, rayCenter.y + movevec.y,
	//	0x00ff00
	//);
	for (const auto& col : grndColList_)
	{
		Raycast::Ray ray = { pos_,movevec };
		if (rayCast_.CheckCollision(ray, col, pos_ ))
		{
			Landing(col.first.y);
			return false;
		}
	}
	return true;
}

bool Player::CollisionVec(Vector2DFloat movevec, Vector2DFloat center)
{
	Vector2DFloat center_{ pos_ + center };
	for (const auto& col : grndColList_)
	{
		Raycast::Ray ray = { center_,movevec };
		if (rayCast_.CheckCollision(ray, col, center_))
		{
			Landing(col.first.y);
			return false;
		}
	}
	return true;
}

bool Player::ColWallGrab(Vector2DFloat movevec, float y)
{
	float wallX=0.0f;
	Vector2DFloat rayCenter = { pos_.x, pos_.y - y };
	for (const auto& col : wallcolList_)
	{
		Raycast::Ray ray = { rayCenter,movevec };
		if (rayCast_.CheckCollision(ray, col, pos_))
		{		
			return false;
		}
	}	
	return true;
}

bool Player::IsWall()
{
	return (!ColWallGrab(moveVec_,16.0f) );
}

void Player::BlocksCollision()
{
	for (auto& block:blocks_.GetBlockList())
	{
		if (block.blockFlag_)
		{

			if (rayCast_.RectToRectCollision(col_.min_, col_.max_,
				block.col_.first, block.col_.second))
			{
				block.blockFlag_ = false;
				movePow_.x= 0.0f;
				movePow_.y /= 4.0f;
			}
		}
	}

}

void Player::Landing(float y)
{
	landingPos_.y = y;
}

void Player::Dead()
{
	pos_ = { 0.0f,0.0f };
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

Player:: ItemList Player::IsItem()
{
	return itemList_;
}

void Player::SetItemList(int itemNum)
{
	itemList_ = (ItemList)itemNum;
}

void Player::SetItem(std::shared_ptr <ItemBase> item)
{
	item_ = item;
}

void Player::TesItemDraw(Vector2DFloat cameraPos)
{
	if (item_->IsActivate()|| item_->IsExplosion())
	{
		item_->Draw(cameraPos);
	}
}

void Player::SetTarget(Vector2DFloat targetPos)
{
	targetPos_ = targetPos;
}

bool Player::TestItem()
{
	return testItemFlag;
}

const std::shared_ptr<ItemBase> Player::GetItem()
{
	return item_;
}

void Player::Damage(ItemBase::ITEM_TYPE type)
{
	switch (type)
	{
	case ItemBase::ITEM_TYPE::MISSILE:
		_damage = &Player::DamageMissile;
		_phase = &Player::DamagePhase;
		movePow_.x = 0.0f;
		lpAnimMng.SetAnime(animeStr_, "Dmage");
		break;
	case ItemBase::ITEM_TYPE::LASER:
		_damage = &Player::DamageMissile;
		lpAnimMng.SetAnime(animeStr_, "Dmage");
		break;
	}
	PlaySoundMem(ExplosionSound_[1], DX_PLAYTYPE_BACK, true);

}

const bool Player::IsWin()
{
	return winFlag_;
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
				moveVec_ = { 20.0f,15.0f };
			}
			//左キー
			if (input.IsPrassed("left"))
			{
				dir_LR_ = DIR_LR::LEFT;
				movePow_.x -= 0.2f;
				moveVec_ = { -20.0f,-15.0f };
			}
		}
		col_.min_ = { pos_.x - 10.0f,pos_.y };
		col_.max_ = { pos_.x + 15.0f,pos_.y - 40.0f };

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
			col_.min_ = { pos_.x - 10.0f,pos_.y };
			col_.max_ = { pos_.x + 15.0f,pos_.y-20.0f };
			lpAnimMng.SetAnime(animeStr_, "Slide");
			//スライディング中は減速する
			if (movePow_.x > 0.0f) 
			{
				movePow_.x -= 0.1f;
			}
			if (movePow_.x < 0.0f) 
			{
				movePow_.x += 0.1f;
			}			
			slideY_ = -13.0f;

		}
		else{ slideY_ = -35.0f; }
	}
}

void Player::ItemUse()
{
	if (!(itemList_ == ItemList::NON))
	{
		//itemList_ = ItemList::NON;
	}
}

void Player::Anchoring(Input& input)
{
	if (!(_phase == &Player::MovePhase))
	{
		if (input.IsTriggerd("hook"))
		{
			slideY_ = -35.0f;
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
		//壁すり状態なら壁ジャンプ
		if ((_phase == &Player::WallGrabPhese))
		{		
			movePow_.y = 0.0f;
			lpAnimMng.SetAnime(animeStr_, "Jump");

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
		else if(!(_phase == &Player::FallPhase))//違うなら通常ジャンプ
		{
			lpAnimMng.SetAnime(animeStr_, "Jump");
			movePow_.y = 0.0f;
			_phase = &Player::JumpPhese;
		}
		if (((_phase == &Player::FallPhase)|| (_phase == &Player::SwingJumpPhese))&&doubleJump_)
		{
			lpAnimMng.SetAnime(animeStr_, "Jump");
			movePow_.y = 0.2f;
			doubleJump_ = false;
			_phase = &Player::JumpPhese;
		}
	}
}

