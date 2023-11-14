#pragma once
#include"LoadMap.h"
#include"../../Vector2D.h"

class Stage
{
public:

	struct Line {
		Vector2DFloat p; //�n�_
		Vector2DFloat end;//�I�_

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

