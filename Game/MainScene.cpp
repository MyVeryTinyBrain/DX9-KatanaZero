#include "stdafx.h"
#include "MainScene.h"
#include "MainSceneScreen.h"

void MainScene::OnLoad(Scene* prevScene)
{
	GameObject* mainSceneScreenObject = new GameObject;
	mainSceneScreenObject->AddComponent<MainSceneScreen>();
}

void MainScene::OnChange(Scene* nextScene)
{
}

Scene* MainScene::Copy()
{
	return new MainScene;
}
