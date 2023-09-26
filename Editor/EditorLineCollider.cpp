#include "editor_stdafx.h"
#include "EditorLineCollider.h"

void EditorLineCollider::Render()
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

	Vec2 left = p + Vec2(-hw, 0);
	Vec2 right = p + Vec2(hw, 0);
	Vec2 v[2] = { left,right };

	line->Draw2DPolygon(v, 2, &param);
}

string EditorLineCollider::JsonType() const
{
	return "line_collider";
}
