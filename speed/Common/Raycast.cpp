#include "Raycast.h"
#include"../_debug/_DebugDispOut.h"
#include<DxLib.h>
bool Raycast::CheckCollision(Ray ray, Collision collision, Vector2DFloat playerPos)
{// ���㉺���E�̈��
    Vector2DFloat view = { 1200.0f, 800.0f };
    auto offset = (view / 4.0f) - playerPos;
    Raycast::Line lines[4] = {  

    //��̕�   
    {collision.first,
        (collision.first )+ Vector2DFloat{collision.second.x,0} },
    //�E��
    {(collision.first) + Vector2DFloat{collision.second.x,0} ,
            collision.first + collision.second },
    //���̕�
    { collision.first + collision.second ,
    collision.first + Vector2DFloat{0,collision.second.y } },
    //����
    { collision.first + Vector2DFloat{0,collision.second.y} ,
    collision.first }
    };

    bool reslut = false;
    for (auto line : lines)
    {
        reslut |=   CheckRay(ray,line, playerPos);
    }
    return reslut;
}

bool Raycast::CheckCollision(Collision collision, Vector2DFloat playerPos)
{

    bool reslut = false;
    reslut |= CheckRectangle(collision, playerPos);

    return reslut;
}

bool Raycast::CheckRay(Ray ray, Line line, Vector2DFloat offset)
{

    //�f�o�b�O�p�̎��ۂ̓����蔻���Ԃ����C���[�t���[���ŕ\������
    //_dbgDrawLine(
    //    static_cast<int>(line.p.x ),
    //    static_cast<int>(line.p.y ),
    //    static_cast<int>(line.end.x ),
    //    static_cast<int>(line.end.y ),
    //    0xff0000
    //);

    auto checkCross = [](Vector2DFloat vec1, Vector2DFloat& vec2)
    {
        return (vec1.x*vec2.y)-(vec2.x*vec1.y);
    };
    
    auto veclLine= (line.end) - (line.p );

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

bool Raycast::CheckRectangle(Collision collision, Vector2DFloat pos)
{
    if (collision.first.y < pos.y && collision.first.x<pos.x &&
        collision.second.y>pos.y && collision.second.x > pos.x)
    {
        return true;
    }
    return false;
}
