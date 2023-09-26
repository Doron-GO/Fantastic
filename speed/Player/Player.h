#pragma once
#include<map>
#include<string>
#include"../Vector2D.h"
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

	//左右移動
	void Move(Input& input);
	void Jump(Input& input);

	std::map<std::string, int> imgkey_;
	
	//走っている画像
	int runImg_[6];
	//ラン画像要のカウント用
	int runFrame_;


	DIR_LR dir_LR_;//キャラクターの向き

	Vector2DFloat pos_another_way;//キャラの座標

	VECTOR pos_;
	VECTOR movePow_;
	VECTOR moveVec_;
};

