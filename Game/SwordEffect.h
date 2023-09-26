#pragma once

#include "Animator.h"

class Animation;

class SwordEffect : public Animator
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(OnDestroy);
	PROTECTED virtual void OnAnimationEnd(const Animation* current) override;
	PROTECTED virtual void OnAnimationChange(const Animation* current, Animation** next) override;
	PRIVATE void OnTriggerEnter(Collider* collider);
	PRIVATE void OnDestroyStickedObject(Object* stickedObject);

	PUBLIC void StickToGameObject(GameObject* gameObject);
	PUBLIC BoxCollider* GetTrigger() const;

	PRIVATE void DetachGameObject();
	PRIVATE void AttachGameObject(GameObject* gameObject);

	PRIVATE GameObject* m_stickedGameObject;
	PRIVATE SpriteRenderer* m_renderer;
	PRIVATE Body* m_body;
	PRIVATE BoxCollider* m_trigger;

	PRIVATE Animation* m_slash;

	PUBLIC bool m_longTake;

	PRIVATE std::vector<Body*> m_damaged;
};

