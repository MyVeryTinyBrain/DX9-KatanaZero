#pragma once

#include "EditorCollider.h"

class EditorLineCollider : public EditorCollider
{
	COMPONENT_OVERRIDE(Render);
	PROTECTED virtual string JsonType() const override;
};

