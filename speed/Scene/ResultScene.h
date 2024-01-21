#pragma once
#include "Scene.h"
class ResultScene :
    public Scene
{
public:
    ResultScene(SceneMng& manager, int n);

    virtual void Update(Input& input) override;
    virtual void Draw() override;
private:

};

