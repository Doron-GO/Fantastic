#pragma once
#include<memory>
#include"LoadMap.h"
#include"../../Vector2D.h"
#include"../../Object/Item/ItemBox.h"

class ItemBox;
class Player;

class Stage
{
public:

	struct Line {
		Vector2DFloat p; //�n�_
		Vector2DFloat end;//�I�_

	};

	Stage(std::vector<std::shared_ptr<Player>>& players);
	~Stage();
	void Init();
	void Update();
	void Draw(Vector2DFloat cameraPos);

	const  Vector2D& GetWorldArea();
	const  Vector2D& GetTileSize();
	const std::string& GetMapKey();
	const ColList& GetColList(void);
	const ColList& GetWallColList(void);
	const ColList& GetWireColList(void);

private:

	LoadMap loadMap_;
	std::unique_ptr<ItemBox> itemBox_;
	std::vector<std::shared_ptr<Player>>& players_;

};

