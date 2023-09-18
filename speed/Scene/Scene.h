#pragma once

class Input;
class SceneMng;
/// <summary>
/// �V�[���̋K��N���X(����1�N���X
/// </summary>

class Scene
{
protected:
	SceneMng& sceneManager_;

public:
	Scene(SceneMng& scene);
	virtual void Update(Input& input) = 0;
	virtual void Draw() = 0;
};

