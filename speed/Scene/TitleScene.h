#pragma once
#include "Scene.h"
class TitleScene :
    public Scene
{
public:
    TitleScene(SceneMng& manager, int n);

    virtual void Update(Input& input) override;
    virtual void Draw() override;
    virtual void DrawOwnScreen(void)override;

private:

    int titleImg_;
    int num_;
};

