#pragma once
#include "ItemBase.h"
class Missile :
    public ItemBase
{
public:
    virtual void Update() override;
    virtual void Draw(Vector2DFloat offset)override;

private:
};

