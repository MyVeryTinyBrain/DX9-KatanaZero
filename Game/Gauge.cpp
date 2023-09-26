#include "stdafx.h"
#include "Gauge.h"
#include "GameRenderOrder.h"

#define TO_WINDOW_ANGLE (-1.0f)

void Gauge::Awake()
{
	m_renderOrder = RENDER_ORDER_WORLD_UI_GAUGE;
	m_percent = 0;
	m_sprBack = new Sprite(L"../Texture/ui/used/spr_gauge_back.png");
	m_sprFront = new Sprite(L"../Texture/ui/used/spr_gauge_front.png");
	m_render = true;
}

void Gauge::Render()
{
	if (!m_render) return;

	m_sprFront->m_type = SpriteType::SlicedRect;
	m_sprFront->U.USlicedRect.slicedRect.left = 0;
	m_sprFront->U.USlicedRect.slicedRect.right = float(m_sprBack->GetTexture()->GetImageInfo().Width) * m_percent;
	m_sprFront->U.USlicedRect.slicedRect.top = 0;
	m_sprFront->U.USlicedRect.slicedRect.bottom = m_sprBack->GetTexture()->GetImageInfo().Height;
	m_sprFront->U.USlicedRect.dontChangeCenter = true;

	auto spr = GraphicDevice::GetSpriteDevice();

	Transform* transform = GetTransform();
	Vec4 position = transform->GetPosition();
	position.z = 0.5f;
	position.w = 1.0f;
	const Mat4& localToWorld = Mat4::SRT(
		transform->GetScale(),
		{ 0,0,transform->GetAngle() * DEG2RAD * TO_WINDOW_ANGLE },
		VEC4_WORLD_TO_WINDOW(position)
	);
	const Mat4& worldToCamera = CCamera2D::GetInstance()->GetWorldToCameraMatrix();
	const Mat4& cameraToScreen = Screen::GetCameraToScreenMatrix();
	Mat4 m = localToWorld * worldToCamera * cameraToScreen;

	spr->DrawSprite(m_sprBack, m, Color(1, 1, 1, 1));
	spr->DrawSprite(m_sprFront, m, Color(1, 1, 1, 1));
}

void Gauge::OnDestroy()
{
	SAFE_DELETE(m_sprBack);
	SAFE_DELETE(m_sprFront);
}

void Gauge::SetPercent(float percent)
{
	if (percent < 0) percent = 0;
	else if (percent > 1) percent = 1;
	m_percent = percent;
}

void Gauge::Show()
{
	m_render = true;
}

void Gauge::Hide()
{
	m_render = false;
}
