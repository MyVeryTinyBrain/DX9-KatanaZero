#include "stdafx.h"
#include "Enemy.h"
#include "PlatformerBody.h"
#include "PathFinder.h"
#include "GameLayer.h"
#include "PlayerController.h"
#include "EnemyAnimator.h"
#include "GameTag.h"
#include "GameLayer.h"
#include "PlayerPusher.h"
#include "Bullet.h"
#include "EnemyDeadBody.h"
#include "GameRenderOrder.h"
#include "Door.h"
#include "StaticEffect.h"
#include "TimeController.h"
#include "TrailRenderer.h"
#include "SoundMgr.h"

std::vector<Enemy*> Enemy::g_enemies;

void Enemy::Awake() 
{
	m_enemyID = g_enemies.size() + 1;

	m_body = AddComponent<PlatformerBody>();
	m_body->GetBoxCollider()->OnTriggerEnter += Function<void, Collider*>(this, &Enemy::OnTriggerEnter);
	m_body->GetCircleCollider()->OnCollisionEnter += Function<void, const CollisionResult&>(this, &Enemy::OnCollisionEnter);
	m_body->GetBoxCollider()->OnCollisionEnter += Function<void, const CollisionResult&>(this, &Enemy::OnCollisionEnter);
	m_body->GetGameObject()->SetTag(TAG_ENEMY);

	GameObject* rendererObject = GameObject::CreateChild(GetGameObject());
	m_mainRenderer = rendererObject->AddComponent<SpriteRenderer>();
	m_mainRenderer->SetRenderOrder(RENDER_ORDER_ENEMY + m_enemyID * 5);
#ifdef SpriteRenderer
	m_mainRenderer->SetRecordRGBMode(false);
#endif

	m_pathFinder = AddComponent<PathFinder>();
	m_pathFinder->AddManagedSpriteRenderer(m_mainRenderer);
	m_pathFinder->OnBeginToTurn += Function<void, float>(this, &Enemy::OnBeginToTurn);
	m_pathFinder->OnBeginToEnterDoor += Function<void>(this, &Enemy::OnBeginToEnterDoor);
	m_pathFinder->OnBeginToExitDoor += Function<void>(this, &Enemy::OnBeginToExitDoor);

	m_bulletCount = 1;

	InitAnimator(&m_animator);
	InitSpeed(&m_walkSpeed, &m_runSpeed);
	InitMelee(&m_meleeKill, &m_hardMelee, &m_meleeRange, &m_meleeWait);
	InitLongRangeAttack(&m_useLongRangeAttack, &m_longRangeAttackRange);
	InitShootDelay(&m_firstShootDelay, &m_shootDelay);

	m_walkSpeed += rand() % 30 - 15;
	m_runSpeed -= rand() % 50 - 25;
	m_longRangeAttackRange += rand() % 50;

	m_rallyPointCount = 0;
	m_rallyPointIndex = 0;
	m_state = State::NONE;

	m_updateTracePathCounter = 0;
	m_detectionRange = 600;

	g_enemies.push_back(this);
}

void Enemy::Start() 
{
	ChangeState(State::SEARCH);
}

void Enemy::FixedUpdate()
{
	if (m_deadBody)
	{
		GetGameObject()->Destroy();
		return;
	}

	m_body->SetStickOnGround(!m_animator->IsPlayingHurt());

	if (m_animator->IsPlayingGroggy() || m_animator->IsPlayingHurt())
	{
		//m_body->SetUnrealisticPhysics(false);
		m_body->SetIgnoreRestitute(false);
		m_body->SetFriction(10);
	}
	else
	{
		//m_body->SetUnrealisticPhysics(true);
		m_body->SetIgnoreRestitute(true);
		m_body->SetFriction(1000);
	}

	m_prevVel = m_body->GetVeloicty();
}

void Enemy::Update()
{
	if (m_deadBody)
	{
		GetGameObject()->Destroy();
		return;
	}

	bool canMove = m_animator->CanMove();
	if(!canMove) m_pathFinder->Pause();
	else m_pathFinder->Resume();

	if (m_animator->IsDead() && fabsf(m_body->GetVeloicty().magnitude()) < 1.0f)
	{
		GameObject* deadBodyObject = new GameObject;
		m_deadBody = deadBodyObject->AddComponent<EnemyDeadBody>();
		m_deadBody->SetDeadBody(this);
		return;
	}

	if (m_animator->IsPlayingIdle())
	{
		m_restCounter -= Time::GetDeltaTime();
	}

	EnemyAnimatorHorizontal hstate = EnemyAnimatorHorizontal::NOTHING;
	switch (m_state)
	{
		case State::SEARCH:
			{
				m_pathFinder->SetSpeed(m_walkSpeed);
				Search();
				bool find = DetectPlayer();
				if (find)
				{
					ChangeState(State::TRACE);
					Melee();
					LongRangeAimming();
					break;
				}
				if (fabsf(m_body->GetVeloicty().x) > 10 && !m_animator->IsAimming())
					hstate = EnemyAnimatorHorizontal::WALK;
			}
			break;
		case State::TRACE:
			{
				m_pathFinder->SetSpeed(m_runSpeed);
				Melee();
				LongRangeAimming();
				if (fabsf(m_body->GetVeloicty().x) > 10 && !m_animator->IsAimming())
					hstate = EnemyAnimatorHorizontal::RUN;
			}
			break;
		case State::LOST_TARGET:
			break;
	}

	if (m_restCounter > 0)
	{
		m_pathFinder->SetSpeed(0);
	}
	
	m_animator->SetHorizontal(hstate);
	m_animator->SetGroundState(m_body->HasGround());

	if (m_animator->IsPlayingGroggy() || m_animator->IsPlayingHurt())
	{
		if (m_body->HasGround() && fabsf(m_body->GetVeloicty().x) > 20.0f && m_dustCreationCounter < 0)
		{
			float angle = (m_body->GetVeloicty().x > 0 ? -180.0f : 0.0f);
			float scale = float(rand() % 100 + 1) * 0.01f + 0.5f;
			float adjust_distance = float(rand() % 5);
			StaticEffect::Create(
				L"../Animation/effect/dustcloud.txt",
				m_body->GetBottom() + Vec2::Direction(angle) * adjust_distance + Vec2::up() * 5,
				0.0f,
				Vec2::one() * scale,
				Vec2::Direction(angle * DEG2RAD) * 50,
				Vec2::zero(),
				true
			);
			m_dustCreationCounter = 0.01f;
		}
		else m_dustCreationCounter -= Time::GetDeltaTime();
	}

	if (m_animator->IsPlayingHurt() && !(m_animator->IsPlayingHurtGround() && m_animator->GetPercent() > 0.5f))
	{
		if (m_bloodCreationCounter < 0)
		{
			Vec2 position = GetTransform()->GetPosition() + Vec2::up() * float(rand() % 20 - 10);
			position -= m_body->GetVeloicty().normalized() * 10;
			int bloodCase = rand() % 3;
			wstring key;
			switch (bloodCase)
			{
				case 0: key = L"../Animation/effect/blood_1.txt"; break;
				case 1: key = L"../Animation/effect/blood_2.txt"; break;
				case 2: key = L"../Animation/effect/blood_3.txt"; break;
			}
			float angle = Vec2::Angle180(m_body->GetVeloicty()) + float(rand() % 45) - 22.5f;
			float scale = float(rand() % 100) * 0.01f + 0.5f;
			StaticEffect::Create(
				key,
				position,
				angle,
				Vec2(-scale, scale),
				Vec2::zero(),
				Vec2::zero(),
				false,
				true
			);
			m_bloodCreationCounter = 0.025f;
		}
		else m_bloodCreationCounter -= Time::GetDeltaTime();
	}
}

void Enemy::BeforeRender()
{
	if (TimeController::GetInstance()->GetCurrentScale() < 0.8f)
	{
		float alpha = m_mainRenderer->GetColor().a;
		m_mainRenderer->SetColor(Color(1, 0, 1, alpha));

		if (m_slowTimeTrailCounter <= 0)
		{
			auto trail = TrailRenderer::Create(m_mainRenderer);
			trail->SetBeginColor(Color(1, 0, 1, 0.5f));
			trail->SetEndColor(Color(1, 0, 1, 0));
			trail->SetDuration(0.1f);
			trail->SetTimeScaleMode(true);
			m_slowTimeTrailCounter = 0.02f;
		}
		else
		{
			m_slowTimeTrailCounter -= Time::GetDeltaTime();
		}
	}
	else
	{
		float alpha = m_mainRenderer->GetColor().a;
		m_mainRenderer->SetColor(Color(1, 1, 1, alpha));
	}
}

void Enemy::OnDestroy()
{
	auto it = std::find(g_enemies.begin(), g_enemies.end(), this);
	if (it == g_enemies.end()) return;
	g_enemies.erase(it);
}

void Enemy::OnBeginToTurn(float hd)
{
	if (m_animator->IsPlayingHurt() || m_animator->IsPlayingGroggy()) return;
	m_animator->PlayTurn();
}

void Enemy::OnBeginToEnterDoor()
{
	if (m_animator->IsPlayingHurt() || m_animator->IsPlayingGroggy()) return;
	m_pathFinder->SetHorizontalDirection(-m_pathFinder->GetHorizontalDirection());
	m_animator->PlayTurn();
}

void Enemy::OnBeginToExitDoor()
{
	if (m_animator->IsPlayingHurt() || m_animator->IsPlayingGroggy()) return;
	m_pathFinder->SetHorizontalDirection(m_pathFinder->GetHorizontalDirection());
	m_animator->OverlapPlayTurn();
}

void Enemy::OnTriggerEnter(Collider* collider)
{
}

void Enemy::OnCollisionEnter(const CollisionResult& collision)
{
	auto fSign = [](float f)
	{
		return f < 0 ? -1 : +1;
	};

	if (!IsHurt() && !IsGroggy() && collision.A->GetLayer() == LAYER_DOOR)
	{
		Door* door = collision.A->GetBody()->GetComponent<Door>();
		if (!door) return;

		float hRel = door->GetTransform()->GetPosition().x - GetTransform()->GetPosition().x;
		if (fSign(hRel) != fSign(m_prevVel.x)) return;
		door->Open(m_prevVel.x);
	}
}

void Enemy::SetHorizontalDirection(float hd)
{
	m_pathFinder->SetHorizontalDirection(hd);
}

bool Enemy::Damage(Vec2 velocity, Component* damageObject, bool canBlock)
{
	Vec2 dirVec = velocity.normalized();
	bool doDamage = true;
	if (canBlock) doDamage = OnDamaged(dirVec);

	m_pathFinder->CancelEnterDoor();

	if (!doDamage)
	{
		if (m_hardMelee && m_animator->IsPlayingMelee()) return false;
		if (dirVec.x < 0) dirVec.x = -1;
		else dirVec.x = +1;
		dirVec.y = 0;
		m_body->SetIgnoreRestitute(false);
		m_body->SetFriction(10);
		m_body->SetHorizontalVelocity(dirVec.x * velocity.magnitude());
		m_animator->PlayGroggy();
	}
	else
	{
		if (Vec2::Angle(Vec2::up(), dirVec) <= 80)
		{
			m_body->SetJump(velocity);
			m_animator->SetGroundState(false);
			m_animator->PlayHurtFly();
		}
		else
		{
			if (dirVec.x < 0) dirVec.x = -1;
			else dirVec.x = +1;
			dirVec.y = 0;

			m_body->SetIgnoreRestitute(false);
			m_body->SetFriction(10);
			//m_body->SetHorizontalVelocity(dirVec.x * velocity.magnitude() * (canBlock ? 2 : 1));
			m_body->SetVelocity(dirVec * velocity.magnitude() * (canBlock ? 2.0f : 1.0f));

			m_pathFinder->SetHorizontalDirection(-dirVec.x);
			m_pathFinder->ClearPath();
			m_pathFinder->Pause();

			m_animator->PlayHurtGround();
		}

		float angle = Vec2::Angle(Vec2::right(), velocity) + 180;
		if (velocity.y < 0) angle *= -1;
		auto hit = StaticEffect::Create(L"../Animation/effect/hit_impact.txt", GetTransform()->GetPosition(), angle, Vec2::one());
		hit->SetColor(Color(1, 1, 1, 0.5f));
		hit->GetTransform()->SetParent(GetTransform());

		for (int i = 0; i < 10 + rand() % 10; ++i)
		{
			Vec2 position = GetTransform()->GetPosition() + Vec2::up() * float(rand() % 20 - 10);
			position -= m_body->GetVeloicty().normalized() * 10;
			int bloodCase = rand() % 3;
			wstring key;
			switch (bloodCase)
			{
				case 0: key = L"../Animation/effect/blood_1.txt"; break;
				case 1: key = L"../Animation/effect/blood_2.txt"; break;
				case 2: key = L"../Animation/effect/blood_3.txt"; break;
			}
			float angle = Vec2::Angle180(-m_body->GetVeloicty()) + float(rand() % 90) - 45.0f;
			float scale = float(rand() % 100) * 0.01f + 0.5f;
			auto blood = StaticEffect::Create(
				key,
				position,
				angle,
				Vec2(-scale, scale),
				Vec2::Direction(angle * DEG2RAD) * 300,
				Vec2::down() * 500,
				false,
				true
			);
			blood->SetSpeed(float(rand() % 100) * 0.01f + 0.5f);
		}
	}
	return doDamage;
}

void Enemy::ChangeState(State state)
{
	if (m_state == state) return;

	State prevState = m_state;
	switch (prevState)
	{
		case State::TRACE:
			{
				m_pathFinder->SetAutoTarget(nullptr);
			}
			break;
	}

	switch (state)
	{
		case State::SEARCH:
			{
				if (HasStandingPoint())
				{
					m_pathFinder->SetTarget(m_rallyPoint[0]);
				}
				else if (HasRallyPoints())
				{
					m_rallyPointIndex = 0;
					m_pathFinder->SetTarget(m_rallyPoint[m_rallyPointIndex]);
				}
			}
			break;
		case State::TRACE:
			{
				m_restCounter = 0.1f;
				m_pathFinder->SetAutoTarget(PlayerController::GetInstance()->GetPlatformerBody());
			}
			break;
		case State::LOST_TARGET:
			break;
	}
	m_state = state;
}

Enemy::State Enemy::GetState() const
{
	return m_state;
}

void Enemy::SetRallyPoints(Vec2 p1, Vec2 p2)
{
	m_rallyPointCount = 2;

	p1.y += 0.1f;
	p2.y += 0.1f;

	RaycastResult res;
	if (Physics::Raycast(p1, p1 + Vec2::down() * 1000, &res, LAYER_GROUND | LAYER_PLATFORM)) m_rallyPoint[0] = res.point;
	else m_rallyPoint[0] = p1;
	if (Physics::Raycast(p2, p2 + Vec2::down() * 1000, &res, LAYER_GROUND | LAYER_PLATFORM)) m_rallyPoint[1] = res.point;
	else m_rallyPoint[1] = p2;

	m_rallyPoint[0].y += 0.1f;
	m_rallyPoint[1].y += 0.1f;
}

void Enemy::SetStandingPoint(Vec2 p)
{
	m_rallyPointCount = 1;
	
	p.y += 0.1f;
	
	RaycastResult res;
	if (Physics::Raycast(p, p + Vec2::down() * 1000, &res, LAYER_GROUND | LAYER_PLATFORM)) m_rallyPoint[0] = res.point;
	else m_rallyPoint[0] = p;
}

bool Enemy::HasRallyPoints() const
{
	return m_rallyPointCount == 2;
}

bool Enemy::HasStandingPoint() const
{
	return m_rallyPointCount == 1;
}

float Enemy::GetHorizontalDirection() const
{
	if (m_mainRenderer->GetTransform()->GetScale().x < 0) return -1;
	if (m_mainRenderer->GetTransform()->GetScale().x > 0) return +1;
	return 0;
}

bool Enemy::IsHurt() const
{
	return m_animator->IsPlayingHurt();
}

bool Enemy::IsGroggy() const
{
	return m_animator->IsPlayingGroggy();
}

int Enemy::GetEnemyID() const
{
	return m_enemyID;
}

SpriteRenderer* Enemy::GetRenderer() const
{
	return m_mainRenderer;
}

void Enemy::Search()
{	
	if (!HasRallyPoints()) return;
	if (Vec2::Distance(m_rallyPoint[m_rallyPointIndex], m_body->GetBottom()) > m_pathFinder->GetMinDistance()) return;

	if (m_rallyPointIndex == 0) m_rallyPointIndex = 1;
	else if (m_rallyPointIndex == 1) m_rallyPointIndex = 0;
	m_pathFinder->SetTarget(m_rallyPoint[m_rallyPointIndex]);
	m_restCounter = 1.0f + float(rand() % 100) * 0.01f;
}

void Enemy::TracePlayer()
{
	if (m_updateTracePathCounter > 0) 
	{
		m_updateTracePathCounter -= Time::GetDeltaTime();
		return;
	}
	m_updateTracePathCounter = 0.1f;

	m_pathFinder->SetAutoTarget(PlayerController::GetInstance()->GetPlatformerBody());
}

void Enemy::Melee()
{
	if (m_animator->IsPlayingTurn()) return;
	if (m_animator->IsPlayingHurt()) return;
	if (m_animator->IsPlayingMelee()) return;
	if (m_animator->IsPlayingGroggy()) return;

	Vec2 current = GetTransform()->GetPosition();
	Vec2 player = PlayerController::GetInstance()->GetTransform()->GetPosition();
	Vec2 relVec = player - current;
	float relMag = relVec.magnitude();

	if (relMag <= m_meleeRange) m_pathFinder->SetSpeed(0);

	float hd = +1;
	if (relVec.x < 0) hd = -1;
	if (relVec.x > 0) hd = +1;

	if (hd != GetHorizontalDirection()) return;
	if (relVec.magnitude() > m_meleeRange) return;

	m_animator->PlayMelee();
	m_pathFinder->ClearPath();
	m_restCounter = 0.2f;

	Vec2 pusherPos = GetTransform()->GetPosition() + Vec2(GetHorizontalDirection(), 0) * m_meleeRange * 0.5f;
	Vec2 pusherScale = Vec2((m_meleeRange + 40) * GetHorizontalDirection(), m_body->GetFullHeight());
	GameObject* pusherObject = new GameObject(pusherPos, 0, pusherScale);
	PlayerPusher* pusher = pusherObject->AddComponent<PlayerPusher>();
	pusher->StickToGameObject(this);
	pusher->SetDamage(m_meleeKill);
	pusher->SetWait(m_meleeWait);

	CSoundMgr::Get_Instance()->PlaySound(L"sound_enemy_punch.wav", CSoundMgr::CHANNELID::ENEMY_MELEE);
}

void Enemy::LongRangeAimming()
{
	bool prevAimming = m_animator->IsAimming();
	m_animator->SetAimming(false);

	if (!m_useLongRangeAttack) return;
	if (m_animator->IsPlayingTurn()) return;
	if (m_animator->IsPlayingHurt()) return;
	if (m_animator->IsPlayingMelee()) return;
	if (m_animator->IsPlayingGroggy()) return;
	if (m_animator->IsPlayingMelee()) return;

	Vec2 current = GetTransform()->GetPosition();
	Vec2 player = PlayerController::GetInstance()->GetTransform()->GetPosition();
	Vec2 relVec = player - current;
	float relMag = relVec.magnitude();

	float hd = +1;
	if (relVec.x < 0) hd = -1;
	if (relVec.x > 0) hd = +1;

	if (hd != GetHorizontalDirection()) return;
	if (relVec.magnitude() > m_longRangeAttackRange + (prevAimming == true ? 100 : 0))
	{
		return;
	}

	RaycastResult res;
	if (Physics::Raycast(GetTransform()->GetPosition(), PlayerController::GetInstance()->GetTransform()->GetPosition(), &res, LAYER_GROUND | LAYER_DOOR | LAYER_BREAKABLE_PLATFORM))
	{
		return;
	}

	float adjust = 0;
	//if (relMag <= m_longRangeAttackRange + adjust) m_pathFinder->Pause();

	m_animator->SetAimming(true);

	float cosAngle = Vec2::Angle(relVec, Vec2::right());
	float angle = cosAngle;
	if (relVec.x < 0) angle = -(angle - 180);
	if (relVec.y < 0) angle *= -1;
	angle = float(int(angle));
	m_animator->SetAimAngle(angle);

	if (prevAimming == false)
	{
		m_restCounter = 0.5f;
		if (m_shootCount <= m_firstShootDelay) m_shootCount = m_firstShootDelay;
		else m_shootCount = m_shootDelay;
	}
	else
	{
		if (m_shootCount <= 0)
		{
			if (m_bulletCount == 1)
			{
				Shoot(1200, 0);

				int soundCase = rand() % 2;
				switch (soundCase)
				{
					case 0:
						CSoundMgr::Get_Instance()->PlaySound(L"sound_gun_fire_1.wav", CSoundMgr::CHANNELID::BULLET_FIRE);
						break;
					case 1:
						CSoundMgr::Get_Instance()->PlaySound(L"sound_gun_fire_2.wav", CSoundMgr::CHANNELID::BULLET_FIRE);
						break;
				}
			}
			else
			{
				for (int i = 0; i < m_bulletCount; ++i)
				{
					float speedAdjust = float(rand() % 100);
					float angleAdjust = float(rand() % 20 - 10);
					Shoot(1200 - speedAdjust, angleAdjust);
				}
				CSoundMgr::Get_Instance()->PlaySound(L"sound_shotgun_fire.wav", CSoundMgr::CHANNELID::BULLET_FIRE);
			}
			m_shootCount = m_shootDelay;
		}
		m_shootCount -= Time::GetDeltaTime();
	}
}

void Enemy::Shoot(float speed, float adjustAngle)
{
	Vec2 current = GetTransform()->GetPosition();
	Vec2 player = PlayerController::GetInstance()->GetTransform()->GetPosition();
	Vec2 relVec = player - current;
	Vec2 dirVec = relVec.normalized();
	if (adjustAngle != 0)
	{
		Quat q = Quat::AxisAngle(Vec3::forawrd(), adjustAngle * DEG2RAD);
		dirVec = q * dirVec;
	}	

	Bullet::Create(current + Vec2::up() * 10 + dirVec * 30, dirVec, speed, true);
}

bool Enemy::DetectPlayer()
{
	Vec2 current = GetTransform()->GetPosition();
	Vec2 player = PlayerController::GetInstance()->GetTransform()->GetPosition();
	Vec2 relVec = player - current;

	// 플레이어를 볼 수 있는 방향이 아니라면 종료합니다.
	float hd = +1;
	if (relVec.x < 0) hd = -1;
	if (relVec.x > 0) hd = +1;
	if (hd != GetHorizontalDirection()) return false;

	// 탐지 가능한 범위보다 멀리 있다면 종료합니다.
	if (relVec.magnitude() > m_detectionRange) return false;

	// 적과 플레이어 사이에 콜라이더가 존재하면
	// 플레이어를 감지하지 못한 상태입니다.
	RaycastResult res;
	if (Physics::Raycast(current, player, &res, LAYER_GROUND | LAYER_PLATFORM | LAYER_DOOR)) 
	{
		return false;
	}

	// 콜라이더가 존재하지 않는다면
	// 플레이어를 감지한 상태입니다.
	return true;
}
