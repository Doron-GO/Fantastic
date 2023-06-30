#pragma once
#include"Stage.h"
#include"Common/Transform.h"
class StagePiece
{
public:
	StagePiece(Stage::STAGE_NUM num,Transform transform);
	~StagePiece(void);

	void Draw(void);
	Transform* GetTransform(void);


private:

	Transform transform_;

	Stage::STAGE_NUM num_;

};

