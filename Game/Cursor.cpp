#include "stdafx.h"
#include "Cursor.h"

void Cursor::Awake()
{
	ShowCursor(false);
	m_sprite = new Sprite(L"../Texture/ui/used/spr_cursor_0.png");
}

void Cursor::Render()
{
	Mat4 localToScreen = Mat4::SRT(
		Vec3::one() * 2,
		Vec3::zero(),
		CCamera2D::GetInstance()->GetMousePositionInScreen()
	);
	GraphicDevice::GetSpriteDevice()->DrawSprite(m_sprite, localToScreen, Color(1, 1, 1, 1));
}

void Cursor::OnDestroy()
{
	ShowCursor(true);
	SAFE_DELETE(m_sprite);
}
