#include "stdafx.h"
#include "StaticSprite.h"
#include "GameRenderOrder.h"

void StaticSprite::Awake()
{
	m_renderer = AddComponent<SpriteRenderer>();
	m_renderer->SetRenderOrder(RENDER_ORDER_BG_OBJECT);
}

void StaticSprite::OnDestroy()
{
	SAFE_DELETE(m_sprite);
}

void StaticSprite::SetSprite(const wstring& textureKey)
{
	m_sprite = new Sprite(textureKey);
	m_renderer->SetSprite(m_sprite);
}

SpriteRenderer* StaticSprite::GetRenderer() const
{
	return m_renderer;
}
