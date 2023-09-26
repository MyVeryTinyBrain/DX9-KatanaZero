#pragma once

#include "TransformableRect.h"
#include "IJsonConvert.h"

class CameraBorderBox : public TransformableRect, public IJsonConvert 
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(Render);

	PUBLIC virtual Json::Value ToJson() const override;
	PUBLIC virtual string JsonType() const override;

	PUBLIC void Reset();
	PUBLIC void FitToBackground();

	PUBLIC bool m_show;
};

