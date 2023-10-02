#pragma once
#include "Scene.h"
class TitleScene :
    public Scene
{
public:
    TitleScene(SceneMng& manager);

    virtual void Update(Input& input) override;
    virtual void Draw() override;
private:
};

