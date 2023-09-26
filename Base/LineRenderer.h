#pragma once

#include "Component.h"

class LineRenderer : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Render);

	PUBLIC void AddVertex(const Vec2& vertex);
	PUBLIC void GetVertex(size_t index, Vec2* pVertex) const;
	PUBLIC void RemoveVertex(size_t index);
	PUBLIC void ClearVertices();
	PUBLIC size_t GetVertexCount() const;
	PUBLIC void SetDrawSpace(bool inWorld);
	PUBLIC bool IsDrawingInWorld() const;
	PUBLIC void SetColor(const Color& color);
	PUBLIC const Color& GetColor() const;
	PUBLIC void SetWidth(float width);
	PUBLIC float GetWidth() const;

	PRIVATE std::vector<Vec2> m_vertices;
	PRIVATE bool m_inWorld; // world or local
	PRIVATE Color m_color;
	PRIVATE float m_width;
};

