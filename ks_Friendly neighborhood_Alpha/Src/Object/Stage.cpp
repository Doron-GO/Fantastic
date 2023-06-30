#include <DxLib.h>
#include <stdlib.h>
#include <string>
#include "../Utility/AsoUtility.h"
#include "../Manager/ResourceManager.h"
#include "Player.h"
#include "Stage.h"
#include"StagePiece.h"


Stage::Stage()
	:resourceManager_(ResourceManager::GetInstance())
{

}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{
	swingpoint_.Load();

	transform_.SetModel(
		resourceManager_.LoadModelDuplicate(ResourceManager::SRC::STAGE));
	//モデルのロード
	//モデルの設定
	transform_.scl = { 1.0f, 1.0f, 1.0f };
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.localPos = { -0.0f,-1500.0f,0.0f };
	transform_.pos = VAdd(transform_.pos, transform_.localPos);
	transform_.quaRot = Quaternion();
	transform_.Update();
	//当たり判定の生成
	MakeStageCol();
}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{
	char p = '0' + 1;
	char pp = '0' + 2;
	DrawFormatString(300, 300, 0xffffff, "B" + p + pp);
	VECTOR pos{ -46.07962f, 8.936572f, 16.83637f };
	DrawSphere3D(test, 60.0f, 20, 0xffffff, 0xffffff, true);
	MV1DrawModel(transform_.modelId);
	auto mom = MV1GetFramePosition(transform_.modelId, 10781);
	DrawFormatString(300, 500, 0xffffff,"%f,%f,%f", mom.x,mom.y,mom.z);

}

void Stage::Release(void) 
{
	MV1DeleteModel(transform_.modelId);
}

int Stage::GetModelId(void) const
{
    return transform_.modelId;
}

const Transform* Stage::GetTransform(int num)
{
	return &transform_;
}

VECTOR Stage::GetSwinPoint(void)
{
	VECTOR sPos ={ -42.4f,37.6f,-12.5f };


	return sPos;
}

void Stage::MakeStageCol(void)
{

	/*for (int idx1 = 0; idx1 < 4; idx1++)
	{
		const char i = '0'+idx1;
		for (int idx2 = 0; idx2 < 5; idx2++)
		{
		 char B='B';
			const char d = '0'+idx2;
			const char dummy = 0;
			char o[4] ={B,i,d,dummy };
			auto  FrameIndex = MV1SearchFrame(transform_.modelId, o);
			frameIndex_.emplace_back(FrameIndex);

			int num = frameIndex_.size();
			col2_.emplace(num, FrameIndex) ;


		}
	}*/
	transform_.MakeCollider2(Collider::TYPE::STAGE,-1);


	//auto col=   MV1SetupCollInfo((transform_.modelId,)
	//col_.push_back();

	// 当たり判定(コライダ)作成

	transform_.Update();
}


void Stage::Load(void)
{
	Transform planetTrans;
	planetTrans.SetModel(
		resourceManager_.LoadModelDuplicate(ResourceManager::SRC::STAGE));
	planetTrans.scl = AsoUtility::VECTOR_ONE;
	planetTrans.quaRot = Quaternion();
	planetTrans.pos = { 0.0f, -100.0f, 0.0f };

	// 当たり判定(コライダ)作成
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	STAGE_NUM num = STAGE_NUM::S1_1;
	StagePiece* stagePiece = new StagePiece(num,  planetTrans);
	//stagePiece_->Init();
	stagePiece_.emplace(num, stagePiece);


}


