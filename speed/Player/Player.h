#pragma once
#include<map>
#include<string>
#include"../Vector2D.h"
#include"../Obj/AnimMng.h"

class Input;

enum class DIR_LR
{
	LIGHT,
	LEFT
};

class Player
{
public:
	 Player();
	 ~Player();
	 void Init();
	 void Update(Input& input);
	 void Draw();
private:

	void (Player::* _phase)(Input& input);
	void (Player::* _draw)();

	void IdlePhase(Input& input);
	void MovePhase(Input& input);//左右移動：ジャンプ
	void JumpPhese(Input& input);//ジャンプ
	void FallPhase(Input& input);//落下中

	void IdleDraw();
	void JumpDraw();
	void MoveDraw();


	//左右移動
	void Move(Input& input);
	void Jump(Input& input);

	std::map<std::string, int> imgkey_;
	

	AnimStr animeStr_;


	DIR_LR dir_LR_;//キャラクターの向き

	Vector2DFloat pos_;//キャラの座標

	VECTOR movePow_;
	VECTOR moveVec_;
};

