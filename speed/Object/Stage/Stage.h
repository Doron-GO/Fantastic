#pragma once
#include"LoadMap.h"
#include"../../Vector2D.h"

class Stage
{
public:

	struct Line {
		Vector2DFloat p; //始点
		Vector2DFloat end;//終点

	};

	Stage();
	~Stage();
	void Init();
	void Draw(Vector2DFloat cameraPos);

	const  Vector2D& GetWorldArea();
	const  Vector2D& GetTileSize();
	const std::string& GetMapKey();
	const ColList& GetColList(void);
	const ColList& GetWallColList(void);
	const ColList& GetWireColList(void);

private:

	LoadMap loadMap_;

};

