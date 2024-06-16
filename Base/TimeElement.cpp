#include "base_stdafx.h"
#include "TimeElement.h"

TimeElement::TimeElement(float interval)
{
	m_interval = interval;
	Reset();
}

TimeElement::~TimeElement()
{
}

void TimeElement::Reset()
{
	m_accumulated = 0;
	m_prevTick = GetTick();
	m_deltaTime = 0;
	m_timeScale = 1;
}

void TimeElement::SetInterval(float interval)
{
	m_interval = interval;
}

float TimeElement::GetInterval() const
{
	return m_interval;
}

float TimeElement::GetAccumulate() const
{
	return m_accumulated;
}

float TimeElement::GetDeltaTime() const
{
	return m_deltaTime * m_timeScale;
}

float TimeElement::GetUnscaledDeltaTime() const
{
	return m_deltaTime;
}

void TimeElement::SetTimeScale(float timeScale)
{
	m_timeScale = timeScale;
}

float TimeElement::GetTimeScale() const
{
	return m_timeScale;
}

LONGLONG TimeElement::GetTickPerSecond() const
{
	LARGE_INTEGER pf;
	QueryPerformanceFrequency(&pf);
	return pf.QuadPart;
}

LONGLONG TimeElement::GetTick() const
{
	LARGE_INTEGER pc;
	QueryPerformanceCounter(&pc);
	return pc.QuadPart;
}

DeltaTimeElement::DeltaTimeElement(float interval) : TimeElement(interval)
{
}

DeltaTimeElement::~DeltaTimeElement()
{
}

DWORD DeltaTimeElement::Accumulate()
{
	LONGLONG currentTick = GetTick();
	// 이 함수가 다시 호출되기까지 걸린 시간입니다.
	LONGLONG elapsedTick = currentTick - m_prevTick;
	m_prevTick = currentTick;
	float elapsed = float(elapsedTick) / float(GetTickPerSecond());
	// 시간을 누적합니다.
	m_accumulated += elapsed;

	if (m_accumulated >= m_interval)
	{
		m_deltaTime = m_accumulated;
		m_accumulated = 0;
		return 1;
	}

	return 0;
}

FixedDeltaTimeElement::FixedDeltaTimeElement(float interval) : TimeElement(interval)
{
}

FixedDeltaTimeElement::~FixedDeltaTimeElement()
{
}

DWORD FixedDeltaTimeElement::Accumulate()
{
	LONGLONG currentTick = GetTick();
	LONGLONG elapsedTick = currentTick - m_prevTick;
	m_prevTick = currentTick;
	float elapsed = float(elapsedTick) / float(GetTickPerSecond());
	m_accumulated += elapsed;

	if (m_accumulated >= m_interval)
	{
		DWORD over = DWORD(m_accumulated / m_interval);
		m_accumulated -= over * m_interval;
		m_deltaTime = m_interval;
		return over;
	}

	return 0;
}
