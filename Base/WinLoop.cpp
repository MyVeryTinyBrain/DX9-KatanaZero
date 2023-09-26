#include "base_stdafx.h"
#include "WinLoop.h"
#include "GraphicDevice.h"
#include "Screen.h"
#include "Time.h"
#include "SceneManager.h"
#include "Job.h"
#include "Physics.h"
#include "Input.h"
#include "TextureManager.h"

WinLoop::WinLoop(bool windowed)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    m_message.message = WM_NULL;

    m_job = new Job;

    // ��ġ�� �ʱ�ȭ�մϴ�.
    GraphicDevice::GetInstance()->Initialize(Screen::GetWidth(), Screen::GetHeight(), Screen::GetHWND(), windowed);

    // ������Ʈ �����Ӱ� ���� ������Ʈ �������� �����մϴ�.
    Time::GetInstance()->Initalize(144, 60);
}

WinLoop::~WinLoop()
{
    SAFE_DELETE(m_job);

    GraphicDevice::GetInstance()->Release();
}

void WinLoop::Loop()
{
    while (m_message.message != WM_QUIT)
    {
        if (PeekMessage(&m_message, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&m_message);
            DispatchMessage(&m_message);
        }

        ManualLoop();
    }
}

void WinLoop::ManualLoop()
{
    DWORD fixedUpdate = Time::GetInstance()->FixedDeltaTimeAccumulate();
    DWORD update = Time::GetInstance()->DeltaTimeAccumulate();

    // ������Ʈ, Ȥ�� ���� ������Ʈ�� �����ؾ� �ϴ� ��쿡
    if (fixedUpdate + update > 0)
    {
        m_job->Prepare();
        m_job->Start();

        if (fixedUpdate)
        {
            m_job->BeforeFixedStep();
        }

        if (fixedUpdate)
        {
            constexpr DWORD maxIteration = 5;

            // ������ Ƚ�� �̻� ���� ������ �ؾ� �ϴ� ��쿡��
            // ���� Ƚ���� �����մϴ�.
            if (fixedUpdate >= maxIteration)
                fixedUpdate = maxIteration;

            // ������ ���� Ƚ����ŭ ���� ������ �մϴ�.
            for (DWORD i = 0; i < fixedUpdate; ++i)
                Physics::GetInstance()->Step(Time::GetFixedDeltaTime());
        }

        if (fixedUpdate)
        {
            m_job->PhysicsEvent();
        }

        m_job->DeleteGarbages();

        if (fixedUpdate)
        {
            m_job->FixedUpdate();
            m_job->LateFixedUpdate();
        }

        if (update)
        {
            m_job->Update();
            m_job->LateUpdate();
            m_job->AfterUpdate();
            GraphicDevice::GetInstance()->BeginRender();
            m_job->BeforeRender();
            m_job->Render();
            m_job->PhysicsRender();
            GraphicDevice::GetInstance()->EndRender();
            m_job->AfterRender();
        }

        // ��ǲ�� �缳���մϴ�.
        if (update)
        {
            Input::GetInstance()->SetUsed();
        }

        // ���� �ٲ��� �ϴ� ��� ���� �����մϴ�.
        // �̋� ��� ���ӿ�����Ʈ�� ������Ʈ���� �����մϴ�.
        SceneManager::GetInstance()->TryChangeScene(m_job);
    }
}

MSG WinLoop::GetLastMessage()
{
    return m_message;
}
