#pragma once

#include "ITriggerCallback.h"

class NextSceneTriggerCallback : public ITriggerCallback
{
	PUBLIC NextSceneTriggerCallback();
	PROTECTED virtual void OnUpdate() override;

	PRIVATE virtual void OnTrigger(Collider* collider) override;

	PRIVATE bool m_used;

	PUBLIC bool m_useRecordPlayback;
};

