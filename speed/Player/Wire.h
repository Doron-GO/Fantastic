#pragma once
#include"../Vector2D.h"
#include"../Common/Raycast.h"

class Player;

class Wire
{
public:

	Wire(Player& player);
	~Wire();

	void Update();
	void Draw();	
	void (Wire::* _phase)();

	void SwingPhase();
	void AnchoringPhase();

	void SetPalam();

	bool IsHitHook();

private:

	Player& player_;
	Raycast rayCast_;

	Vector2DFloat pos_;
	Vector2DFloat moveVec_;
	VECTOR movePow_;

};

