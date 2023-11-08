#pragma once
#include"../Vector2D.h"
#include"../Common/Raycast.h"
#include "../Common/Time.h"

class Player;

class Wire
{
public:

	Wire(Player& player);
	~Wire();

	void Update();
	void Draw(Vector2DFloat cameraPos);

	void SetPalam();

	//フェーズをEndSwingPhaseにして、player_.EndSwing();
	void StartSwingJump(); 
	void EndSwing();
	void Pump();
	bool IsHitHook();
	void SwingJump();

private:

	void (Wire::* _phase)();

	void SwingPhase();
	void EndSwingPhase();
	void AnchoringPhase();


	Time timeManager_;

	Player&  player_;
	Raycast rayCast_;

	Vector2DFloat fulcrum_;
	float angle_;
	VECTOR vel_;
	float v_;
	
	Vector2DFloat moveVec_;
	Vector2DFloat gravity_;
	Vector2DFloat yNorm_;			//軸から錘の正規化済み垂直ベクトル
	
	VECTOR Scale_;
	VECTOR movedPos_;

	float gMag_;			//重力の大きさ
	float theta_;			//角度
	float omega_;			//角速度
	float length_;			//長さ
	float pow_;

};

