#pragma once
#include<list>
#include "../Vector2D.h"


using Collision = std::pair<Vector2DFloat, Vector2DFloat>;//“ñ“_‚ÌêŠ
using ColList = std::list<Collision>;
using GrndColList = std::list<Collision>;
using WallColList = std::list<Collision>;
using WireColList = std::list<Collision>;


