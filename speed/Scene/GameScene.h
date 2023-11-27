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

    
    //��Ԑ擪�̃v���C���[�ɒǏ]������
    //�Q�[���V�[���Ő擪�����߂āA�J�����N���X�ɓn��
    void DecideOnTheBeginning();

    std::shared_ptr<Player> player_;
    std::vector<std::shared_ptr<Player>> players_;
    std::unique_ptr<Stage> stage_;
    std::unique_ptr<Camera> camera_;
    std::unique_ptr<OutSide> outSide_;
    std::unique_ptr<CheckPoint> checkPoint_;
    //std::unique_ptr<PostEfffect> postEfffect_;

    Vector2DFloat drawOffset_;
    Vector2DFloat FirstPos_;

    Vector2DFloat CheckPoint_;//�������ׂ�����
    std::vector<Vector2DFloat> CheckPoints_;

    std::vector<float> distance_;

    int playerNum_;

    PLAYER_NUM old_Num_;
    PLAYER_NUM new_Num_;

    int comparison_NUM_;
    PLAYER_NUM last_Num_;
};
