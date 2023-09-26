#include "stdafx.h"
#include "TrailRenderer.h"

void TrailRenderer::Awake()
{
	m_renderer = AddComponent<SpriteRenderer>();
	m_useTimeScale = true;
	ResetElapsed();
	SetDuration(0.25f);
	SetBeginColor(Color(0, 1, 1, 0.9f));
	SetEndColor(Color(1, 0, 1, 0.0f));
	m_useScale = false;
	m_firstScale = Vec2::one();
}

void TrailRenderer::Start()
{
	m_firstScale = GetTransform()->GetScale();
}

void TrailRenderer::Update()
{
	float percent = m_elapsed / m_duration;
	Color color;
	D3DXColorLerp(&color, &m_begColor, &m_endColor, percent);
	m_renderer->SetColor(color);
	
	if (m_useScale)
	{
		Vec2 scale = Vec2::Lerp(m_firstScale, Vec2::one() * 0.05f, percent);
		m_renderer->GetTransform()->SetScale(scale);
	}

	if (percent > 1.0f)
	{
		GetGameObject()->Destroy();
	}
	if (m_useTimeScale)
		m_elapsed += Time::GetDeltaTime();
	else 
		m_elapsed += Time::GetUnscaledDeltaTime();
}

void TrailRenderer::OnDestroy()
{
	Sprite* sprite = m_renderer->GetSprite();
	m_renderer->SetSprite(nullptr);
	SAFE_DELETE(sprite);
}

TrailRenderer* TrailRenderer::Create(SpriteRenderer* renderer)
{
	GameObject* trailObject = new GameObject();
	TrailRenderer* trail = trailObject->AddComponent<TrailRenderer>();
	trail->SetSprite(renderer);
	return trail;
}

TrailRenderer* TrailRenderer::Create(const Vec2& position, Sprite* sprite, int order)
{
	GameObject* trailObject = new GameObject();
	TrailRenderer* trail = trailObject->AddComponent<TrailRenderer>();

	Sprite* rendererSprite = sprite;

	if (rendererSprite)
	{
		Sprite* newSprite = new Sprite(*rendererSprite);
		trail->m_renderer->SetSprite(newSprite);
		trail->m_renderer->SetRenderOrder(order);
		trail->m_renderer->GetTransform()->SetPosition(position);
	}

	return trail;
}

void TrailRenderer::SetSprite(SpriteRenderer* renderer)
{
	Sprite* rendererSprite = renderer->GetSprite();
	if (!rendererSprite) return;

	Sprite* newSprite = new Sprite(*rendererSprite);
	m_renderer->SetSprite(newSprite);
	m_renderer->SetRenderOrder(renderer->GetRenderOrder() - 1);

	Transform* rt = renderer->GetTransform();
	GetTransform()->SetTransform(rt->GetPosition(), rt->GetAngle(), rt->GetScale());
}

void TrailRenderer::ResetElapsed()
{
	m_elapsed = 0;
}

void TrailRenderer::SetDuration(float duration)
{
	m_duration = duration;
}

void TrailRenderer::SetBeginColor(const Color& color)
{
	m_begColor = color;
}

void TrailRenderer::SetEndColor(const Color& color)
{
	m_endColor = color;
}

void TrailRenderer::SetTimeScaleMode(bool enable)
{
	m_useTimeScale = enable;
}

void TrailRenderer::SetAdditiveBlending(bool enable)
{
	m_renderer->SetAdditiveBlending(enable);
}

void TrailRenderer::SetRenderOrder(int order)
{
	m_renderer->SetRenderOrder(order);
}

void TrailRenderer::SetScaleMode(bool enable)
{
	m_useScale = enable;
}
