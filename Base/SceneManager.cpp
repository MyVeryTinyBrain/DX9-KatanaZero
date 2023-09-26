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
    // �ٲ� ���� �����ϴ� ���
    // ���� �����մϴ�.
    if (m_nextScene)
    {
        Scene* prev = m_currentScene;
        Scene* next = m_nextScene;

        // ���� ���� �����ϴ� ���
        // ���� ���� OnChange() �Լ��� ȣ���մϴ�.
        if (prev)
        {
            prev->OnChange(next);
        }

        // ��� ������Ʈ�� �����մϴ�.
        job->DeleteAll();

        // ���� ���� �����ϰ� OnLoad() �Լ��� ȣ���մϴ�.
        m_currentScene = next;
        next->OnLoad(prev);

        // �� �ε��߿� �ð��� ���� �ɸ� �Ϳ� ����Ͽ� �ð��� �� �ҷ����Ⱑ ���� �ķ� �缳���մϴ�.
        Time::GetInstance()->Reset();

        // ���� ���� ���ϴ�.
        m_nextScene = nullptr;

        // ���� ���� �����մϴ�.
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
