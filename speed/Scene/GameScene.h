#pragma once
#include "Scene.h"
#include<memory>
#include<vector>
#include"../Object/OutSide.h"
#include"../Object/Camera.h"
#include"../Object/Stage/Stage.h"
#include"../Player/Player.h"
#include"../Input/Input.h"



class GameScene :
    public Scene
{
public:

    enum class PLAYER_NUM
    {
        P_1,
        P_2,
        P_3,
        P_4
    };

    GameScene(SceneMng& manager);

    virtual void Update(Input& input) override;
    virtual void Draw() override;
     void DrawOwnScreen() ;
private:

    
    //一番先頭のプレイヤーに追従させる
    //ゲームシーンで先頭を決めて、カメラクラスに渡す
    void DecideOnTheBeginning();
    void DecideOnTheLastPerson();


    std::shared_ptr<Player> player_;
    std::vector<std::shared_ptr<Player>> players_;
    std::unique_ptr<Stage> stage_;
    std::unique_ptr<Camera> camera_;
    std::unique_ptr<OutSide> outSide_;

    Vector2DFloat drawOffset_;
    int playerNum_;

    PLAYER_NUM old_Num_;
    PLAYER_NUM last_Num_;
    PLAYER_NUM new_Num_;
};
