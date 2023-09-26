#include "editor_stdafx.h"
#include "EditorScene.h"
#include "EditorManager.h"

void EditorScene::OnLoad(Scene* prevScene)
{
	auto* managerObj = new GameObject;
	managerObj->AddComponent<EditorManager>();
}

void EditorScene::OnChange(Scene* nextScene)
{
}

Scene* EditorScene::Copy()
{
	return new EditorScene;
}
