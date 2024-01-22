#pragma once
#include "Scene.h"
#include<memory>
#include"Transition/TileTransitor.h"


class TitleScene :
    public Scene
{
public:
    TitleScene(SceneMng& manager, int n, Transitor& transit);
    ~TitleScene();
    virtual void Update(Input& input) override;
    virtual void Draw() override;

private:

    int titleImg_;
    int num_;
};

