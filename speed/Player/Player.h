#pragma once
#include<map>
#include<string>
#include"../Vector2D.h"
class Input;

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

	std::map<std::string, int> imgkey_;
	
	//走っている画像
	int runImg_[6];
	//ラン画像要のカウント用
	int runFrame_;

	Vector2DFloat pos_;//プレイヤーの座標

};

