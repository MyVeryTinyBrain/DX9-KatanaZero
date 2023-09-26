#pragma once

#include "Component.h"

class CCamera2D;

class Camera : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(BeforeRender);

	PUBLIC static void SetMainCamera(Camera* camera);
	PUBLIC static Camera* GetMainCamera();
	PUBLIC static CCamera2D* GetCCamera2D();

	PRIVATE static Camera* g_mainCamera;
};

