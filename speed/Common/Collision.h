#pragma once
#include<list>
#include "../Vector2D.h"


using Collision = std::pair<Vector2DFloat, Vector2DFloat>;//二点の場所
using ColList = std::list<Collision>;
using GrndColList = std::list<Collision>;
using WallColList = std::list<Collision>;
using WireColList = std::list<Collision>;


