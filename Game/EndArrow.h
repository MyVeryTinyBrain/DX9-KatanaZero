#pragma once

#include "DownArrow.h"

class EndArrow : public DownArrow
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Start);
	COMPONENT_OVERRIDE(Update);

	PRIVATE bool m_hasEnd;
	PRIVATE Vec2 m_endPos;
};

