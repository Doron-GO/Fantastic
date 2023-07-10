#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Common/Flame.h"
#include "../Input/Controller.h"
#include "../Object/Common/Capsule.h"
#include "../Object/Common/Collider.h"
#include "../Object/SkyDome.h"
#include "../Object/Stage.h"
#include "../Object/StagePiece.h"
#include "../Object/Player.h"
#include "GameScene.h"
#include "../Object/Common/Grid.h"
GameScene::GameScene(void)
	:SceneBase()
{
	player_ = nullptr;
}

void GameScene::Init(void)
{
	isLoaded = false;
	//�X�e�[�W
	stage_ = std::make_unique<Stage>();
	//�v���C���[
	player_ = new Player();
	// �X�J�C�h�[��
	skyDome_ = std::make_unique<SkyDome>();
	// �O���b�h��
	grid_ = std::make_unique<Grid>();

	stage_->Init();

	player_->Init();

	for (int i =1; i<21;i++ )
	{ 
		auto p = stage_->GetPiece();
		auto oo = p[static_cast<Stage::STAGE_NUM>(i-1)];
		player_->AddCollider(oo->GetTransform()->collider);
	}


	skyDome_->Init();

	grid_->Init();
	//�J�����̐ݒ�
	lpSceneMng.GetCamera()->ChangeMode(Camera::MODE::FOLLOW);

	lpSceneMng.GetCamera()->SetTransform(player_->GetTransform());
	isLoaded_ = true;

	//loadThread_ = std::thread(&GameScene::Load, this);
	//Load();
}

void GameScene::Update(void)
{
	
	//controller_->Update();
	// �V�[���J��
	//auto contData = controller_->GetInputData();
	//if (contData[NowFlame][static_cast<int>(InputID::Btn3)] &&
	//	!contData[OldFlame][static_cast<int>(InputID::Btn3)])

	if (!isLoaded_)
	{ return; 

	}

	auto& ins = InputManager::GetInstance();
	if (ins.IsNew(KEY_INPUT_SPACE))
	{
		lpSceneMng.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}


	stage_->Update();
	player_->Update(SceneManager::GetInstance().GetDeltaTime(), SceneManager::GetInstance().GetCamera()->GetPos());

	skyDome_->Update();

}

void GameScene::Draw(void)
{
	if (!isLoaded_) 
	{
		DrawFormatString(200, 200, 0xffffff, "Now Loading");
		return;
	}
	
	//skyDome_->Draw();
	stage_->Draw();
	player_->Draw();
	grid_->Draw();
	// �f�o�b�O�\��
	//DrawDebug();

}

void GameScene::DrawDebug(void)
{
	DrawString(0, 0, "GameScene", 0xffffff);
}

void GameScene::Release(void)
{	
	player_->Release();
	delete player_;

	skyDome_->Release();
	stage_->Release();

}

void GameScene::Load(void)
{



	//�X�e�[�W
	//stage_ = std::make_unique<Stage>();
	stage_->Init();

	//�v���C���[
	//player_ = new Player();
	player_->Init();
	//player_->AddCollider(stage_->GetTransform()->collider);

	// �X�J�C�h�[��
	//skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();

	// �O���b�h��
	//grid_ = std::make_unique<Grid>();
	grid_->Init();
	//�J�����̐ݒ�
	lpSceneMng.GetCamera()->ChangeMode(Camera::MODE::FOLLOW);

	lpSceneMng.GetCamera()->SetTransform(player_->GetTransform());

	isLoaded_ = true;

}
