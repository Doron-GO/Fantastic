#include <DxLib.h>
#include "StagePiece.h"
#include "Common/Transform.h"

StagePiece::StagePiece(Stage::STAGE_NUM num, Transform transform)
{
	num_ = num;
	transform_ = transform;

}

StagePiece::~StagePiece(void)
{
}

void StagePiece::Draw(void)
{
	MV1SetOpacityRate(transform_.modelId, 0.5f);
	MV1DrawModel(transform_.modelId);

}

Transform* StagePiece::GetTransform(void)
{
	return &transform_;
}
