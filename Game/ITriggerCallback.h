#pragma once

#include "Trigger.h"

class ITriggerCallback
{
	PUBLIC ITriggerCallback() {}
	PUBLIC virtual ~ITriggerCallback() {}
	PUBLIC virtual void OnTrigger(Collider* collider) = 0;
	PUBLIC virtual void OnUpdate() = 0;
	PUBLIC Trigger* m_trigger;
};

