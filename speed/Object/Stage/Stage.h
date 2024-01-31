#pragma once
#include<memory>
#include"LoadMap.h"
#include"../../Vector2D.h"

class ItemBox;
class Player;
class Blocks;

class Stage
{
public:

	struct Line {
		Vector2DFloat p; //始点
		Vector2DFloat end;//終点

	};

	Stage();
	~Stage();
	void Init(std::vector<std::shared_ptr<Player>> players);
	void Update();
	void Draw(Vector2DFloat cameraPos);

	const  Vector2D& GetWorldArea();
	const  Vector2D& GetTileSize();
	const std::string& GetMapKey();
	const ColList& GetColList(void);
	const ColList& GetWallColList(void);
	const ColList& GetWireColList(void);
	const PointColList& CheckPointGetColList(void);
	const  std::unique_ptr<Blocks> &GetBlocks();
private:

	LoadMap loadMap_;
	std::unique_ptr<ItemBox> itemBox_;
	std::unique_ptr<Blocks> blocks_;
	int backImg_;

};

