#pragma once

class TimeElement;
class DeltaTimeElement;
class FixedDeltaTimeElement;

class Time
{
	DECLARE_SINGLETON(Time)

	PUBLIC void Initalize(float update_fps, float physics_fps);
	PUBLIC void Reset();

	PUBLIC DWORD DeltaTimeAccumulate();
	PUBLIC DWORD FixedDeltaTimeAccumulate();

	PUBLIC void SetUpdateFPS(float fps);
	PUBLIC float GetUpdateFPS() const;
	PUBLIC void SetPhysicsFPS(float fps);
	PUBLIC float GetPhysicsFPS() const;

	PUBLIC static float GetDeltaTime();
	PUBLIC static float GetUnscaledDeltaTime();
	PUBLIC static float GetTimeScale();
	PUBLIC static void SetTimeScale(float timeScale);
	PUBLIC static float GetFixedDeltaTime();
	PUBLIC static float GetUnscaledFixedDeltaTime();
	PUBLIC static float GetFixedTimeScale();
	PUBLIC static void SetFixedTimeScale(float timeScale);

	PUBLIC static float GetUpdateInterval();
	PUBLIC static float GetFixedUpdateInterval();
	PUBLIC static float GetUpdateAccumulated();
	PUBLIC static float GetFixedUpdateAccumulated();

	PRIVATE TimeElement* m_deltaTime;
	PRIVATE float m_updateFPS;
	PRIVATE TimeElement* m_fixedDeltaTime;
	PRIVATE float m_physicsFPS;
};

