#include "stdafx.h"
#include "Trigger.h"
#include "ITriggerCallback.h"
#include "GameLayer.h"
#include "DebugDraw.h"

std::vector<Trigger*> Trigger::g_triggers;

void Trigger::Awake()
{
	m_body = AddComponent<Body>();
	m_body->SetGravityMode(false);
	m_body->SetType(BodyType::Dynamic);
	m_body->SetLayerIndex(LAYERINDEX_TRIGGER);
	m_body->SetAlwaysWakeMode(true);

	m_triggerBox = AddComponent<BoxCollider>();
	m_triggerBox->SetTriggerMode(true);
	m_triggerBox->OnTriggerEnter += Function<void, Collider*>(this, &Trigger::OnTriggerEnter);
	m_triggerBox->DrawDebug = DebugDraw::DRAW_TRIGGER;

	if (DebugDraw::DRAW_TRIGGER_NAME)
	{
		TextRenderer* text = AddComponent<TextRenderer>();
		text->SetText(GetGameObject()->GetName());
	}

	g_triggers.push_back(this);
}

void Trigger::Update()
{
	if (!m_callback) return;
	m_callback->OnUpdate();
}

void Trigger::OnDestroy()
{
	SAFE_DELETE(m_callback);

	m_triggerBox->OnTriggerEnter -= Function<void, Collider*>(this, &Trigger::OnTriggerEnter);

	auto it = std::find(g_triggers.begin(), g_triggers.end(), this);
	if (it == g_triggers.end()) return;
	g_triggers.erase(it);
}

void Trigger::OnTriggerEnter(Collider* collider)
{
	if (!m_callback) return;
	m_callback->OnTrigger(collider);
}

void Trigger::SetCallback(ITriggerCallback* callback)
{
	m_callback = callback;
	m_callback->m_trigger = this;
}

void Trigger::SetSize(float width, float height)
{
	m_triggerBox->SetWidth(width);
	m_triggerBox->SetHeight(height);
}

Body* Trigger::GetBody() const
{
	return m_body;
}

BoxCollider* Trigger::GetCollider() const
{
	return m_triggerBox;
}

Trigger* Trigger::GetTrigger(size_t index)
{
	return g_triggers[index];
}

Trigger* Trigger::FindWithName(const wstring& name)
{
	for (auto& trigger : g_triggers)
	{
		if (trigger->GetGameObject()->GetName() == name)
		{
			return trigger;
		}
	}
	return nullptr;
}

size_t Trigger::GetTriggerCounter()
{
	return g_triggers.size();
}
