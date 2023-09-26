#pragma once

#include "Component.h"
#include "TextDevice.h"

class TextRenderer : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Render);

	PUBLIC void SetText(const wstring& text);
	PUBLIC const wstring& GetText() const;
	PUBLIC void SetColor(const Color& color);
	PUBLIC const Color& GetColor() const;
	PUBLIC void SetRenderOrder(int order);
	PUBLIC void SetDrawInWorld(bool enable);
	PUBLIC bool IsDrawInWorld() const;
	PUBLIC void SetHorizontalAlignment(TextHAlignment alignment);
	PUBLIC void SetVerticalAlignment(TextVAlignment alignment);

	PRIVATE bool m_inWorld;
	PRIVATE wstring m_text;
	PRIVATE Color m_color;
	PRIVATE TextHAlignment m_hAlignment;
	PRIVATE TextVAlignment m_vAlignment;
};

