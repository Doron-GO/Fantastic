#pragma once
#include<map>
#include<string>
#include"../Vector2D.h"
#include"../Object/AnimMng.h"
#include"../Common/Collision.h"
#include"../Common/Raycast.h"
#include"../Input/Input.h"
#include"../Object/Camera.h"
enum class DIR_LR
{
	RIGHT,
	LEFT
};

class Player
{
public:
	 Player(int playerNum);
	 ~Player();
	 void Init(GrndColList colList, WallColList wallColList);
	 void Update(Input& input);
	 void Draw(Vector2DFloat cameraPos);//これにオフセット値を渡し描画をずらすようにする
	 const Vector2DFloat GetPos();

	 int headFlag_;//自分が先頭かどうか

private:

	void (Player::* _phase)(Input& input);
	void (Player::* _draw)();

	void IdlePhase(Input& input);
	void MovePhase(Input& input);//左右移動：ジャンプ
	void JumpPhese(Input& input);//ジャンプ
	void FallPhase(Input& input);//落下中
	void WallSlidePhese(Input& input);//壁ずり落ち
	void WallJumpPhese(Input& input);//壁ジャンプ
	//自分の中心から true 当たってない:false 当たってる
	bool Collision();
	//足元から　true 当たってない:false 当たってる
	bool Collision(Vector2DFloat movevec);


	bool ColWallSlide(Vector2DFloat movevec);

	void IdleDraw();
	void JumpDraw();
	void MoveDraw();

	//左右移動
	void Move(Input& input);
	void Jump(Input& input);
	//bool Jump(Input& input);

	std::map<std::string, int> imgkey_;
	
	AnimStr animeStr_;
	Input input_;
	int padNum_;//自分が何番目のPADを使っているか

	DIR_LR dir_LR_;//キャラクターの向き
	Vector2DFloat pos_;//キャラの座標
	Vector2DFloat center_;//キャラの中心座標
	GrndColList colList_;
	WallColList wallcolList_;
	Raycast rayCast_;

	Vector2DFloat cameraPos_;//カメラの座標
	Vector2DFloat movePow_;	//移動する力
	Vector2DFloat moveVec_;	//向いている方向

	float slideY_ = -35.0f;	
	int test= 0xffffff;
};

