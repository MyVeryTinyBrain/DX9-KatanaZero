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
	// 잔상의 진행 비율(삭제까지 남은 비율)
	float percent = m_elapsed / m_duration;

	// 색상을 비율에 따라 보간합니다.
	Color color;
	D3DXColorLerp(&color, &m_begColor, &m_endColor, percent);
	m_renderer->SetColor(color);
	
	// 플래그가 활성화되어 있으면, 비율에 따라 크기를 점점 줄입니다.
	if (m_useScale)
	{
		Vec2 scale = Vec2::Lerp(m_firstScale, Vec2::one() * 0.05f, percent);
		m_renderer->GetTransform()->SetScale(scale);
	}

	// 진행 비율이 100% 이상이면 이 잔상 오브젝트를 삭제합니다.
	if (percent >= 1.0f)
	{
		GetGameObject()->Destroy();
	}

	// 이 플래그가 활성화 되어 있으면, 게임의 속도에 영향을 받습니다.
	if (m_useTimeScale)
		m_elapsed += Time::GetDeltaTime();
	// 이 플래그가 비활성화 되어 있으면, 게임의 속도에 영향을 받지 않습니다.
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
	// 생성한 잔상 스프라이트 렌더러에 원본 스프라이트 렌더러의 스프라이트를 설정합니다.
	Sprite* rendererSprite = renderer->GetSprite();
	if (!rendererSprite) return;
	Sprite* newSprite = new Sprite(*rendererSprite);
	m_renderer->SetSprite(newSprite);
	m_renderer->SetRenderOrder(renderer->GetRenderOrder() - 1);

	// 생성한 잔상 스프라이트 렌더러를 스프라이트 렌더러의 위치로 옮깁니다.
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
