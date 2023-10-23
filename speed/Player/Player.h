#pragma once
#include<map>
#include<string>
#include"../Vector2D.h"
#include"../Obj/AnimMng.h"
#include"../Common/Collision.h"
#include"../Common/Raycast.h"
#include"../Input/Input.h"
#include"../Obj/Camera.h"
enum class DIR_LR
{
	LIGHT,
	LEFT
};

class Player
{
public:
	 Player(int playerNum);
	 ~Player();
	 void Init(ColList colList,int headTest);
	 void Update(Input& input);
	 void Draw();
	 const Vector2DFloat GetPos();

	 int headFlag_;//自分が先頭かどうか

private:

	void (Player::* _phase)(Input& input);
	void (Player::* _draw)();

	void IdlePhase(Input& input);
	void MovePhase(Input& input);//左右移動：ジャンプ
	void JumpPhese(Input& input);//ジャンプ
	void FallPhase(Input& input);//落下中

	bool Collision();
	bool Collision(Vector2DFloat movevec);

	void IdleDraw();
	void JumpDraw();
	void MoveDraw();


	//左右移動
	void Move(Input& input);
	void Jump(Input& input);

	std::map<std::string, int> imgkey_;
	
	AnimStr animeStr_;

	Input input_;

	int padNum_;//自分が何番目のPADを使っているか

	DIR_LR dir_LR_;//キャラクターの向き
	Vector2DFloat pos_;//キャラの座標
	Vector2DFloat oldPos_;//前のキャラ座標
	Vector2DFloat center_;//キャラの中心座標
	ColList colList_;
	Raycast rayCast_;

	Vector2DFloat cameraPos_;//カメラの座標

	Vector2DFloat view = { 1200.0f, 800.0f };

	Vector2DFloat offset_;

	

	Vector2DFloat movePow_;
	Vector2DFloat moveVec_;

	//VECTOR movePow_;
	//VECTOR moveVec_;
};

