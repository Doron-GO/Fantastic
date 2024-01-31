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

    int soundH_[2];
    bool startFlag_;
    int titleImg_;
    int startImg_;
    int selectImg_[4];
    int num_;
};

