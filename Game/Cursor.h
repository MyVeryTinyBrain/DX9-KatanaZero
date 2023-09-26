#pragma once

class Cursor : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Render);
	COMPONENT_OVERRIDE(OnDestroy);

	PRIVATE Sprite* m_sprite;
};

