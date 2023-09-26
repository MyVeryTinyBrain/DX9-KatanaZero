#pragma once

class CCamera2D
{
	DECLARE_SINGLETON(CCamera2D)

	PUBLIC void SetPosition(const Vec2& position);
	PUBLIC void SetAngle(const float& angle);
	PUBLIC void SetScale(const Vec2& scale);
	PUBLIC void SetTransform(const Vec2& position, const float& angle, const Vec2& scale);
	PUBLIC const Vec2& GetPosition() const;
	PUBLIC const float& GetAngle() const;
	PUBLIC const Vec2& GetScale() const;
	PUBLIC const Mat4& GetWorldToCameraMatrix() const;
	PUBLIC const Mat4& GetCameraToWorldMatrix() const;
	PRIVATE void UpdateMatrices();

	PUBLIC void SetWidth(float width);
	PUBLIC void SetHeight(float height);
	PUBLIC float GetWidth() const;
	PUBLIC float GetHeight() const;
	PUBLIC Vec2 GetLeftTop() const;
	PUBLIC Vec2 GetRightTop() const;
	PUBLIC Vec2 GetLeftBottom() const;
	PUBLIC Vec2 GetRightBottom() const;

	PUBLIC Vec2 WorldToScreenPoint(const Vec2& world) const;
	PUBLIC Vec2 ScreenToWorldPoint(const Vec2& screen) const;
	PUBLIC Vec2 WorldToScreenVector(const Vec2& world) const;
	PUBLIC Vec2 ScreenToWorldVector(const Vec2& screen) const;
	PUBLIC Vec2 GetMousePositionInScreen() const;
	PUBLIC Vec2 GetMousePositionInWorld() const;

	PRIVATE Vec2 m_position;
	PRIVATE float m_angle;
	PRIVATE Vec2 m_scale;
	PRIVATE Mat4 m_worldToCamera;
	PRIVATE Mat4 m_cameraToWorld;
};

