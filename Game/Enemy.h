#pragma once

class PlatformerBody; 
class PathFinder;
class EnemyAnimator;
class EnemyDeadBody;

class Enemy : public Component
{
	PUBLIC enum class State { NONE, SEARCH, TRACE, LOST_TARGET };

	PROTECTED virtual void InitAnimator(EnemyAnimator** pAnimator) = 0;
	PROTECTED virtual void InitSpeed(float* pWalkSpeed, float* pRunSpeed) = 0;
	PROTECTED virtual void InitMelee(bool* pMeleeKill, bool* pHardMelee, float* pMeleeRange, float* pMeleeWait) = 0;
	PROTECTED virtual void InitLongRangeAttack(bool* pUse, float* pLongRangeAttackRange) = 0;
	PROTECTED virtual void InitShootDelay(float* pFirstShootDelay, float* pShootDelay) = 0;
	PROTECTED virtual bool OnDamaged(Vec2 attackDir) = 0;

	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Start);
	COMPONENT_OVERRIDE(FixedUpdate);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(BeforeRender);
	COMPONENT_OVERRIDE(OnDestroy);
	PRIVATE void OnBeginToTurn(float hd);
	PRIVATE void OnBeginToEnterDoor();
	PRIVATE void OnBeginToExitDoor();
	PRIVATE void OnTriggerEnter(Collider* collider);
	PRIVATE void OnCollisionEnter(const CollisionResult& collision);

	PUBLIC void SetHorizontalDirection(float hd);

	PUBLIC bool Damage(Vec2 velocity, Component* damageObject, bool canBlock = false);

	PUBLIC void ChangeState(State state);
	PUBLIC State GetState() const;

	PUBLIC void SetRallyPoints(Vec2 p1, Vec2 p2);
	PUBLIC void SetStandingPoint(Vec2 p);
	PUBLIC bool HasRallyPoints() const;
	PUBLIC bool HasStandingPoint() const;

	PUBLIC float GetHorizontalDirection() const;
	PUBLIC bool IsHurt() const;
	PUBLIC bool IsGroggy() const;

	PUBLIC int GetEnemyID() const;
	PUBLIC SpriteRenderer* GetRenderer() const;

	PRIVATE void Search();
	PRIVATE void TracePlayer();
	PRIVATE void Melee();
	PRIVATE void LongRangeAimming();
	PRIVATE void Shoot(float speed, float adjustAngle);

	PRIVATE bool DetectPlayer();

	PROTECTED PlatformerBody* m_body;
	PRIVATE PathFinder* m_pathFinder;
	PRIVATE SpriteRenderer* m_mainRenderer;
	PROTECTED EnemyAnimator* m_animator;

	PRIVATE State m_state;

	// 랠리 포인트의 갯수입니다.
	// 카운트가 한 개이면 해당 위치에 서있게 되고
	// 카운트가 두 개라면 두 점을 순찰하게 됩니다.
	PRIVATE size_t m_rallyPointCount;
	PRIVATE Vec2 m_rallyPoint[2];
	PRIVATE int m_rallyPointIndex;

	PRIVATE float m_restCounter;

	PRIVATE float m_updateTracePathCounter;
	PRIVATE float m_detectionRange;
	PRIVATE float m_walkSpeed;
	PRIVATE float m_runSpeed;
	PRIVATE bool m_useLongRangeAttack;
	PRIVATE float m_meleeRange;
	PRIVATE bool m_meleeKill;
	PRIVATE float m_meleeWait;
	PRIVATE bool m_hardMelee;
	PRIVATE float m_longRangeAttackRange;
	PRIVATE float m_firstShootDelay;
	PRIVATE float m_shootDelay;
	PRIVATE float m_shootCount;
	PROTECTED int m_bulletCount;
	EnemyDeadBody* m_deadBody;
	PRIVATE float m_dustCreationCounter;
	PRIVATE float m_bloodCreationCounter;
	PRIVATE float m_slowTimeTrailCounter;
	PRIVATE int m_enemyID;

	PRIVATE Vec2 m_prevVel;

	PUBLIC static std::vector<Enemy*> g_enemies;
};

