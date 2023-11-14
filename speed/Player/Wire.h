#pragma once
#include"../Vector2D.h"
#include"../Common/Raycast.h"
#include "../Common/Time.h"

class Player;

class Wire
{
public:

	Wire(Player& player,ColList& list);
	~Wire();

	void Update();
	void Draw(Vector2DFloat cameraPos);

	void SetSwingPalam();
	void SetAnchorPalam();

	//フェーズをEndSwingPhaseにして、player_.EndSwing();
	void StartSwingJump(); 
	void StartSwing(); //スイングフェーズに切り替える関数
	void EndSwing();//スイングフェーズに切り替える関数
	void Pump();
	bool IsHitHook();
	void SwingJump();
	void ChangeStandby();

private:

	void (Wire::* _phase)();

	void SwingPhase();
	void StandbyPhase();
	void EndSwingPhase();
	void AnchoringPhase();


	ColList col;
	Time timeManager_;

	Player&  player_;
	Raycast rayCast_;

	Vector2DFloat fulcrum_;
	VECTOR fulcrum_pos;//計算用
	float angle_;
	VECTOR vel_;
	float v_;
	
	Vector2DFloat moveVec_;
	Vector2DFloat gravity_;
	Vector2DFloat yNorm_;			//軸から錘の正規化済み垂直ベクトル
	
	VECTOR Scale_;

	float gMag_;			//重力の大きさ
	float theta_;			//角度
	float omega_;			//角速度
	float length_;			//長さ
	float pow_;

};

