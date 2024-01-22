#pragma once
#include "Scene.h"
class ResultScene :
    public Scene
{
public:
    ResultScene(SceneMng& manager, int n, Transitor& transit);

    virtual void Update(Input& input) override;
    virtual void Draw() override;
private:
    int titleImg_;

};

