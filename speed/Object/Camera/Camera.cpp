#include "Camera.h"
#include"../../Player/Player.h"
#include<algorithm>

Camera::Camera(): _phase (& Camera::Follow)
{
}

Camera::~Camera()
{
}

void Camera::Init(const Vector2D& worldSize)
{
    //_phase = &Camera::Follow;
}

void Camera:: Update()
{
    (this->*_phase)();
}

void Camera::Switching()
{
    Vector2DFloat view = { 1600.0f, 1000.0f };
    Vector2DFloat offset;
    offset.x = (view.x / 4.0f) * 2.0f - target_.lock()->GetPos().x;
    offset.y = (view.y / 2.0f) - (target_.lock()->GetPos().y);
    if (time <= MAXFRAME){time++;}
    cameraPos_.x = oldPos_.x * (1.0f - time / MAXFRAME) + offset.x * time / MAXFRAME;
    cameraPos_.y = oldPos_.y * (1.0f - time / MAXFRAME) + offset.y * time / MAXFRAME;
    if (time >= MAXFRAME)
    {
        _phase = &Camera::Follow;
        time = 0.0f;
    }
}

void Camera::PhaseChanging(int num)
{
    nextNum_ = num;
    _phase = &Camera::Switching;
}

void Camera::Follow()
{
    Vector2DFloat view = { 1600.0f, 1000.0f };
    Vector2DFloat offset;
    offset.x = (view.x / 4.0f) * 2.0f - target_.lock()->GetPos().x;
    offset.y = (view.y / 2.0f) - target_.lock()->GetPos().y;
    cameraPos_ = offset;
    oldPos_ = cameraPos_;
}

void Camera::Zoom()
{
    //std::clamp(,,,)
}

bool Camera::ReConnect(std::weak_ptr<Player> actor)
{
    target_ = actor;
    return true;
}


const Vector2DFloat& Camera::GetPos(void) const
{
    return cameraPos_; 
}

const Vector2DFloat& Camera::GetTargetPos(void) const
{
     return target_.lock()->GetPos(); 
}

