#pragma once
#include<memory>
#include<vector>
#include"../Vector2D.h"
#include"../Common/Collision.h"
#include"../Input/Input.h"

class Player;

using Players = std::vector<std::shared_ptr<Player>>;

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

	PlayerManager();
	~PlayerManager();

	void Update(Input& input);
	void Init(int playerNum, ColList gruound, ColList Wall, ColList wire);
	void Draw(Vector2DFloat cameraPos);
	const Vector2DFloat& GetPlayerPos(int playerNum);
	const Players GetPlayers();
	void DecideOnTheBeginning(Vector2DFloat checkPoint);

	const PLAYER_NUM GetOldLeadNum();
	const PLAYER_NUM GetNewLeadNum();
	const PLAYER_NUM GetLastLeadNum();
	void SetOld();
private:
	Players players_;
	std::vector<float> distance_;
	std::vector< std::pair<int, float>> testDistance_;//firstはパッドナンバー:secondはdistance
	std::pair<int, float> iD_;

	PLAYER_NUM old_LeadNum_;//1フレーム前の最前列のプレイヤー
	PLAYER_NUM new_LeadNum_;//最新の最前列のプレイヤー
	PLAYER_NUM last_Num_;//最後尾のプレイヤー

};

