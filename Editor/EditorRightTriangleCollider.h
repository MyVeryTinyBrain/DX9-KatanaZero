#pragma once

#include "EditorCollider.h"

class EditorRightTriangleCollider : public EditorCollider
{
	COMPONENT_OVERRIDE(Render);
	PROTECTED virtual string JsonType() const override;
};

