#pragma once
#include "Scene.h"
#include"../Player/Player.h"
#include"../Obj/Camera.h"
#include<memory>
#include"../Obj/TmxObj.h"
#include"../Obj/Camera.h"

class GameScene :
    public Scene
{
public:
    GameScene(SceneMng& manager);

    virtual void Update(Input& input) override;
    virtual void Draw() override;
     void DrawOwnScreen() ;
private:
    //std::shared_ptr<Player> player_;
    std::vector<std::shared_ptr<Player>> playerList_;
    std::unique_ptr<Camera> camera_;
    TmxObj tmxObj_;
    Vector2DFloat drawOffset_;

};

