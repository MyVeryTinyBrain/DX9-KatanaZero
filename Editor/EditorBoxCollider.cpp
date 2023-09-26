#include "editor_stdafx.h"
#include "EditorBoxCollider.h"

void EditorBoxCollider::Render()
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

	line->Draw2DRect(x - hw, y + hh, x + hw, y - hh, &param);
}

string EditorBoxCollider::JsonType() const
{
	return "box_collider";
}
