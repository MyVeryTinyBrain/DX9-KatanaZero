#pragma once

class ITriggerCallback;

class Trigger : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(OnDestroy);

	PRIVATE void OnTriggerEnter(Collider* collider);

	PUBLIC void SetCallback(ITriggerCallback* callback);
	PUBLIC void SetSize(float width, float height);
	PUBLIC Body* GetBody() const;
	PUBLIC BoxCollider* GetCollider() const;

	PUBLIC static Trigger* GetTrigger(size_t index);
	PUBLIC static Trigger* FindWithName(const wstring& name);
	PUBLIC static size_t GetTriggerCounter();

	PRIVATE Body* m_body;
	PRIVATE BoxCollider* m_triggerBox;
	PRIVATE ITriggerCallback* m_callback;
	PRIVATE static std::vector<Trigger*> g_triggers;
};

