#pragma once
#include <memory>
#include <chrono>

class SceneBase;
class Fader;
class Camera;

#define lpSceneMng SceneManager::GetInstance()

class SceneManager
{

public:

	// �V�[���Ǘ��p
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME
	};
	
	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static SceneManager& GetInstance(void);

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// ��ԑJ��
	void ChangeScene(SCENE_ID nextId);

	// �V�[��ID�̎擾
	SCENE_ID GetmSceneID(void);

	// �f���^�^�C���̎擾
	float GetDeltaTime(void) const;

	// �J�����̎擾
	Camera* GetCamera(void) const;

private:

	// �ÓI�C���X�^���X
	static SceneManager* instance_;



	SCENE_ID sceneID_;
	SCENE_ID waitSceneID_;

	SceneBase* scene_;
	Fader* fader_;
	Camera* camera_;

	bool isSceneChanging_;

	// �f���^�^�C��
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;
	
	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	SceneManager(void);
	~SceneManager(void);

	void ResetDeltaTime(void);

	// �V�[���J��
	void DoChangeScene(SCENE_ID sceneId);

	// �t�F�[�h
	void Fade(void);

};