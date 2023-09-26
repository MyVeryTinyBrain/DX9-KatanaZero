#pragma once

#include "TransformableRect.h"
#include "IJsonConvert.h"

class TriggerBox : public TransformableRect, public IJsonConvert
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Render);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC virtual Json::Value ToJson() const override;
	PUBLIC virtual string JsonType() const override;

	PUBLIC wstring m_name;
	PUBLIC static std::vector<TriggerBox*> triggers;
};

