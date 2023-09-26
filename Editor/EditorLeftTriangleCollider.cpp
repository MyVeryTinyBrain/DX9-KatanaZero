#include "editor_stdafx.h"
#include "EditorLeftTriangleCollider.h"

void EditorLeftTriangleCollider::Render()
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

	Vec2 rt = p + Vec2(hw, hh);
	Vec2 rb = p + Vec2(hw, -hh);
	Vec2 lb = p + Vec2(-hw, -hh);
	Vec2 v[3] = { rt,rb,lb };

	line->Draw2DPolygon(v, 3, &param);
}

string EditorLeftTriangleCollider::JsonType() const
{
	return "left_triangle_collider";
}
