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

	//�x�X�g�ȕ��@�ł͂Ȃ����������ǁAPad�i���o�[���g����actlist��ς���
	 char num = '0'+padNum_;
	 std::string act = "Src/Img/act";//string����������
	 act += num;						 //	�������A��
	 act +=".list";					 //	�������A��

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

		//�ǂɓ������Ă����牡�ړ������Ȃ�
		if (CollisionVec(moveVec_))
		{
			pos_.x += movePow_.x;
		}
	}

}

void Player::Draw(Vector2DFloat cameraPos)
{

	auto pos = pos_ + cameraPos;

	//�f�o�b�O�p�̎��ۂ̃L�����̍��W��\��

	DrawRotaGraph2F(pos_.x, pos_.y ,
			24.0f, 35.0f,
			1.5, 0.0,
			lpImageMng.GetID(animeStr_.imgKey_)[(*animeStr_.animID_)[GraphHD]],
			true, static_cast<int>(dir_LR_), 0);

	//���������̃L�����N�^�[�`��
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
		DrawString(0, 100, "�ǂɓ�������", 0xffffff);
	}
	if (!CollisionDown())
	{
		DrawString(0, 80, "���ɓ�������", 0xffffff);
	}
	if (!ColWallGrab(moveVec_))
	{
		DrawString(0, 60, "�W�����v�ǂɓ�������", test);
	}

	}
	char num= '0' + padNum_;
	std::string pp ="����";
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
	//���������Ȃ�������t�H�[���ɂ���
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

	//�W�����v���x���ő�ɒB�����������́A�����Ԃ�����
	if((movePow_.y<=-6.0f)||!(CollisionVec(up_)))
	{
		//���̈ړ���0�ɂ��ăt�H�[�����Ă�
		//movePow_.y = 0.0f;
		_phase = &Player::FallPhase;
	}
	//�łȂ����y�̈ړ��ʂ����Z����
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
	//���Ɠ������Ă��Ȃ�������
	if (!CollisionVec(movecec))
	{
		movePow_.y = 0.0f;
		_phase = &Player::MovePhase;

	}
	else
	{
		movePow_.y += 0.2f;
	}
	//�������x�����𒴂����猈�܂����l�ɂ���
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

	//�ǂɂ������Ă��Ȃ�������t�H�[����ԂɈڍs����
	if (!(_phase == &Player::WallJumpPhese))
	{
		if (!IsWall())
		{
			moveVec_ = -(moveVec_);
			_phase = &Player::FallPhase;
		}
	}
	Vector2DFloat movecec = { 0.0f,movePow_.y };

	//�ǂ��܂蒆�ɓ���ł����炻��ȏ�͏オ��Ȃ�
	if (!CollisionVec(up_))
	{
		movePow_.y = 0.0f;
	}
	//�����n�ʂɑ���������
	if ( !CollisionVec(movecec))
	{
		movePow_.y = 0.0f;
		_phase = &Player::MovePhase;
	}
	else//�łȂ���Ώ�ɏオ��
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
	//�W�����v���x���ő�ɒB�����������́A�n�ʂɐڒn������
	if ((movePow_.y <= -5.0f) || !(CollisionVec(movevec)))
	{
		_phase = &Player::FallPhase;
	}
	//�łȂ����y�̈ړ��ʂ����Z����
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

	//�ǂɓ�������������x���O�ɂ��� 1:�����̑O�� 2:��΂ߑO 
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
	//�n�ʂ������͓V��ɓ���������X�C���O����߂�
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
	//�������x�����𒴂����猈�܂����l�ɂ���
	if (movePow_.y >= 8.0f)
	{
		movePow_.y = 8.0f;
	}

	Vector2DFloat movecec = { 0.0f,movePow_.y };
	Vector2DFloat up = { 0.0f,-50.0f };

	//���Ɠ������Ă��Ȃ�������
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

	//�ǂ��܂��Ԃ̔���
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

	//�ǂɓ�������������x���O�ɂ��� 1:�����̑O�� 2:��΂ߑO 
	diagonallyVec_ = { moveVec_.x,slideY_ };
	if (!CollisionVec(moveVec_) || !CollisionVec(diagonallyVec_))
	{
		movePow_.x = 0.0f;
	}

	//�w������ǂɓ���������
	Vector2DFloat backVec = { -(moveVec_.x / 2.0f),0.0f };
	Vector2DFloat backDiagonallyVec_ = { backVec.x,slideY_ };//�w������̎΂߃o�[�W����

	if (!CollisionVec(backVec))
	{
		movePow_.x = -(backVec.x / 11.0f);
	}
	if (!CollisionVec(backDiagonallyVec_))
	{
		movePow_.x = -(backVec.x / 11.0f);
	}
	//�ǃW�����v�̂ł���ǂƔw�����Ƃ̓����蔻��
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
			//TRACE("��������\n");
			return false;
		}
	}
	return true;
}

bool Player::CollisionVec(Vector2DFloat movevec)
{
	Vector2DFloat rayCenter = { pos_ };
	//���ۂ̓����蔻�背�C�̕`��
	
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
	//�E�Ƃ͍��L�[��������Ă��Ȃ��Ƃ�
	if (!input.IsPrassed("right")&&!input.IsPrassed("left"))
	{
		if (_phase == &Player::MovePhase)
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
		//�W�����v�A�j���[�V����������Ȃ�������
		if (_phase == &Player::MovePhase) { lpAnimMng.SetAnime(animeStr_, "Idle"); }
	}
	else 
	{
		//�W�����v�A�j���[�V����������Ȃ�������
		if (_phase == &Player::MovePhase) { lpAnimMng.SetAnime(animeStr_, "Run");}
	}

	//�X���C�f�B���O�{�_����������Ă��Ȃ���
	if (!input.IsPrassed("c"))
	{
		if (!(_phase == &Player::WallJumpPhese)&& !(_phase == &Player::SwingPhese))
		{
			//�E�L�[
			if (input.IsPrassed("right"))
			{
				dir_LR_ = DIR_LR::RIGHT;
				movePow_.x += 0.2f;
				moveVec_ = { 16.0f,0.0f };

			}
			//���L�[
			if (input.IsPrassed("left"))
			{
				dir_LR_ = DIR_LR::LEFT;
				movePow_.x -= 0.2f;
				moveVec_ = { -16.0f,0.0f };

			}
		}

		if(_phase == &Player::SwingJumpPhese)
		{
			//�ړ����x�����𒴂���ƍő呬�x�ɌŒ肷��
			if (movePow_.x <= -12.0f) { movePow_.x = -12.0f;}
			else if (movePow_.x >= 12.0f) { movePow_.x = 12.0f;}
		}
		else
		{
			//�ړ����x�����𒴂���ƍő呬�x�ɌŒ肷��
			if (movePow_.x <= -8.0f) { movePow_.x = -8.0f; }
			else if (movePow_.x >= 8.0f) { movePow_.x = 8.0f; }
		}
	}
	//�ǂɓ���������̂ǂ̏������܂Ƃ߂Ă���
	MoveColision();

	//����������Ȃ��Ƃ�
	if (!(_phase == &Player::FallPhase)&&!(_phase == &Player::JumpPhese) &&
		!(_phase == &Player::SwingJumpPhese))
	{
		//�X���C�f�B���O�{�^����������Ă�����
		if (input.IsPrassed("c"))
		{
			lpAnimMng.SetAnime(animeStr_, "Slide");
			//�X���C�f�B���O���͌�������
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
				_phase = &Player::MovePhase;//����̂����ŃW�����v���Ƀt�b�N�{�^���A�łłӂ���W�����v����
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
	//��L�[���������Ƃ�
	if (input.IsTriggerd("jump"))
	{
		lpAnimMng.SetAnime(animeStr_, "Jump");
		movePow_.y = 0.0f;
		//�ǂ����ԂȂ�ǃW�����v
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
		else//�Ⴄ�Ȃ�ʏ�W�����v
		{
			_phase = &Player::JumpPhese;
		}
	}
}

