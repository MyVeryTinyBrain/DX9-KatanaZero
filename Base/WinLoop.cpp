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

    // 장치를 초기화합니다.
    GraphicDevice::GetInstance()->Initialize(Screen::GetWidth(), Screen::GetHeight(), Screen::GetHWND(), windowed);

    // 업데이트 프레임과 고정 업데이트 프레임을 설정합니다.
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

    // 업데이트, 혹은 고정 업데이트를 시행해야 하는 경우에
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

            // 지정된 횟수 이상 물리 연산을 해야 하는 경우에는
            // 연산 횟수를 고정합니다.
            if (fixedUpdate >= maxIteration)
                fixedUpdate = maxIteration;

            // 지정된 연산 횟수만큼 물리 연산을 합니다.
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

        // 인풋을 재설정합니다.
        if (update)
        {
            Input::GetInstance()->SetUsed();
        }

        // 씬이 바뀌어야 하는 경우 씬을 변경합니다.
        // 이떄 모든 게임오브젝트와 컴포넌트들을 제거합니다.
        SceneManager::GetInstance()->TryChangeScene(m_job);
    }
}

MSG WinLoop::GetLastMessage()
{
    return m_message;
}
