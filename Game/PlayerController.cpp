#include "stdafx.h"
#include "PlayerController.h"
#include "PlayerAnimator.h"
#include "PlatformerBody.h" 
#include "TrailRenderer.h"
#include "GameLayer.h"
#include "SwordEffect.h"
#include "GameRenderOrder.h"
#include "Door.h"
#include "GameTag.h"
#include "TimeController.h"
#include "StaticEffect.h"
#include "Recorder.h"
#include "UI.h"
#include "ThrowObject.h"
#include "SoundMgr.h"

PlayerController* PlayerController::g_instance = nullptr;

void PlayerController::Awake()
{
	if (!g_instance) 
	{
		g_instance = this;
	}

	//godMode = true;

	GameObject* rendererObject = GameObject::CreateChild(GetGameObject());
	m_renderer = rendererObject->AddComponent<SpriteRenderer>();
	m_renderer->SetRenderOrder(RENDER_ORDER_PLAYER);
#ifdef SpriteRenderer
	m_renderer->SetRecordRGBMode(false);
#endif

	m_animator = rendererObject->AddComponent<PlayerAnimator>();
	m_animator->OnEndPlayHurtflyGround += Function<void>(this, &PlayerController::OnEndPlayHurtflyGround);

	m_body = AddComponent<PlatformerBody>();
	m_body->GetBody()->GetGameObject()->SetTag(TAG_PLAYER);
	m_body->OnGround += Function<void, const Vec2&, const Vec2&>(this, &PlayerController::OnGround);

	m_accelDir = Vec2::zero();
	m_jumpGravityScale = 0.6f;
	m_wallGrabGragivtScale = 0.2f;

	m_trailCounter = 0;

	m_maxSlowTime = 5.0f;
	m_slowCounter = m_maxSlowTime;
	m_slowRecoveryTime = 0.5f;
	m_slowRecovery = true;
	m_slowRecoveryCounter = 0.0f;
	m_useSlowControll = true;
	m_slowModeColorChange = true;

	m_maxLimitTime = 121.0f;
	m_limitCounter = m_maxLimitTime;
}

void PlayerController::FixedUpdate()
{
	if (TimeController::GetInstance()->IsStop()) return;

	if (!m_animator->IsPlayingAttack() && !m_animator->IsPlayingHurt())
	{
		m_accelDir.Normalize();
		if (m_accelDir.sqrMagnitude() > 0 && m_knockbackCounter <= 0)
		{
			if (m_body->HasGround())
			{
				float crouchingFactor = m_crouching ? 0.3f : 1.0f;
				m_body->SetHorizontalVelocity(m_accelDir.x * 220 * crouchingFactor);
			}
			else
			{
				m_body->AccelerateTo(m_accelDir, 250, 20);
			}
		}
		m_accelDir = Vec2::zero();
		float gs = 1.0f;
		if (m_wallGrabbing)
		{
			gs = m_wallGrabGragivtScale;
		}
		else if (m_pressingJump && !m_body->HasGround() && m_body->IsJumping() && m_body->GetBody()->GetVelocity().y > 0)
		{
			gs = m_jumpGravityScale;
		}
		m_body->SetDefaultGravityScale(gs);
		
		m_pressingJump = false;

		if (m_hasJump && m_body->HasGround())
		{
			m_body->SetJump(250);
			OnJump(m_body->GetRaycastResult().normal, m_body->GetRaycastResult().point);
		}
		m_hasJump = false;
	}

	if (m_reservedFlipVel.sqrMagnitude() > 0)
	{
		m_body->SetJump(m_reservedFlipVel);
	}
	m_reservedFlipVel = Vec2::zero();

	if (m_reservedVel.sqrMagnitude() > 0)
	{
		m_body->SetVelocity(m_reservedVel);
	}
	m_reservedVel = Vec2::zero();

	if (m_animator->IsPlayingAttack())
	{
		m_body->SetUnrealisticPhysics(false);
		m_body->SetFriction(1.0f);
	}
	else if (m_animator->IsPlayingHurt() || m_knockbackCounter > 0)
	{
		m_knockbackCounter -= Time::GetFixedDeltaTime();
		m_body->SetUnrealisticPhysics(false);
		m_body->SetFriction(10.0f);
	}
	else
	{
		m_body->SetUnrealisticPhysics(true);
		m_body->SetFriction(10.0f);
	}

	if (m_animator->IsPlayingRoll())
	{
		m_body->SetHorizontalVelocity(m_rollDirection.x * 400);
		m_body->SetFriction(100.0f);
	}


	auto fSign = [](float f)
	{
		return f < 0 ? -1.0f : +1.0f;
	};
	Vec2 curVel = m_body->GetVeloicty();
	if (m_body->HasRayHit() && m_animator->GetHorizontaControlState() && !m_animator->IsCrouch())
	{
		bool differentDir = fSign(curVel.x) != fSign(m_prevVel.x);
		bool xVelChange = fabsf(m_prevVel.x) < 150.0f && fabsf(curVel.x) >= 50.0f;
		if (differentDir || xVelChange)
		{
			auto stomp = StaticEffect::Create(L"../Animation/effect/stompcloud.txt", m_body->GetBottom(), 0.0f, Vec2(-fSign(curVel.x), 1));
			stomp->SetAlphaDestroy(true);

			constexpr int dustCount = 5;
			for (int i = 0; i < dustCount; ++i)
			{
				float velAngle = float(rand() % 45);
				float scale = float(rand() % 100 + 1) * 0.01f + 0.2f;
				float adjust_distance = float(rand() % 5);
				auto dust = StaticEffect::Create(
					L"../Animation/effect/dustcloud.txt",
					m_body->GetBottom() + Vec2::Direction(velAngle * DEG2RAD) * adjust_distance + Vec2::up() * 5,
					velAngle,
					Vec2::one() * scale);
				Quat q = Quat::AxisAngle(Vec3(0,0,1), velAngle * DEG2RAD);
				Vec2 vel = q * Vec2::right() * (20 + float(rand() % 100));
				vel.x *= -GetDirection();
				dust->SetVelocity(vel);
				dust->SetColor(Color(1, 1, 1, 0.5f));
			}
		}
	}
	m_prevVel = m_body->GetVeloicty();
}

void PlayerController::Update()
{
	if (Input::GetKeyDown(Key::R))
	{
		Restart();
	}

	if (TimeController::GetInstance()->IsStop()) return;

	HorizontalMove();
	Jump();
	LongJump();
	Attack();
	RollAndThroughPlatform();
	Crouch();
	Flip();
	WallGrab();
	DoorOpen();

	if (!m_wasPutInThrowObjectPrevFrame && Input::GetKeyDown(Key::RightMouse))
	{
		ThrowHasObject();
	}

	if (m_wallGrabbing) CSoundMgr::Get_Instance()->PlaySoundContiue(L"sound_player_wallslide.wav", CSoundMgr::CHANNELID::PLAYER_WALLSLIDE);
	else CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::PLAYER_WALLSLIDE);
}

void PlayerController::LateUpdate()
{
	m_wasPutInThrowObjectPrevFrame = false;

	MakeTrailEffect();

	Slow();
	LimitTime();

	m_animator->SetHorizontalControl(m_accelDir.x != 0 && m_knockbackCounter <= 0);

	if (m_body->HasRayHit())
	{
		if (m_body->GetRaycastResult().collider->GetLayer() == LAYER_PLATFORM && m_body->GetVeloicty().y > 1.0f)
		{
			m_animator->SetGroundState(false);
		}
		else
		{
			m_animator->SetGroundState(true);
		}
	}
	else
	{
		m_animator->SetGroundState(false);
	}

	if (m_animator->IsPlayingRoll() || m_wallGrabbing)
	{
		if (m_dustCreationCounter < 0)
		{
			float scale = float(rand() % 100 + 1) * 0.01f + 0.5f;
			if (m_body->HasCollision() && m_animator->IsPlayingRoll())
			{
				float adjust_distance = float(rand() % 10);
				auto effect = StaticEffect::Create(
					L"../Animation/effect/dustcloud.txt",
					m_body->GetBottom() + Vec2(-GetDirection(), 0) * adjust_distance + Vec2::up() * 5,
					(GetDirection() < 0 ? 0.0f : 180.0f),
					Vec2::one() * scale);
				effect->SetVelocity(Vec2(-GetDirection(), 0.25f) * 100);
			}
			else if (m_wallGrabbing)
			{
				StaticEffect::Create(
					L"../Animation/effect/dustcloud.txt",
					m_grabWallPoint,
					float(rand() % 360),
					Vec2::one() * scale);
			}
			m_dustCreationCounter = 0.01f;
		}
		else m_dustCreationCounter -= Time::GetDeltaTime();
	}
}

void PlayerController::BeforeRender()
{
	if (!m_slowModeColorChange) return;

	if (TimeController::GetInstance()->GetCurrentScale() < 0.8f)
	{
		float alpha = m_renderer->GetColor().a;
		m_renderer->SetColor(Color(0, 1, 1, alpha));

		if (m_slowTimeTrailCounter <= 0)
		{
			auto trail = TrailRenderer::Create(m_renderer);
			trail->SetBeginColor(Color(0, 1, 1, 0.9f));
			trail->SetEndColor(Color(0, 1, 1, 0));
			trail->SetDuration(0.1f);
			trail->SetTimeScaleMode(true);
			trail->SetAdditiveBlending(true);
			m_slowTimeTrailCounter = 0.02f;
		}
		else
		{
			m_slowTimeTrailCounter -= Time::GetDeltaTime();
		}
	}
	else
	{
		float alpha = m_renderer->GetColor().a;
		m_renderer->SetColor(Color(1, 1, 1, alpha));
	}
}

void PlayerController::OnDestroy()
{
	m_body->OnGround -= Function<void, const Vec2&, const Vec2&>(this, &PlayerController::OnGround);

	if (g_instance == this) 
	{
		g_instance = nullptr;
	}
}

void PlayerController::OnGround(const Vec2& normal, const Vec2& point)
{
	StaticEffect::Create(L"../Animation/effect/landcloud.txt", point, 0, Vec2::one() * 1.35f);

	CSoundMgr::Get_Instance()->PlaySound(L"sound_player_land.wav", CSoundMgr::CHANNELID::PLAYER);
}

void PlayerController::OnJump(const Vec2& normal, const Vec2& point)
{
	StaticEffect::Create(L"../Animation/effect/jumpcloud.txt", point);
	StaticEffect::Create(L"../Animation/effect/landcloud.txt", point, 0, Vec2::one() * 1.35f);

	CSoundMgr::Get_Instance()->PlaySound(L"sound_player_jump.wav", CSoundMgr::CHANNELID::PLAYER);
}

void PlayerController::OnFlip(const Vec2& normal, const Vec2& point)
{
	Vec2 p = point;
	p.y = GetTransform()->GetPosition().y;
	Quat q = Quat::FromToRotation(Vec2::up(), normal);
	auto effect = StaticEffect::Create(L"../Animation/effect/jumpcloud.txt", point, q.GetEulerAngle().z, Vec2::one());
	effect->SetSpeed(0.5f);

	CSoundMgr::Get_Instance()->PlaySound(L"sound_player_roll.wav", CSoundMgr::CHANNELID::PLAYER);
}

void PlayerController::OnEndPlayHurtflyGround()
{
	Restart();
}

PlayerController * PlayerController::GetInstance()
{
	return g_instance;
}

PlatformerBody * PlayerController::GetPlatformerBody() const
{
	return m_body;
}

void PlayerController::SetDirection(float hd)
{
	if (hd < 0) hd = -1;
	if (hd > 0) hd = 1;
	Vec2 scale = m_renderer->GetTransform()->GetScale();
	scale.x = hd;
	m_renderer->GetTransform()->SetScale(scale);
}

float PlayerController::GetDirection() const
{
	return m_renderer->GetTransform()->GetScale().x;
}

void PlayerController::Knockback(Vec2 velocity, Component* damageObject)
{
	m_knockbackCounter = 0.5f;
	m_body->SetUnrealisticPhysics(false);
	m_body->CancelJumpState();
	m_reservedVel = velocity;
}

void PlayerController::HardKnockback(Vec2 velocity, Component* damageObject)
{
	Hurt(velocity, damageObject, true);
}

void PlayerController::Damage(Vec2 velocity, Component* damageObject)
{
	Hurt(velocity, damageObject, (godMode ? true : false));
}

bool PlayerController::IsRolling() const
{
	return m_animator->IsPlayingRoll();
}

bool PlayerController::IsHurt() const
{
	return m_animator->IsPlayingHurt();
}

bool PlayerController::CanPutInThrowObject() const
{
	return !m_wasPutInThrowObjectPrevFrame;
}

void PlayerController::PutInThrowObject(ThrowObject* object)
{
	ThrowHasObject();

	if (!object) return;

	m_throwObject = object;
	m_wasPutInThrowObjectPrevFrame = true;
	UI::GetInstance()->SetThrowIconKey(object->GetSpriteKey());

	CSoundMgr::Get_Instance()->PlaySound(L"sound_clatter_brick1.wav", CSoundMgr::CHANNELID::PLAYER_UI);
}

void PlayerController::ThrowHasObject()
{
	if (!m_throwObject) return;

	Vec2 throwDir = (CCamera2D::GetInstance()->GetMousePositionInWorld() - GetTransform()->GetPosition()).normalized();
	m_throwObject->Throw(GetTransform()->GetPosition(), throwDir * 1000);

	m_throwObject = nullptr;
	UI::GetInstance()->SetThrowIconToEmpty();

	CSoundMgr::Get_Instance()->PlaySound(L"sound_player_throw.wav", CSoundMgr::CHANNELID::PLAYER_EFFECT);
}

SpriteRenderer* PlayerController::GetRenderer() const
{
	return m_renderer;
}

void PlayerController::SetSlowControll(bool enable)
{
	m_useSlowControll = enable;
}

void PlayerController::SetSlowModeColorChange(bool enable)
{
	m_slowModeColorChange = enable;
}

void PlayerController::MakeTrailEffect()
{
	if (TimeController::GetInstance()->GetCurrentScale() < 0.8f) return;

	m_trailCounter -= Time::GetDeltaTime();
	if (m_trailCounter <= 0)
	{
		TrailRenderer* trail = TrailRenderer::Create(m_renderer);
		if (m_animator->IsPlayingRoll() || m_animator->IsPlayingFlip() || m_animator->IsPlayingAttack())
		{
			trail->SetDuration(0.2f);
			trail->SetBeginColor(Color(0, 1, 1, 1.5f));
			trail->SetEndColor(Color(1, 0, 1, 0));
		}
		else
		{
			trail->SetDuration(0.1f);
			trail->SetBeginColor(Color(0, 1, 1, 0.3f));
			trail->SetEndColor(Color(1, 0, 1, 0));
		}

		m_trailCounter = 0.01f;
		trail->SetAdditiveBlending(true);
	}
}

void PlayerController::HorizontalMove()
{
	if (!m_animator->CanMove()) return;
	if (m_knockbackCounter > 0) return;
	if (Input::GetKey(Key::A))
	{
		m_accelDir.x -= 1;
		m_renderer->GetTransform()->SetScale(Vec2(-1, 1));
		if (m_animator->IsPlayingToIdle() && !Input::GetKey(Key::S)) m_animator->PlayDefaultAnimation();
	}
	if (Input::GetKey(Key::D))
	{
		m_accelDir.x += 1;
		m_renderer->GetTransform()->SetScale(Vec2(+1, 1));
		if (m_animator->IsPlayingToIdle() && !Input::GetKey(Key::S)) m_animator->PlayDefaultAnimation();
	}
}

void PlayerController::Jump()
{
	if (!m_animator->CanMove()) return;
	if (m_knockbackCounter > 0) return;
	if (m_hasJump) return;
	if (Input::GetKeyDown(Key::W))
	{
		m_hasJump = true;
	}
	m_animator->SetVerticalVelocity(m_body->GetBody()->GetVelocity().y);
}

void PlayerController::LongJump()
{
	if (!m_animator->CanMove()) return;
	m_pressingJump = Input::GetKey(Key::W);
}

void PlayerController::Attack()
{
	if (!m_animator->CanAttack()) return;
	if (Input::GetKeyDown(Key::LeftMouse))
	{
		m_animator->PlayAttack();
		Vec2 attackDirection = (CCamera2D::GetInstance()->GetMousePositionInWorld() - GetTransform()->GetPosition()).normalized();
		m_renderer->GetTransform()->SetScale(Vec2(attackDirection.x < 0 ? -1 : +1.0f, 1.0f));
		m_body->SetUnrealisticPhysics(false);
		m_body->CancelJumpState();
		m_reservedVel = attackDirection * 400;

		GameObject* swordEffectObject = GameObject::CreateChild(GetGameObject());
		Transform* t = swordEffectObject->GetTransform();
		t->SetRight(attackDirection);
		SwordEffect* swordEffect = swordEffectObject->AddComponent<SwordEffect>();
		swordEffect->StickToGameObject(GetGameObject());

		float attackAngle = Vec2::Angle360(attackDirection);
		if (attackAngle > 240 && attackAngle < 300 && m_body->GetRaycastResult().collider->GetLayer() == LAYER_PLATFORM)
		{
			m_body->ThroughPlatformOnce();
			swordEffect->m_longTake = true;
			//Vec2 playerPos = GetTransform()->GetPosition();
			//Vec2 triggerPos = swordEffect->GetTrigger()->GetTransform()->GetPosition();
			//float triggerWidth = swordEffect->GetTrigger()->GetWidth();
			//RaycastResult res;
			//if (Physics::Raycast(playerPos, triggerPos + attackDirection * triggerWidth * 0.5f, &res, LAYER_PLATFORM))
			//{

			//}
		}

		float dot = Vec2::Dot(attackDirection, Vec2::left());
		if (dot < 0) t->SetScale(Vec2(1, -1));
	}
}

void PlayerController::RollAndThroughPlatform()
{
	if (!m_animator->CanMove()) return;
	if (!m_body->HasGround()) return;
	if (!Input::GetKeyDown(Key::S)) return;
	if (Input::GetKey(Key::A) || Input::GetKey(Key::D))
	{
		m_rollDirection = Input::GetKey(Key::A) ? Vec2::left() : Vec2::right();
		m_renderer->GetTransform()->SetScale(Vec2(m_rollDirection.x < 0 ? -1 : +1.0f, 1.0f));
		m_animator->PlayRoll();

		CSoundMgr::Get_Instance()->PlaySound(L"sound_player_roll.wav", CSoundMgr::CHANNELID::PLAYER);
	}
	else
	{
		m_body->ThroughPlatformOnce();
	}
}

void PlayerController::Crouch()
{
	m_crouching = Input::GetKey(Key::LCtrl);
	m_animator->SetCrouch(m_crouching);
}

void PlayerController::WallGrab()
{
	constexpr float wallGrabRayLength = 1;
	if (!m_wallGrabbing /*&& !m_animator->IsPlayingFlip()*/)
	{
		if (m_body->HasRayHit()) return;
		if (!Input::GetKey(Key::W)) return;

		RaycastResult result;
		bool hit = RaycastToWall(m_renderer->GetTransform()->GetScale().x < 0 ? true : false, wallGrabRayLength, & result);
		if (hit)
		{
			if (m_animator->IsPlayingFlip()) m_animator->PlayDefaultAnimation();
			m_wallGrabbing = true;
			Vec2 velocity = m_body->GetVeloicty();
			if (velocity.y < 0) velocity.y = 0;
			m_body->SetVelocity(velocity);

			m_grabWallNormal = result.normal;
			m_grabWallPoint = result.point;
		}
	}
	else
	{
		if (m_body->HasRayHit())
		{
			m_wallGrabbing = false;
		}
		else
		{
			RaycastResult result;
			bool hit = RaycastToWall(m_renderer->GetTransform()->GetScale().x < 0 ? true : false, wallGrabRayLength, & result);
			if (!hit) m_wallGrabbing = false;

			m_grabWallNormal = result.normal;
			m_grabWallPoint = result.point;
		}
	}
	m_animator->SetWallGrab(m_wallGrabbing);
}

void PlayerController::Flip()
{
	//if (!m_wallGrabbing) return;
	//if (!Input::GetKeyDown(Key::W)) return;

	//m_wallGrabbing = false;
	//m_animator->PlayFlip();

	//float xDirection = m_renderer->GetTransform()->GetScale().x;
	//Vec2 direction = Vec2(-xDirection, 1);
	//m_renderer->GetTransform()->SetScale(direction);
	//direction.Normalize();
	//m_body->SetVelocity(Vec2(0, m_body->GetVeloicty().y));
	//m_body->SetJump(direction * 400);
	//m_reservedVel = Vec2::zero();
	//OnFlip(m_grabWallNormal, m_grabWallPoint);

	if (!m_wallGrabbing) return;
	if (!Input::GetKeyDown(Key::W)) return;

	m_wallGrabbing = false;
	m_animator->PlayFlip();

	float xDirection = m_renderer->GetTransform()->GetScale().x;
	Vec2 direction = Vec2(-xDirection, 1);

	m_renderer->GetTransform()->SetScale(direction);

	direction.Normalize();
	m_body->SetVelocity(Vec2::zero());
	m_reservedFlipVel = direction * 400;
	m_reservedVel = Vec2::zero();
	OnFlip(m_grabWallNormal, m_grabWallPoint);
}

void PlayerController::Hurt(Vec2 velocity, Component* damageObject, bool canWake)
{
	if (m_isDead) return;

	m_wallGrabbing = false;
	m_body->SetDefaultGravityScale(1.0f);

	m_knockbackCounter = 0;
	m_body->SetUnrealisticPhysics(false);

	m_reservedVel = velocity;

	m_animator->SetGroundState(false);
	m_animator->PlayHurt(canWake);

	if (!canWake) m_isDead = true;

	if (m_isDead)
	{
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
}

void PlayerController::HurtCover()
{
	if (m_animator->IsPlayingHurtGroundLoop())
	{
		m_animator->PlayHurtCover();
	}
}

void PlayerController::DoorOpen()
{
	if (!m_animator->IsPlayingHorizontalMoveOnGround()) return;
	Vec2 rayDir(m_renderer->GetTransform()->GetScale().x, 0);
	Vec2 position = GetTransform()->GetPosition();
	RaycastResult result;
	if (Physics::Raycast(position, position + rayDir * (m_body->GetCircleCollider()->GetRadius() + 2.5f), &result, LAYER_DOOR))
	{
		Door* door = result.collider->GetBody()->GetComponent<Door>();
		if (door)
		{
			m_animator->PlayDoorOpen();
			door->Open(rayDir.x);
			m_accelDir = Vec2::zero();
			m_body->SetVelocity(Vec2::zero());
		}
	}
}

void PlayerController::Slow()
{
	if (!m_useSlowControll) return;

	float timeScale = 1.0f;
	if (m_slowCounter > 0 && Input::GetKeyDown(Key::LShift))
	{
		m_slow = true;
	}
	if (Input::GetKeyUp(Key::LShift))
	{
		m_slow = false;
	}
	if (m_slow)
	{
		timeScale = 0.1f;
		m_slowCounter -= Time::GetUnscaledDeltaTime();
		if (m_slowCounter <= 0)
		{
			m_slow = false;
			m_slowCounter = 0.0f;
			m_slowRecoveryCounter = m_slowRecoveryTime;
			m_slowRecovery = false;
		}
	}
	else if (!m_slowRecovery && m_slowRecoveryCounter > 0)
	{
		m_slowRecoveryCounter -= Time::GetUnscaledDeltaTime();
		if (m_slowRecoveryCounter < 0) m_slowRecovery = true;
	}
	else if (m_slowRecoveryCounter <= 0)
	{
		m_slowCounter += Time::GetUnscaledDeltaTime();
		if (m_slowCounter > m_maxSlowTime) m_slowCounter = m_maxSlowTime;
	}

	UI::GetInstance()->SetBatteryPercent(m_slowCounter / m_maxSlowTime);

	if (m_isDead) timeScale = 1.0f;

	TimeController::GetInstance()->SetScale(timeScale);
}

void PlayerController::LimitTime()
{
	if (Recorder::GetInstance()->IsPlaying()) return;

	m_limitCounter -= Time::GetUnscaledDeltaTime();

	if (m_limitCounter < 0)
	{
		Restart();
	}

	UI::GetInstance()->SetTimePercent(m_limitCounter / m_maxLimitTime);
}

void PlayerController::Restart()
{
	if (!Recorder::GetInstance()->IsPlayingReverse())
	{
		Recorder::GetInstance()->PlayReverse();
		Recorder::GetInstance()->SetCanReloadSceneWhenPlayReverse(true);
	}
}

bool PlayerController::RaycastToWall(bool left, float length, RaycastResult* pResult)
{
	Vec2 direction;
	if (left) direction = Vec2::left();
	else direction = Vec2::right();
	Vec2 moveTo = direction * (m_body->GetCircleCollider()->GetRadius() + length);

	const BoxCollider* boxCollider = m_body->GetBoxCollider();
	const CircleCollider* circleCollider = m_body->GetCircleCollider();

	Vec2 middle = boxCollider->GetTransform()->GetPosition();
	Vec2 top = middle + Vec2::up() * boxCollider->GetHeight() * 0.5f;
	Vec2 bottom = middle + Vec2::down() * boxCollider->GetHeight() * 0.5f;

	if (Physics::Raycast(middle, middle + moveTo, pResult, LAYER_GROUND))
		return true;
	if (Physics::Raycast(top, top + moveTo, pResult, LAYER_GROUND))
		return true;
	if (Physics::Raycast(bottom, bottom + moveTo, pResult, LAYER_GROUND))
		return true;
	return false;
}
