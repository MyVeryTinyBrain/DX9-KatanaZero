#include "base_stdafx.h"
#include "Camera.h"
#include "CCamera2D.h"
#include "Screen.h"
#include "CoreExecutionOrder.h"

Camera* Camera::g_mainCamera = nullptr;

void Camera::Awake()
{
	m_beforeRenderExecutionOrder = BEFORE_RENDER_ORDER_CAMREA;

	if (!g_mainCamera)
	{
		g_mainCamera = this;
	}
}

void Camera::BeforeRender()
{
	if (this == g_mainCamera)
	{
		Transform* transform = GetTransform();
		const Vec2& position = transform->GetPosition();
		const float& angle = transform->GetAngle();
		const Vec2& scale = transform->GetScale();
		CCamera2D::GetInstance()->SetTransform(position, angle, scale);
	}
}

void Camera::SetMainCamera(Camera* camera)
{
	g_mainCamera = camera;
}

Camera* Camera::GetMainCamera()
{
	return g_mainCamera;
}

CCamera2D* Camera::GetCCamera2D()
{
	return CCamera2D::GetInstance();
}
