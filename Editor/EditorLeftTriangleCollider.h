#pragma once

#include "EditorCollider.h"

class EditorLeftTriangleCollider : public EditorCollider
{
	COMPONENT_OVERRIDE(Render);
	PROTECTED virtual string JsonType() const override;
};

