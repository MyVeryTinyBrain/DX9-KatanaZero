#pragma once

#include "EditorObject.h"

class TeleportDoor : public EditorObject
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Start);
	COMPONENT_OVERRIDE(BeforeRender);
	COMPONENT_OVERRIDE(Render);
	COMPONENT_OVERRIDE(OnDestroy);

	PRIVATE void OnDestroyChildDoor(Object* object);

	PRIVATE void Release();

	PUBLIC virtual Json::Value ToJson() const override;

	PUBLIC TransformableRect* GetDoorPointA() const;
	PUBLIC TransformableRect* GetDoorPointB() const;

	PRIVATE void CreatePair();

	PRIVATE TransformableRect* m_doorPoint[2];
	PRIVATE SpriteRenderer* m_doorRenderers[2];
};

