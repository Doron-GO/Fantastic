#pragma once
#include "Scene.h"
#include<memory>
#include<vector>
#include"../Object/Camera/OutSide.h"
#include"../Object/Camera/Camera.h"
#include"../Object/Stage/Stage.h"
#include"../Player/Player.h"
#include"../Input/Input.h"
#include"../Object/Stage/CheckPoint.h"
#include"../Object/Shader/PostEfffect.h"



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
    void TestDecideOnTheBeginning();

    std::shared_ptr<Player> player_;
    std::vector<std::shared_ptr<Player>> players_;
    std::unique_ptr<Stage> stage_;
    std::unique_ptr<Camera> camera_;
    std::unique_ptr<OutSide> outSide_;
    std::unique_ptr<CheckPoint> checkPoint_;

    PLAYER_NUM old_Num_;//1フレーム前の最前列のプレイヤー
    PLAYER_NUM new_Num_;//最新の最前列のプレイヤー
    PLAYER_NUM last_Num_;//最後尾のプレイヤー

    std::vector<float> distance_;
    std::vector< std::pair<int, float>> testDistance_;//firstはパッドナンバー:secondはdistance
    std::pair<int, float> iD_;

};
