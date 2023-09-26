#pragma once

class TimeElement abstract
{
	PUBLIC TimeElement(float interval);
	PUBLIC ~TimeElement();

	PUBLIC virtual DWORD Accumulate() = 0;

	PUBLIC void Reset();
	PUBLIC void SetInterval(float interval);
	PUBLIC float GetInterval() const;
	PUBLIC float GetAccumulate() const;
	PUBLIC float GetDeltaTime() const;
	PUBLIC float GetUnscaledDeltaTime() const;
	PUBLIC void SetTimeScale(float timeScale);
	PUBLIC float GetTimeScale() const;
	PUBLIC LONGLONG GetTickPerSecond() const;
	PUBLIC LONGLONG GetTick() const;

	PROTECTED float m_interval;
	PROTECTED float m_accumulated;
	PROTECTED LONGLONG m_prevTick;
	PROTECTED float m_deltaTime;
	PROTECTED float m_timeScale;
};

class DeltaTimeElement : public TimeElement
{
	PUBLIC DeltaTimeElement(float interval);
	PUBLIC ~DeltaTimeElement();

	PUBLIC virtual DWORD Accumulate() override;
};

class FixedDeltaTimeElement : public TimeElement
{
	PUBLIC FixedDeltaTimeElement(float interval);
	PUBLIC ~FixedDeltaTimeElement();

	PUBLIC virtual DWORD Accumulate() override;
};