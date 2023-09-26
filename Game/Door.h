#pragma once

#include "Animator.h"

class Animation;

class Door : public Animator
{
	PUBLIC enum class Type { CLUB, DOOR, IRON, MANSION };
	PUBLIC enum class State { CLOSING, CLOSED, OPENING, OPENED };

	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Update);
	PROTECTED virtual void OnAnimationEnd(const Animation* current) override;
	PROTECTED virtual void OnAnimationChange(const Animation* current, Animation** next) override;

	PUBLIC void SetType(Door::Type type);
	PUBLIC void Open();
	PUBLIC void Open(float horizontalDirection);
	PUBLIC void Close();
	PUBLIC bool IsOpen() const;
	PUBLIC BoxCollider* GetCollider() const;

	PRIVATE void PlayAgain();

	PRIVATE SpriteRenderer* m_renderer;
	PRIVATE Body* m_body;
	PRIVATE BoxCollider* m_boxCollider;

	PRIVATE Animation* m_closing;
	PRIVATE Animation* m_closed;
	PRIVATE Animation* m_opening;
	PRIVATE Animation* m_opened;
	PRIVATE Type m_type;
	PRIVATE State m_state;
};

