#pragma once
#include "Scene.h"
#include"../Player/Player.h"
#include<memory>
#include"../Obj/TmxObj.h"

class GameScene :
    public Scene
{
public:
    GameScene(SceneMng& manager);

    virtual void Update(Input& input) override;
    virtual void Draw() override;
private:

    std::unique_ptr<Player> player_;
    TmxObj tmxObj_;

};

