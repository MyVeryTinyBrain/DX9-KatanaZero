#include "base_stdafx.h"
#include "TextRenderer.h"
#include "GraphicDevice.h"
#include "CCamera2D.h"
#include "Screen.h"

#define TO_WINDOW_ANGLE (-1)

void TextRenderer::Awake()
{
	m_color = Color(1, 1, 1, 1);
    m_inWorld = true;
    m_hAlignment = TextHAlignment::Center;
    m_vAlignment = TextVAlignment::Center;
}

void TextRenderer::Render()
{
	if (m_text.size() == 0) return;

    Transform* transform = GetTransform();
    WriteTextParameters p;
    p.inWorld = m_inWorld;
    p.position = transform->GetPosition();
    p.angle = transform->GetAngle();
    p.scale = transform->GetScale();
    p.color = m_color;
    p.hAlignment = m_hAlignment;
    p.vAlignment = m_vAlignment;

    GraphicDevice::GetTextDevice()->WriteText(m_text, &p);
}

void TextRenderer::SetText(const wstring& text)
{
	m_text = text;
}

const wstring& TextRenderer::GetText() const
{
	return m_text;
}

void TextRenderer::SetColor(const Color& color)
{
	m_color = color;
}

const Color& TextRenderer::GetColor() const
{
	return m_color;
}

void TextRenderer::SetRenderOrder(int order)
{
	m_renderOrder = order;
}

void TextRenderer::SetDrawInWorld(bool enable)
{
    m_inWorld = enable;
}

bool TextRenderer::IsDrawInWorld() const
{
    return m_inWorld;
}

void TextRenderer::SetHorizontalAlignment(TextHAlignment alignment)
{
    m_hAlignment = alignment;
}

void TextRenderer::SetVerticalAlignment(TextVAlignment alignment)
{
    m_vAlignment = alignment;
}
