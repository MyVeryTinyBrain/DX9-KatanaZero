#include "editor_stdafx.h"
#include "EditorRightTriangleCollider.h"

void EditorRightTriangleCollider::Render()
{
	EditorCollider::Render();

	LineDevice* line = GraphicDevice::GetLineDevice();
	const Vec2& p = GetTransform()->GetPosition();
	const float& x = p.x;
	const float& y = p.y;
	float hw = GetWidth() * 0.5f;
	float hh = GetHeight() * 0.5f;

	Draw2DLineParameters param;
	param.color = Color(0, 1, 0, 1.0f);

	Vec2 lt = p + Vec2(-hw, hh);
	Vec2 lb = p + Vec2(-hw, -hh);
	Vec2 rb = p + Vec2(hw, -hh);
	Vec2 v[3] = { lt,lb,rb };

	line->Draw2DPolygon(v, 3, &param);
}

string EditorRightTriangleCollider::JsonType() const
{
	return "right_triangle_collider";
}
