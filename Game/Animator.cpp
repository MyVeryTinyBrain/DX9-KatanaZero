#include "stdafx.h"
#include "Animator.h"
#include "Animation.h"

void Animator::Awake()
{ 
	m_updateExecutionOrder = INT_MIN;
	m_lateUpdateExecutionOrder = INT_MAX;
	m_beforeRenderExecutionOrder = INT_MIN;

	AutoConnectToSpriteRenderer();
	m_elapsed = 0;
	m_current = nullptr;
	m_default = nullptr;
	m_pause = false;
	m_speed = 1.0f;
	m_transition = true;
	m_adjust = true;
}

void Animator::Start()
{
	AutoConnectToSpriteRenderer();
}

void Animator::Update()
{
	if (!m_renderer) return;
	if (!m_current) return;
	if (m_speed == 0) return;

	Sprite* sprite;
	bool endOfAnimation = !m_current->TimeOf(m_elapsed, &sprite);
	m_renderer->SetSprite(sprite);
}

void Animator::LateUpdate()
{
	if (!m_renderer) return;
	if (!m_current) return;
	if (m_speed == 0) return;

	if (!m_pause)
		m_elapsed += Time::GetDeltaTime() * m_speed;

	if (!m_current->IsLoop() && m_current->IsEnd(m_elapsed) || m_current == nullptr)
	{
		if (!m_transition) return;
		Animation* current = m_current;
		m_current = nullptr;
		OnAnimationEnd(current);
		if (!m_transition)
		{
			m_current = current;
			return;
		}

		if (!m_current) PlayAnimation(m_default, true);
		//else PlayAnimation(m_current, true);

		if (!m_current) return;

		Sprite* sprite;
		m_current->TimeOf(m_elapsed, &sprite);
		m_renderer->SetSprite(sprite);
	}
}

void Animator::BeforeRender()
{
	if (m_adjust && m_current && m_renderer)
	{
		m_renderer->GetTransform()->SetLocalPosition(m_current->GetAdjust());
	}
}

void Animator::PlayAnimation(Animation* animation, bool overlap)
{
	ResumeAnimation();
	if (!overlap && animation == m_current) return;
	OnAnimationChange(m_current, &animation);
	m_elapsed = 0;
	m_current = animation;

	Sprite* sprite;
	if (m_current)
	{
		bool endOfAnimation = !m_current->TimeOf(m_elapsed, &sprite);
		m_renderer->SetSprite(sprite);
	}

	if (m_adjust && m_current && m_renderer)
	{
		m_renderer->GetTransform()->SetLocalPosition(m_current->GetAdjust());
	}
}

void Animator::SetDefaultAnimation(Animation* defaultAnimation, bool overlap)
{
	Animation* prev = m_default;
	m_default = defaultAnimation;
	if (overlap && prev != m_default) PlayAnimation(m_default);
}

Animation* Animator::GetCurrentAnimation() const
{
	return m_current;
}

Animation* Animator::GetDefaultAnimation() const
{
	return m_default;
}

void Animator::AutoConnectToSpriteRenderer()
{
	if (m_renderer) return;
	m_renderer = GetComponentInChild<SpriteRenderer>();
}

void Animator::PauseAnimation()
{
	m_pause = true;
}

void Animator::ResumeAnimation()
{
	m_pause = false;
}

void Animator::PlayDefaultAnimation(bool overlap)
{
	PlayAnimation(m_default, overlap);
}

void Animator::SetSpeed(float speed)
{
	m_speed = speed;
}

float Animator::GetSpeed() const
{
	return m_speed;
}

float Animator::GetElapsed() const
{
	return m_elapsed;
}

float Animator::GetPercent() const
{
	if (!m_current)
		return 0.0f;
	return (m_elapsed - Time::GetDeltaTime()) / m_current->GetMaximumTime();
}

int Animator::GetFrameIndex() const
{
	if (!m_current)
		return -1;
	return int(GetPercent() * float(m_current->GetSpriteCount()));
}

void Animator::SetTransition(bool transition)
{
	m_transition = transition;
}

void Animator::SetAdjust(bool enable)
{
	m_adjust = enable;
}
