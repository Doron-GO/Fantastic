#pragma once
#include "Scene.h"
#include"../Player/Player.h"
#include"../Obj/Camera.h"
#include<memory>
#include"../Obj/TmxObj.h"
#include"../Obj/Camera.h"
#include"../Input/Input.h"

class GameScene :
    public Scene
{
public:
    GameScene(SceneMng& manager);

    virtual void Update(Input& input) override;
    virtual void Draw() override;
     void DrawOwnScreen() ;
private:

    
    //��Ԑ擪�̃v���C���[�ɒǏ]������
    //�Q�[���V�[���Ő擪�����߂āA�J�����N���X�ɓn��


    //std::shared_ptr<Player> player_;
    std::vector<std::shared_ptr<Player>> players_;
    std::unique_ptr<Camera> camera_;
    TmxObj tmxObj_;
    Vector2DFloat drawOffset_;
    
};
