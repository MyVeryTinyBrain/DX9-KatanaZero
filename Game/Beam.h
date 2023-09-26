#pragma once

class Beam : public Component
{
	PUBLIC enum class SoundType
	{
		SHORT, NORMAL, NOPLAY
	};

	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Start);
	COMPONENT_OVERRIDE(FixedUpdate);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(Render);
	COMPONENT_OVERRIDE(OnDestroy);
	PRIVATE void OnTriggerEnter(Collider* collider);

	PUBLIC void SetDuration(float duration);
	PUBLIC void SetWidth(float width);
	PUBLIC void SetLength(float length);

	PRIVATE void FitTriggerTransform();

	PRIVATE Sprite* m_sprCircle;
	PRIVATE Body* m_body;
	PRIVATE BoxCollider* m_trigger;
	PRIVATE float m_width;
	PRIVATE float m_length;
	PRIVATE Color m_colors[2];
	PRIVATE float m_duration;
	PRIVATE float m_elapsed;
	PRIVATE float m_waveform;

	PRIVATE float m_invPercent;

	PUBLIC SoundType m_soundType;
};

