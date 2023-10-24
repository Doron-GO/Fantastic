#pragma once
#include "Scene.h"
#include"../Player/Player.h"
#include"../Obj/Camera.h"
#include<memory>
#include"../Obj/Camera.h"
#include"../Input/Input.h"
#include"../Obj/Stage.h"

class GameScene :
    public Scene
{
public:
    GameScene(SceneMng& manager);

    virtual void Update(Input& input) override;
    virtual void Draw() override;
     void DrawOwnScreen() ;
private:

    
    //一番先頭のプレイヤーに追従させる
    //ゲームシーンで先頭を決めて、カメラクラスに渡す


    //std::shared_ptr<Player> player_;
    std::vector<std::shared_ptr<Player>> players_;
    std::unique_ptr<Stage> stage_;
    std::unique_ptr<Camera> camera_;
    Vector2DFloat drawOffset_;
    
};
