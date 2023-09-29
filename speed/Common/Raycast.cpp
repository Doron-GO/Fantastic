#include "Raycast.h"

bool Raycast::CheckCollision(Ray ray, Collision collision)
{// ���㉺���E�̈��

    Raycast::Line lines[4] = {  

    //��̕�   
    {collision.first,
        collision.first + Vector2DFloat{collision.second.x,0}},
    //�E��
    {collision.first + Vector2DFloat{collision.second.x,0},
            collision.first + collision.second},
    //���̕�
    { collision.first + collision.second ,
    collision.first + Vector2DFloat{0,collision.second.y}},
    //����
    { collision.first + Vector2DFloat{0,collision.second.y},
    collision.first}
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
