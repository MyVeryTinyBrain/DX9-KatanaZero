#include "stdafx.h"
#include "StaticEffect.h"
#include "Animation.h"
#include "AnimationManager.h"
#include "GameRenderOrder.h"

void StaticEffect::Awake()
{
	m_renderer = AddComponent<SpriteRenderer>();

	Animator::Awake();

	SetAdjust(false);

	m_wait = 0;
	m_played = false;
}

void StaticEffect::Start()
{
	Animator::Start();

	if (!m_animation)
	{
		GetGameObject()->Destroy();
		return;
	}
}

void StaticEffect::Update()
{
	if (m_wait > 0)
	{
		m_wait -= Time::GetDeltaTime();
		return;
	}

	if (!m_played)
	{
		PlayAnimation(m_animation);
		m_played = true;
	}

	Animator::Update();

	if (m_alphaDestroy)
	{
		Color color = m_renderer->GetColor();
		color.a = 1.0f - GetPercent();
		m_renderer->SetColor(color);
	}

	const Vec2& position = GetTransform()->GetPosition();
	m_velocity += m_gravity * Time::GetDeltaTime();
	GetTransform()->SetPosition(position + m_velocity * Time::GetDeltaTime());

	if (m_autoAngle)
	{
		float angle;
		if (GetTransform()->GetScale().x > 0)
		{
			angle = Vec2::Angle(Vec2::right(), m_velocity);
			if (m_velocity.y < 0) angle *= -1;
		}
		else
		{
			angle = Vec2::Angle(Vec2::left(), m_velocity);
			if (m_velocity.y > 0) angle *= -1;
		}
		GetTransform()->SetAngle(angle);
	}
}

void StaticEffect::LateUpdate()
{
	if (m_wait > 0) return;

	Animator::LateUpdate();
}

void StaticEffect::BeforeRender()
{
	if (m_wait > 0) return;

	Animator::BeforeRender();
}

void StaticEffect::OnAnimationEnd(const Animation* current)
{
	if (!m_dontDestroy)
		GetGameObject()->Destroy();
}

void StaticEffect::OnAnimationChange(const Animation* current, Animation** next)
{
}

StaticEffect* StaticEffect::Create(const wstring& animationKey, const Vec2& position, bool front)
{
	return Create(animationKey, position, 0.0f, Vec2::one(), Vec2::zero(), Vec2::zero(), false, true);
}

StaticEffect* StaticEffect::Create(const wstring& animationKey, const Vec2& position, const Vec2& direction, const Vec2& scale, const Vec2& velocity, const Vec2& gravity, bool autoAngle, bool front)
{
	float angle;
	if (scale.x > 0)
	{
		angle = Vec2::Angle(Vec2::right(), direction);
		if (direction.y < 0) angle *= -1;
	}
	else
	{
		angle = Vec2::Angle(Vec2::left(), direction);
		if (direction.y > 0) angle *= -1;
	}
	return Create(animationKey, position, angle, scale, velocity, gravity, autoAngle, front);
}

StaticEffect* StaticEffect::Create(const wstring& animationKey, const Vec2& position, float angle, const Vec2& scale, const Vec2& velocity, const Vec2& gravity, bool autoAngle, bool front)
{
	auto o = new GameObject(position, angle, scale);

	auto effect = o->AddComponent<StaticEffect>();

	effect->m_animation = AnimationManager::GetAnimation(animationKey);

	if (front) effect->m_renderer->SetRenderOrder(RENDER_ORDER_EFFECT_FRONT);
	else effect->m_renderer->SetRenderOrder(RENDER_ORDER_EFFECT_BACK);

	effect->m_velocity = velocity;
	effect->m_gravity = gravity;
	effect->m_autoAngle = autoAngle;

	return effect;
}

void StaticEffect::SetAlphaDestroy(bool enable)
{
	m_alphaDestroy = true;
}

void StaticEffect::SetColor(const Color& color)
{
	m_renderer->SetColor(color);
}

void StaticEffect::SetVelocity(const Vec2& velocity)
{
	m_velocity = velocity;
}

void StaticEffect::SetGravity(const Vec2& gravity)
{
	m_gravity = gravity;
}

void StaticEffect::SetUseAutoAngle(bool enable)
{
	m_autoAngle = enable;
}

void StaticEffect::SetRenderOrder(int order)
{
	m_renderer->SetRenderOrder(order);
}

void StaticEffect::SetWait(float wait)
{
	m_wait = wait;
}

void StaticEffect::DontDestroyOnEndOfAnimation()
{
	m_dontDestroy = true;
	SetTransition(false);
}
