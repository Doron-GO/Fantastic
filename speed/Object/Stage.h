#pragma once
#include"TmxObj.h"
#include"LoadMap.h"
#include"../Vector2D.h"

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

private:

	TmxObj tmxObj_;
	LoadMap loadMap_;

};

