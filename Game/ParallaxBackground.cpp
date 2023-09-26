#include "stdafx.h"
#include "ParallaxBackground.h"
#include "GameRenderOrder.h"
#include "PlayerController.h"
#include "PlayerCamera.h"

void ParallaxBackground::Awake()
{
	m_base = Vec2::zero();
	m_parallaxScale = Vec2(0.0f, 0.0f);
	m_scale = 1.0f;
	m_renderer = AddComponent<SpriteRenderer>();
	m_renderer->SetRenderOrder(RENDER_ORDER_PARALLAX_BACK);
#ifdef SpriteRenderer
	m_renderer->m_callBack = this;
#endif
}

void ParallaxBackground::LateUpdate()
{
	Vec2 camPos = CCamera2D::GetInstance()->GetPosition();
	Vec2 delta = (camPos - m_base);
	delta.x *= m_parallaxScale.x;
	delta.y *= m_parallaxScale.y;

	GetTransform()->SetTransform(m_base + delta, 0.0f, Vec2::one() * m_scale);
}

void ParallaxBackground::OnDestroy()
{
	SAFE_DELETE(m_sprite);
}

void ParallaxBackground::SetBase(const Vec2& base)
{
	m_base = base;
}

void ParallaxBackground::SetParallaxScale(const Vec2& scale)
{
	m_parallaxScale = scale;
}

void ParallaxBackground::SetScale(float scale)
{
	m_scale = scale;
}

void ParallaxBackground::SetSprite(const wstring& key)
{
	SAFE_DELETE(m_sprite);
	m_renderer->SetSprite(nullptr);

	m_key = key;
	m_sprite = new Sprite(key);

	if (!m_sprite->GetTexture())
	{
		SAFE_DELETE(m_sprite);
		return;
	}
	else
	{
		m_renderer->SetSprite(m_sprite);
	}
}

const Vec2& ParallaxBackground::GetBase() const
{
	return m_base;
}

const Vec2& ParallaxBackground::GetParallaxScale() const
{
	return m_parallaxScale;
}

float ParallaxBackground::GetScale() const
{
	return m_scale;
}

const wstring& ParallaxBackground::GetKey() const
{
	return m_key;
}

Vec2 ParallaxBackground::GetParallaxPosition(const Vec2& camPos)
{
	Vec2 delta = (camPos - m_base);
	delta.x *= m_parallaxScale.x;
	delta.y *= m_parallaxScale.y;
	return m_base + delta;
}

void ParallaxBackground::SetOrder(int order)
{
	m_renderer->SetRenderOrder(order);
}

Vec2 ParallaxBackground::GetRecordPosition()
{
	Vec2 targetCamPos = PlayerController::GetInstance()->GetTransform()->GetPosition();
	Vec2 camSize = Vec2(CCamera2D::GetInstance()->GetWidth(), CCamera2D::GetInstance()->GetHeight());
	Vec2 camPos = PlayerCamera::GetInstance()->CalculateFitInBorderPosition(targetCamPos, camSize);
	return GetParallaxPosition(camPos);
}

float ParallaxBackground::GetRecordAngle()
{
	return GetTransform()->GetAngle();
}

Vec2 ParallaxBackground::GetRecordScale()
{
	return GetTransform()->GetScale();
}

