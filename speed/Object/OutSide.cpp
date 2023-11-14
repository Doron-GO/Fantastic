#include<DxLib.h>
#include "OutSide.h"
#include"Camera.h"

OutSide::OutSide(Camera& camera) :camera_(camera), minPos_({ 0.0f,0.0f }),maxPos_({ 1600.0f, 1000.0f })
{

}

OutSide::~OutSide()
{
}

void OutSide::Update()
{
	minPos_ .x+= 0.5f;
	minPos_ .y+= 0.5f;
	maxPos_.x -= 0.5f;
	maxPos_.y -= 0.5f;
}

void OutSide::Draw()
{
	//Œ©‚¹‚©‚¯‚Ì˜g
	DrawBox(minPos_.x, minPos_.y, maxPos_.x, maxPos_.y, 0x0000ff,false);
}

bool OutSide::IsOutSide()
{




	return false;
}
