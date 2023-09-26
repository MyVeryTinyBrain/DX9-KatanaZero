#include "stdafx.h"
#include "DownArrow.h"
#include "AnimationManager.h"
#include "Animation.h"
#include "GameRenderOrder.h"

void DownArrow::Awake()
{
	auto o = GameObject::CreateChild(GetGameObject());
	m_renderer = o->AddComponent<SpriteRenderer>();
	m_renderer->SetRenderOrder(RENDER_ORDER_WORLD_UI_DOWNARROW);
#ifdef SpriteRenderer
	m_renderer->SetRecord(false);
#endif

	Animator::Awake();

	SetAdjust(false);

	m_animation = AnimationManager::GetAnimation(L"../Animation/ui/downarrow.txt");

	PlayAnimation(m_animation);
}

void DownArrow::Update()
{
	Animator::Update();

	float localY = 40 + sinf(10 * m_elapsed) * 10;
	m_renderer->GetTransform()->SetLocalPosition(Vec2(0, localY));

	m_elapsed += Time::GetDeltaTime();
}

void DownArrow::OnAnimationEnd(const Animation* current)
{
}

void DownArrow::OnAnimationChange(const Animation* current, Animation** next)
{
}
