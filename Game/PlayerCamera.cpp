#include "stdafx.h"
#include "PlayerCamera.h"
#include "PlayerController.h"

PlayerCamera* PlayerCamera::g_instance = nullptr;

void PlayerCamera::Awake()
{
	g_instance = this;
	m_border = FRect(-10000, 10000, 10000, -10000);
	m_useScaledTimeForShake = false;
	m_shakeDuration = 0;

	//CCamera2D::GetInstance()->SetWidth(576);
	//CCamera2D::GetInstance()->SetHeight(305);
	CCamera2D::GetInstance()->SetWidth(1920.0f * 0.34f);
	CCamera2D::GetInstance()->SetHeight(1080.0f * 0.34f);
}

void PlayerCamera::BeforeRender()
{
	if (m_stop) return;

	//CCamera2D::GetInstance()->SetWidth(576);
	//CCamera2D::GetInstance()->SetHeight(305);
	CCamera2D::GetInstance()->SetWidth(1920.0f * 0.34f);
	CCamera2D::GetInstance()->SetHeight(1080.0f * 0.34f);

	TrackingMouse();
	FitInBorder();
	Shaking();
}

void PlayerCamera::OnDestroy()
{
	if (g_instance == this)
		g_instance = nullptr;
}

PlayerCamera* PlayerCamera::GetInstance()
{
	return g_instance;
}

void PlayerCamera::Shake(const Vec2& dir, float duration, float power, float shakeCount)
{
	m_shakeCounter = 0;
	m_shakeDuration = duration;
	m_shakeDir = dir;
	m_shakePower = power;
	m_shakeCount = shakeCount;
}

void PlayerCamera::Stop()
{
	m_stop = true;
}

Vec2 PlayerCamera::CalculateFitInBorderPosition(const Vec2& position, const Vec2& size)
{
	float width = size.x;
	float height = size.y;

	float hw = width * 0.5f;
	float hh = height * 0.5f;

	float left = position.x - hw;
	float top = position.y + hh;
	float right = position.x + hw;
	float bottom = position.y - hh;

	Vec2 adjust = Vec2::zero();
	float leftOver = left - m_border.left;
	float rightOver = right - m_border.right;
	float topOver = top - m_border.top;
	float bottomOver = bottom - m_border.bottom;

	if (leftOver < 0)
		adjust.x = -leftOver;
	if (rightOver > 0)
		adjust.x = -rightOver;
	if (bottomOver < 0)
		adjust.y = -bottomOver;
	if (topOver > 0)
		adjust.y = -topOver;

	Vec2 adjustPosition = position + adjust;

	if (m_border.GetWidth() <= width)
		adjustPosition.x = (m_border.right + m_border.left) * 0.5f;
	if (m_border.GetHeight() <= height)
		adjustPosition.y = (m_border.top + m_border.bottom) * 0.5f;

	return adjustPosition;
}

void PlayerCamera::TrackingMouse()
{
	Vec2 playerPos = PlayerController::GetInstance()->GetTransform()->GetPosition();
	Vec2 mousePos = CCamera2D::GetInstance()->GetMousePositionInWorld();
	Vec2 targetPos = (playerPos + mousePos) * 0.5f;

	Vec2 currentCamPos = CCamera2D::GetInstance()->GetPosition();
	Vec2 relative = targetPos - playerPos;
	float relativeMag = relative.magnitude();
	if (relativeMag > 50) relativeMag = 50;
	Vec2 mouseToTarget = relative.normalized() * relativeMag;
	Vec2 camPos = playerPos + mouseToTarget;

	float t = Time::GetUnscaledDeltaTime() * 10;
	if (t >= 1.0f) t = 1.0f;
	if (t <= 0.0f) t = 0.0f;
	Vec2 lerp = Vec2::Lerp(currentCamPos, camPos, Time::GetUnscaledDeltaTime() * 10);

	CCamera2D::GetInstance()->SetPosition(lerp);
}

void PlayerCamera::FitInBorder()
{
	auto cam = CCamera2D::GetInstance();
	float width = cam->GetWidth();
	float height = cam->GetHeight();

	float hw = width * 0.5f;
	float hh = height * 0.5f;
	Vec2 pos = CCamera2D::GetInstance()->GetPosition();

	float left = pos.x - hw;
	float top = pos.y + hh;
	float right = pos.x + hw;
	float bottom = pos.y - hh;

	Vec2 adjust = Vec2::zero();
	float leftOver = left - m_border.left;
	float rightOver = right - m_border.right;
	float topOver = top - m_border.top;
	float bottomOver = bottom - m_border.bottom;

	if (leftOver < 0) 
		adjust.x = -leftOver;
	if (rightOver > 0) 
		adjust.x = -rightOver;
	if (bottomOver < 0) 
		adjust.y = -bottomOver;
	if (topOver > 0) 
		adjust.y = -topOver;

	pos += adjust;

	if (m_border.GetWidth() <= width)
		pos.x = (m_border.right + m_border.left) * 0.5f;
	if (m_border.GetHeight() <= height)
		pos.y = (m_border.top + m_border.bottom) * 0.5f;

	CCamera2D::GetInstance()->SetPosition(pos);
}

void PlayerCamera::Shaking()
{
	if (m_shakeDuration == 0) return;
	if (m_shakeCounter >= m_shakeDuration)
	{
		m_shakeDuration = 0;
		m_shakeCounter = 0;
		return;
	}

	float shake = sinf(TAU * m_shakeCounter * m_shakeCount) * m_shakePower;
	Vec2 adjust = m_shakeDir * shake;

	if (m_useScaledTimeForShake)
		adjust *= Time::GetDeltaTime();
	else
		adjust *= Time::GetUnscaledDeltaTime();

	Vec2 pos = CCamera2D::GetInstance()->GetPosition();
	CCamera2D::GetInstance()->SetPosition(pos + adjust);

	if (m_useScaledTimeForShake)
		m_shakeCounter += Time::GetDeltaTime();
	else 
		m_shakeCounter += Time::GetUnscaledDeltaTime();
}
