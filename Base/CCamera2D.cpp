#include "base_stdafx.h"
#include "CCamera2D.h"
#include "Screen.h"

IMPLEMENT_SINGLETON(CCamera2D)

IMPLEMENT_SINGLETON_CONSTRUCTOR(CCamera2D)
{
	SetTransform({}, {}, { 1,1 });
}

IMPLEMENT_SINGLETON_DESTRUCTOR(CCamera2D)
{
}

void CCamera2D::SetPosition(const Vec2& position)
{
	m_position = position;
	UpdateMatrices();
}

void CCamera2D::SetAngle(const float& angle)
{
	m_angle = angle;
	UpdateMatrices();
}

void CCamera2D::SetScale(const Vec2& scale)
{
	m_scale = scale;
	UpdateMatrices();
}

void CCamera2D::SetTransform(const Vec2& position, const float& angle, const Vec2& scale)
{
	m_position = position;
	m_angle = angle;
	m_scale = scale;
	UpdateMatrices();
}

const Vec2& CCamera2D::GetPosition() const
{
	return m_position;
}

const float& CCamera2D::GetAngle() const
{
	return m_angle;
}

const Vec2& CCamera2D::GetScale() const
{
	return m_scale;
}

const Mat4& CCamera2D::GetWorldToCameraMatrix() const
{
	return m_worldToCamera;
}

const Mat4& CCamera2D::GetCameraToWorldMatrix() const
{
	return m_cameraToWorld;
}

void CCamera2D::UpdateMatrices()
{
	//// TRS
	//m_worldToCamera =
	//	Mat4::Translate(VEC2_WORLD_TO_WINDOW(-m_position)) *
	//	Mat4::Rotate({0, 0, -m_angle * DEG2RAD }) *
	//	Mat4::Scale({ 1.0f / m_scale.x, 1.0f / m_scale.y, 1.0f });
	// 	SRT
	//m_cameraToWorld =
	//	Mat4::Scale({ m_scale.x, m_scale.y, 1.0f }) *
	//	Mat4::Rotate({ 0, 0, m_angle * DEG2RAD }) *
	//	Mat4::Translate(VEC2_WINDOW_TO_WORLD(m_position));
	// TSR
	m_worldToCamera =
		Mat4::Translate(VEC2_WORLD_TO_WINDOW(-m_position)) *
		Mat4::Scale({ 1.0f / m_scale.x, 1.0f / m_scale.y, 1.0f }) *
		Mat4::Rotate({ 0, 0, -m_angle * DEG2RAD });
	// RST
	m_cameraToWorld =
		Mat4::Rotate({ 0, 0, m_angle * DEG2RAD }) *
		Mat4::Scale({ m_scale.x, m_scale.y, 1.0f }) *
		Mat4::Translate(VEC2_WINDOW_TO_WORLD(m_position));
}

void CCamera2D::SetWidth(float width)
{
	float screenWidth = float(Screen::GetWidth());
	m_scale.x = width / screenWidth;
	UpdateMatrices();
}

void CCamera2D::SetHeight(float height)
{
	float screenHeight = float(Screen::GetHeight());
	m_scale.y = height / screenHeight;
	UpdateMatrices();
}

float CCamera2D::GetWidth() const
{
	float screenWidth = float(Screen::GetWidth()); 
	return screenWidth * m_scale.x;
}

float CCamera2D::GetHeight() const
{
	float screenHeight = float(Screen::GetHeight());
	return screenHeight * m_scale.y;
}

Vec2 CCamera2D::GetLeftTop() const
{
	float hw = GetWidth() * 0.5f;
	float hh = GetHeight() * 0.5f;
	return m_position + Vec2(hw, hh) * Mat4::Rotate({ 0,0,m_angle * DEG2RAD });
}

Vec2 CCamera2D::GetRightTop() const
{
	float hw = GetWidth() * 0.5f;
	float hh = GetHeight() * 0.5f;
	return m_position + Vec2(-hw, hh) * Mat4::Rotate({ 0,0,m_angle * DEG2RAD });
}

Vec2 CCamera2D::GetLeftBottom() const
{
	float hw = GetWidth() * 0.5f;
	float hh = GetHeight() * 0.5f;
	return m_position + Vec2(-hw, -hh) * Mat4::Rotate({ 0,0,m_angle * DEG2RAD });
}

Vec2 CCamera2D::GetRightBottom() const
{
	float hw = GetWidth() * 0.5f;
	float hh = GetHeight() * 0.5f;
	return m_position + Vec2(hw, -hh) * Mat4::Rotate({ 0,0,m_angle * DEG2RAD });
}

Vec2 CCamera2D::WorldToScreenPoint(const Vec2& world) const
{
	Vec2 p = VEC2_WORLD_TO_WINDOW(world);
	Vec4 p4 = { p.x,p.y,0,1 };
	const Mat4& worldToCamera = GetWorldToCameraMatrix();
	const Mat4& cameraToScreen = Screen::GetCameraToScreenMatrix();
	return p4 * worldToCamera * cameraToScreen;
}

Vec2 CCamera2D::ScreenToWorldPoint(const Vec2& screen) const
{
	Vec2 p = screen;
	Vec4 p4 = { p.x,p.y,0,1 };
	const Mat4& screenToCamera = Screen::GetScreenToCameraMatrix();
	const Mat4& cameraToWorld = GetCameraToWorldMatrix();
	const Vec4& worldPoint = p4 * screenToCamera * cameraToWorld;
	return VEC4_WINDOW_TO_WORLD(worldPoint);
}

Vec2 CCamera2D::WorldToScreenVector(const Vec2& world) const
{
	Vec2 p = VEC2_WORLD_TO_WINDOW(world);
	Vec4 p4 = { p.x,p.y,0,0 };
	const Mat4& worldToCamera = GetWorldToCameraMatrix();
	const Mat4& cameraToScreen = Screen::GetCameraToScreenMatrix();
	return p4 * worldToCamera * cameraToScreen;
}

Vec2 CCamera2D::ScreenToWorldVector(const Vec2& screen) const
{
	Vec2 p = screen;
	Vec4 p4 = { p.x,p.y,0,0 };
	const Mat4& screenToCamera = Screen::GetScreenToCameraMatrix();
	const Mat4& cameraToWorld = GetCameraToWorldMatrix();
	const Vec4& worldPoint = p4 * screenToCamera * cameraToWorld;
	return VEC4_WINDOW_TO_WORLD(worldPoint);
}

Vec2 CCamera2D::GetMousePositionInScreen() const
{
	POINT mousePoint;
	GetCursorPos(&mousePoint);
	ScreenToClient(Screen::GetHWND(), &mousePoint);
	return Vec2(float(mousePoint.x), float(mousePoint.y));
}

Vec2 CCamera2D::GetMousePositionInWorld() const
{
	return ScreenToWorldPoint(GetMousePositionInScreen());
}
