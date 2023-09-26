#include "stdafx.h"
#include "StaticBackground.h"
#include "GameRenderOrder.h"

std::vector<StaticBackground*> StaticBackground::g_backgrounds;

void StaticBackground::Awake()
{
	m_scale = 1.0f;
	m_emptyColor = Color(0, 0, 0, 1);
	m_renderer = AddComponent<SpriteRenderer>();
	m_renderer->SetRenderOrder(RENDER_ORDER_BG_BACK);

	g_backgrounds.push_back(this);
}

void StaticBackground::Update()
{
	GetTransform()->SetScale(Vec2::one() * m_scale);
	if (m_renderer->GetRenderOrder() == RENDER_ORDER_BG_BACK)
		GraphicDevice::SetBackgroundColor(m_emptyColor);
}

void StaticBackground::OnDestroy()
{
	SAFE_DELETE(m_sprite);

	auto it = std::find(g_backgrounds.begin(), g_backgrounds.end(), this);
	if (it != g_backgrounds.end())
	{
		g_backgrounds.erase(it);
	}
}

void StaticBackground::SetSprite(const wstring& key)
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

const wstring& StaticBackground::GetKey() const
{
	return m_key;
}

void StaticBackground::SetScale(float scale)
{
	m_scale = scale;
}

float StaticBackground::GetScale() const
{
	return m_scale;
}

void StaticBackground::SetEmptyColor(const Color& color)
{
	m_emptyColor = color;
}

const Color& StaticBackground::GetEmptyColor()
{
	return m_emptyColor;
}

void StaticBackground::SetOrder(int order)
{
	m_renderer->SetRenderOrder(order);
}

int StaticBackground::GetOrder() const
{
	return m_renderer->GetRenderOrder();
}
