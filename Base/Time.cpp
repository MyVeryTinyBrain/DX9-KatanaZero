#include "base_stdafx.h"
#include "Time.h"
#include "TimeElement.h"

#define INST (g_instance)

IMPLEMENT_SINGLETON(Time)

IMPLEMENT_SINGLETON_CONSTRUCTOR(Time)
{
	m_deltaTime = new DeltaTimeElement(1.0f / 60.0f);
	m_fixedDeltaTime = new FixedDeltaTimeElement(1.0f / 60.0f);
	SetUpdateFPS(60);
	SetPhysicsFPS(60);
}

IMPLEMENT_SINGLETON_DESTRUCTOR(Time)
{
	SAFE_DELETE(m_deltaTime);
	SAFE_DELETE(m_fixedDeltaTime);
}

void Time::Initalize(float update_fps, float physics_fps)
{
	m_deltaTime->SetInterval(1.0f / update_fps);
	m_fixedDeltaTime->SetInterval(1.0f / physics_fps);
	Reset();
}

void Time::Reset()
{
	m_deltaTime->Reset();
	m_fixedDeltaTime->Reset();
}

DWORD Time::DeltaTimeAccumulate()
{
	return m_deltaTime->Accumulate();
}

DWORD Time::FixedDeltaTimeAccumulate()
{
	return m_fixedDeltaTime->Accumulate();
}

void Time::SetUpdateFPS(float fps)
{
	m_deltaTime->SetInterval(1.0f / fps);
	m_updateFPS = fps;
}

float Time::GetUpdateFPS() const
{
	return m_updateFPS;
}

void Time::SetPhysicsFPS(float fps)
{
	m_fixedDeltaTime->SetInterval(1.0f / fps);
	m_physicsFPS = fps;
}

float Time::GetPhysicsFPS() const
{
	return m_physicsFPS;
}

float Time::GetDeltaTime()
{
	return INST.m_deltaTime->GetDeltaTime();
}

float Time::GetUnscaledDeltaTime()
{
	return INST.m_deltaTime->GetUnscaledDeltaTime();
}

float Time::GetTimeScale()
{
	return INST.m_deltaTime->GetTimeScale();
}

void Time::SetTimeScale(float timeScale)
{
	INST.m_deltaTime->SetTimeScale(timeScale);
}

float Time::GetFixedDeltaTime()
{
	return INST.m_fixedDeltaTime->GetInterval() * INST.m_fixedDeltaTime->GetTimeScale();
}

float Time::GetUnscaledFixedDeltaTime()
{
	return INST.m_fixedDeltaTime->GetInterval();
}

float Time::GetFixedTimeScale()
{
	return INST.m_fixedDeltaTime->GetTimeScale();
}

void Time::SetFixedTimeScale(float timeScale)
{
	INST.m_fixedDeltaTime->SetTimeScale(timeScale);
}

float Time::GetUpdateInterval()
{
	return INST.m_deltaTime->GetInterval();
}

float Time::GetFixedUpdateInterval()
{
	return INST.m_fixedDeltaTime->GetInterval();
}

float Time::GetUpdateAccumulated()
{
	return INST.m_deltaTime->GetAccumulate();
}

float Time::GetFixedUpdateAccumulated()
{
	return INST.m_fixedDeltaTime->GetAccumulate();
}
