#pragma once

class Scene;
class Job;

class SceneManager final
{
	DECLARE_SINGLETON(SceneManager)

	PUBLIC bool TryChangeScene(Job* job);

	PUBLIC static void ChangeScene(Scene* scene);
	PUBLIC static void ReloadCurrentScene();
	PUBLIC static Scene* GetCurrentScene();

	PRIVATE Scene* m_currentScene;
	PRIVATE Scene* m_nextScene;
};

