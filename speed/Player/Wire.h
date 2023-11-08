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

	//�t�F�[�Y��EndSwingPhase�ɂ��āAplayer_.EndSwing();
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
	Vector2DFloat yNorm_;			//�����琎�̐��K���ςݐ����x�N�g��
	
	VECTOR Scale_;
	VECTOR movedPos_;

	float gMag_;			//�d�͂̑傫��
	float theta_;			//�p�x
	float omega_;			//�p���x
	float length_;			//����
	float pow_;

};

