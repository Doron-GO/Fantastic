#pragma once
#include<map>
#include<string>
#include"../Vector2D.h"
#include"../Object/AnimMng.h"
#include"../Common/Collision.h"
#include"../Common/Raycast.h"
#include"../Input/Input.h"
#include"../Object/Camera.h"

class Wire;


class Player
{
public:

	enum class DIR_LR
	{
		RIGHT,
		LEFT
	};
	enum class PHASE
	{
		FALL,
		JUMP,
		MOVE,
		WALLSLIDE,
		WALLJUMP,
		SWING,
		SWINGFALL
	};

	 Player(int playerNum);
	 ~Player();
	 void Init(GrndColList colList, WallColList wallColList);
	 void Update(Input& input);
	 void Draw(Vector2DFloat cameraPos);//これにオフセット値を渡し描画をずらすようにする
	 const Vector2DFloat GetPos();


	GrndColList grndColList_;//特にギミックのない当たり判定
	WallColList wallcolList_;//壁ジャンプができる当たり判定


	const Vector2DFloat GetDiagonallyVecVec();
	const Vector2DFloat GetMoveVec();
	const Vector2DFloat GetMovePow();
	Vector2DFloat pos_;//キャラの座標
	Vector2DFloat movePow_;	//移動する力
	int padNum_;//自分が何番目のPADを使っているか
	DIR_LR dir_LR_;//キャラクターの向き

	void StartSwingJump();

private:

	PHASE phase_;
	std::string now_;

	void DebugPhaseCheck();

	void (Player::* _phase)(Input& input);
	void (Player::* _draw)();

	void IdlePhase(Input& input);
	void MovePhase(Input& input);//左右移動：ジャンプ
	void JumpPhese(Input& input);//ジャンプ
	void FallPhase(Input& input);//落下中
	void WallSlidePhese(Input& input);//壁ずり落ち
	void WallJumpPhese(Input& input);//壁ジャンプ
	void SwingPhese(Input& input);//スイング状態
	void SwingJumpPhese(Input& input);//スイングジャンプ状態
	//自分の中心から true 当たってない:false 当たってる
	bool Collision();
	//足元から　true 当たってない:false 当たってる
	bool Collision(Vector2DFloat movevec);


	bool ColWallSlide(Vector2DFloat movevec);

	void IdleDraw();
	void JumpDraw();
	void MoveDraw();
	void Move(Input& input);	//左右移動

	void Anchoring(Input& input);//フックを飛ばす:進行方向の斜め上
	void Jump(Input& input);

	std::map<std::string, int> imgkey_;
	
	AnimStr animeStr_;
	Input input_;

	Vector2DFloat center_;//キャラの中心座標
	Raycast rayCast_;

	Vector2DFloat cameraPos_;//カメラの座標
	Vector2DFloat moveVec_;	//向いている方向
	Vector2DFloat diagonallyVec_;	//向いている方向

	std::unique_ptr<Wire> wire_;

	float slideY_ = -35.0f;	
	int test= 0xffffff;
};

