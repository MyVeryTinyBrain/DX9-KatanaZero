#include "base_stdafx.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Job.h"
#include "Time.h"

#define INST (g_instance)

IMPLEMENT_SINGLETON(SceneManager)

IMPLEMENT_SINGLETON_CONSTRUCTOR(SceneManager)
{
	m_currentScene = nullptr;
	m_nextScene = nullptr;
}

IMPLEMENT_SINGLETON_DESTRUCTOR(SceneManager)
{
    SAFE_DELETE(m_currentScene);
    SAFE_DELETE(m_nextScene);
}

bool SceneManager::TryChangeScene(Job* job)
{
    // 바꿀 씬이 존재하는 경우
    // 씬을 변경합니다.
    if (m_nextScene)
    {
        Scene* prev = m_currentScene;
        Scene* next = m_nextScene;

        // 이전 씬이 존재하는 경우
        // 이전 씬의 OnChange() 함수를 호출합니다.
        if (prev)
        {
            prev->OnChange(next);
        }

        // 모든 오브젝트를 제거합니다.
        job->DeleteAll();

        // 현재 씬을 설정하고 OnLoad() 함수를 호출합니다.
        m_currentScene = next;
        next->OnLoad(prev);

        // 씬 로드중에 시간이 오래 걸릴 것에 대비하여 시간을 씬 불러오기가 끝난 후로 재설정합니다.
        Time::GetInstance()->Reset();

        // 다음 씬을 비웁니다.
        m_nextScene = nullptr;

        // 이전 씬을 제거합니다.
        SAFE_DELETE(prev);

        return true;
    }

    return false;
}

void SceneManager::ChangeScene(Scene* scene)
{
    if (!scene) return;
    if (INST.m_nextScene)
    {
        SAFE_DELETE(INST.m_nextScene);
    }
    INST.m_nextScene = scene;
}

void SceneManager::ReloadCurrentScene()
{
    if (INST.m_currentScene)
        INST.ChangeScene(INST.m_currentScene->Copy());
}

Scene* SceneManager::GetCurrentScene()
{
	return INST.m_currentScene;
}
