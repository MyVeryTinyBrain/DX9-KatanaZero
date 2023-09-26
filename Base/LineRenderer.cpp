#include "base_stdafx.h"
#include "LineRenderer.h"
#include "GraphicDevice.h"
#include "LineDevice.h"

void LineRenderer::Awake()
{
	m_inWorld = true;
	m_color = Color(1, 1, 1, 1);
	m_width = 1;
}

void LineRenderer::Render()
{
	if (m_vertices.size() < 2) return;

	Mat4 transformMatrix;
	if (m_inWorld) transformMatrix = Mat4::Identity();
	else transformMatrix = GetTransform()->LocalToWorldMatrix();

	LineDevice* line = GraphicDevice::GetLineDevice();
	Draw2DLineParameters p;
	p.width = m_width;
	p.color = m_color;
	p.inWorld = true;

	Vec2 prev = m_vertices[0] * transformMatrix;
	for (size_t i = 1; i < GetVertexCount(); ++i)
	{
		size_t index = i == GetVertexCount() ? 0 : i;
		Vec2 current = m_vertices[index] * transformMatrix;
		line->Draw2DLine(prev, current, &p);
		prev = current;
	}
}

void LineRenderer::AddVertex(const Vec2& vertex)
{
	m_vertices.push_back(vertex);
}

void LineRenderer::GetVertex(size_t index, Vec2* pVertex) const
{
	*pVertex = m_vertices[index];
}

void LineRenderer::RemoveVertex(size_t index)
{
	auto it = std::next(m_vertices.begin(), index);
	m_vertices.erase(it);
}

void LineRenderer::ClearVertices()
{
	m_vertices.clear();
}

size_t LineRenderer::GetVertexCount() const
{
	return m_vertices.size();
}

void LineRenderer::SetDrawSpace(bool inWorld)
{
	m_inWorld = inWorld;
}

bool LineRenderer::IsDrawingInWorld() const
{
	return m_inWorld;
}

void LineRenderer::SetColor(const Color& color)
{
	m_color = color;
}

const Color& LineRenderer::GetColor() const
{
	return m_color;
}

void LineRenderer::SetWidth(float width)
{
	m_width = width;
}

float LineRenderer::GetWidth() const
{
	return m_width;
}
