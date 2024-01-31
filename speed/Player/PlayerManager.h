#pragma once
#include<memory>
#include<vector>
#include<algorithm>
#include"../Vector2D.h"
#include"../Common/Collision.h"
#include"../Input/Input.h"

class Player;
class Blocks;

using Players = std::vector<std::shared_ptr<Player>>;
using Vec = Vector2DFloat;
class PlayerManager
{
public:

	enum class PLAYER_NUM
	{
		P_1,
		P_2,
		P_3,
		P_4
	};

	PlayerManager(bool& conclusion, Blocks& blocks);
	~PlayerManager();

	void Update(Input& input);
	void Init(int playerNum, ColList gruound, ColList Wall, ColList wire);
	void Draw(Vector2DFloat cameraPos);
	const Vector2DFloat& GetPlayerPos(int playerNum);
	const Players GetPlayers();
	//先頭を探すよ
	void DecideOnTheBeginning2(std::pair<bool,Vector2DFloat>checkPoint);
	void HormingTargrt();
	void Conclusion();
	void Goal();

	//アイテムとの当たり判定をプレイヤー人数ぶん回す関数
	void ItemCol();

	const PLAYER_NUM GetOldLeadNum();
	const PLAYER_NUM GetNewLeadNum();
	const PLAYER_NUM GetLastLeadNum();
	void SetOld();

	//当たり判定をする関数
	bool IsItemCollision(Vec pMin, Vec pMax, Vec iMin, Vec iMax);
	//アイテムとの当たり判定上からの条件
	bool TopSide(Vec Max,Vec Min);
	bool DownSide(Vec Max,Vec Min);
	bool LeftSide(Vec Max,Vec Min);
	bool RightSide(Vec Max,Vec Min);

	void SinglePlay();
private:
	Players players_;
	std::vector< std::pair<int, float>> leadDistance_;//firstはパッドナンバー:secondはdistance
	std::vector< std::pair<float, int>> TTleadDistance_;//firstはパッドナンバー:secondはdistance
	std::vector<float> Distance_;
	std::pair<int, float> iD_;

	PLAYER_NUM old_LeadNum_;//1フレーム前の最前列のプレイヤー
	PLAYER_NUM new_LeadNum_;//最新の最前列のプレイヤー
	PLAYER_NUM last_Num_;//最後尾のプレイヤー


	bool &conclusion_;
	int winImg_;
	int restertImg_;
	int winner_;

	bool goalFlag_;
	Blocks& blocks_;
	bool singlePlay_=false;
	int count_;

	//int &playerCounts_;//生存数
};

