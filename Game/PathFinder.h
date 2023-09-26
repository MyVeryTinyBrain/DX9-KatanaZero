#pragma once

#include "StageGraphStruct.h" 

class PlatformerBody;

class PathFinder : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(FixedUpdate);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(OnDestroy);
	PRIVATE void OnDestroySpriteRenderer(Object* object);

	PUBLIC void SetTarget(Vec2 target);
	PUBLIC void SetTargetWithPlatformerBody(PlatformerBody* body);
	PUBLIC void SetAutoTarget(PlatformerBody* body);
	PUBLIC void SetSpeed(float speed);
	PUBLIC void ClearPath();

	PUBLIC void Pause();
	PUBLIC void Resume();
	PUBLIC bool IsPause() const;
	PUBLIC bool IsUsingDoor() const;

	PUBLIC float GetHorizontalDirection() const;
	PUBLIC void SetHorizontalDirection(float hd);

	PUBLIC bool IsLastPosition() const;
	PUBLIC bool HasPath() const;

	PUBLIC void AddManagedSpriteRenderer(SpriteRenderer* renderer);
	PUBLIC void RemoveAlphaManagedSpriteRenderer(SpriteRenderer* renderer);

	PUBLIC void SetMinDistance(float md);
	PUBLIC float GetMinDistance() const;

	PUBLIC void CancelEnterDoor();

	PRIVATE void SetSpriteRenderersDirection(float hd);
	PRIVATE void SetSpriteRenderersAlpha(float a);

	PRIVATE Vec2 GetBottom() const;
	PRIVATE void SetBottom(Vec2 position) const;

	PRIVATE void UseDoorProcess();
	PRIVATE void AutoTargetProcess();
	PRIVATE void PathFindProcess();
	PRIVATE void UpdatePathProcess();
	PRIVATE void EnterDoor();

	PRIVATE void ThreadWork();

	PRIVATE PlatformerBody* m_body;
	PRIVATE PlatformerBody* m_autoTarget;
	PRIVATE float m_autoTargetCounter;
	PRIVATE bool m_hasTarget;
	PRIVATE Vec2 m_target;
	PRIVATE Vec2 m_threadTarget;
	PRIVATE float m_speed;
	PRIVATE float m_minDistance;

	PRIVATE std::vector<StageGraphPathNode> m_path;
	PRIVATE size_t m_currentIndex;
	PRIVATE LineRenderer* m_debugLine;

	PRIVATE std::vector<SpriteRenderer*> m_managedSpriteRenderers;

	PRIVATE bool m_readyToEnterDoor;
	PRIVATE bool m_readyToExitDoor;
	PRIVATE Vec2 m_exitPos;
	PRIVATE float m_exitHDir;
	PRIVATE float m_enterCounter;
	PRIVATE float m_exitCounter;
	PRIVATE float m_doorUseTime;

	PRIVATE float m_prevHD;

	PRIVATE bool m_pause;

	PRIVATE std::vector<StageGraphPathNode> m_threadPath;
	PRIVATE std::thread m_thread;
	PRIVATE std::mutex m_mutex;
	PRIVATE std::condition_variable m_cv;
	PRIVATE bool m_stopThread;

	// void OnBeginToTurn(float turnDirection)
	PUBLIC Delegate<void, float> OnBeginToTurn;

	PUBLIC Delegate<void> OnBeginToEnterDoor;

	PUBLIC Delegate<void> OnBeginToExitDoor;
};

