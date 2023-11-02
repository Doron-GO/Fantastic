#include "Camera.h"
#include"../Player/Player.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

bool Camera::Init(const Vector2D& worldSize)
{
    _phase = &Camera::Follow;
    return true;
}

void Camera:: Update()
{
    (this->*_phase)();
}

void Camera::Switching()
{

    Vector2DFloat view = { 1600.0f, 1000.0f };
    Vector2DFloat offset;

    offset.x = (view.x / 4.0f) * 2.0f - taeget_.lock()->GetPos().x;
    offset.y = (view.y / 2.0f) - taeget_.lock()->GetPos().y;

    if (time <= 60.0f){time++;}
    cameraPos_.x = oldPos_.x * (1.0f - time / 60.0f) + offset.x * time / 60.0f;
    cameraPos_.y = oldPos_.y * (1.0f - time / 60.0f) + offset.y * time / 60.0f;
   
    if (time >= 60.0f)
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
    offset.x = (view.x / 4.0f) * 2.0f - taeget_.lock()->GetPos().x;
    offset.y = (view.y / 2.0f) - taeget_.lock()->GetPos().y;

    cameraPos_ = offset;

    oldPos_ = offset;
}

bool Camera::ReConnect(std::weak_ptr<Player> actor)
{
    taeget_ = actor;
    return true;
}

