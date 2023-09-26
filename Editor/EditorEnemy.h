#pragma once

#include "EditorObject.h"

class EditorEnemy : public EditorObject
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Render);

	PUBLIC virtual Json::Value ToJson() const override;

	PUBLIC void SetRally(bool enable);

	PUBLIC bool IsRally() const;
	PUBLIC TransformableRect* GetRallyPointA() const;
	PUBLIC TransformableRect* GetRallyPointB() const;

	PRIVATE bool m_rally;
	PRIVATE TransformableRect* m_rallyPoint[2];
};

