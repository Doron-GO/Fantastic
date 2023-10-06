#include "Raycast.h"
#include"../_debug/_DebugDispOut.h"
#include<DxLib.h>
bool Raycast::CheckCollision(Ray ray, Collision collision, Vector2DFloat playerPos)
{// ↓上下左右の一辺
    Vector2DFloat view = { 800.0f, 600.0f };
    auto offset = (view / 3.0f) - playerPos;
    Raycast::Line lines[4] = {  

    //上の辺   
    {collision.first,
        (collision.first )+ Vector2DFloat{collision.second.x,0} },
    //右辺
    {(collision.first) + Vector2DFloat{collision.second.x,0} ,
            collision.first + collision.second },
    //下の辺
    { collision.first + collision.second ,
    collision.first + Vector2DFloat{0,collision.second.y } },
    //左辺
    { collision.first + Vector2DFloat{0,collision.second.y} ,
    collision.first }
    };
    ////上の辺   
    //{collision.first+ offset,
    //    (collision.first + offset)+ Vector2DFloat{collision.second.x,0} },
    ////右辺
    //{(collision.first+ offset) + Vector2DFloat{collision.second.x,0} ,
    //        collision.first + collision.second + offset},
    ////下の辺
    //{ collision.first + collision.second + offset,
    //collision.first + Vector2DFloat{0,collision.second.y } + offset},
    ////左辺
    //{ collision.first + Vector2DFloat{0,collision.second.y} + offset,
    //collision.first + offset}
    //};
    bool reslut = false;
    for (auto line : lines)
    {

        reslut |=   CheckRay(ray,line, offset);

    }

    return reslut;
}

bool Raycast::CheckRay(Ray ray, Line line, Vector2DFloat offset)
{
        _dbgDrawLine(
        static_cast<int>(line.p.x ),
        static_cast<int>(line.p.y ),
        static_cast<int>(line.end.x ),
        static_cast<int>(line.end.y ),
        0xff0000
    );

    auto checkCross = [](Vector2DFloat vec1, Vector2DFloat& vec2)
    {
        return (vec1.x*vec2.y)-(vec2.x*vec1.y);
    };
    
   //auto veclLine= line.end - line.p;
   
    auto veclLine= (line.end+offset) - (line.p + offset);

    //ray.vec: 始点からの方向ベクトル
   auto crossRayLine= checkCross(ray.vec, veclLine);
   if (crossRayLine == 0.0)
   {
       //外積の結果が０の場合平衡は絶対交差しないのではじく
       return false;
   }

   //Vector2DFloat v =  line.p- ray.p;
  
   Vector2DFloat v = (line.p + offset) - ray.p;

   auto cross_vRay  = checkCross(v, ray.vec);
   auto cross_vLine = checkCross(v, veclLine);

  auto t1 = cross_vRay / crossRayLine;
  auto t2 = cross_vLine / crossRayLine;

  if (t1 > 0.0 && t1 <= 1.0 && t2 > 0.0 && t2 <= 1.0)
  {
      return true;
  }

    return false;
}
