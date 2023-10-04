#include "Raycast.h"
#include"../_debug/_DebugDispOut.h"
#include<DxLib.h>
bool Raycast::CheckCollision(Ray ray, Collision collision, Vector2DFloat offset)
{// ���㉺���E�̈��
    Vector2DFloat view = { 800.0f, 600.0f };
    auto offset_ = (view / 3.0f) - offset;
    Raycast::Line lines[4] = {  

    //��̕�   
    {collision.first+ offset_,
        (collision.first + offset_)+ Vector2DFloat{collision.second.x,0} },
    //�E��
    {(collision.first+ offset_) + Vector2DFloat{collision.second.x,0} ,
            collision.first + collision.second + offset_},
    //���̕�
    { collision.first + collision.second + offset_,
    collision.first + Vector2DFloat{0,collision.second.y } + offset_},
    //����
    { collision.first + Vector2DFloat{0,collision.second.y} + offset_,
    collision.first + offset_}
    };
    bool reslut = false;
    for (auto line : lines)
    {
        reslut |=   CheckRay(ray,line);
    }
    return reslut;
}

bool Raycast::CheckRay(Ray ray, Line line)
{
    
        _dbgDrawLine(
            static_cast<int>(line.p.x),
            static_cast<int>(line.p.y),
            static_cast<int>(line.end.x),
            static_cast<int>(line.end.y),
            0xff0000

        );

    auto checkCross = [](Vector2DFloat vec1, Vector2DFloat& vec2)
    {
        return (vec1.x*vec2.y)-(vec2.x*vec1.y);
    };
    
   auto veclLine= line.end - line.p;
    //ray.vec: �n�_����̕����x�N�g��
   auto crossRayLine= checkCross(ray.vec, veclLine);
   if (crossRayLine == 0.0)
   {
       //�O�ς̌��ʂ��O�̏ꍇ���t�͐�Ό������Ȃ��̂ł͂���
       return false;
   }

   Vector2DFloat v =  line.p- ray.p;

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
