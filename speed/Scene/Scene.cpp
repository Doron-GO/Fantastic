#include "Scene.h"
#include<DxLib.h>
Scene::Scene(SceneMng& scene):sceneManager_(scene)
{
}

void Scene::Draw()
{
	DrawGraph(drawOffset_.x, drawOffset_.y, screenID_, true);

}
