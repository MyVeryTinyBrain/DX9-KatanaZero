#pragma once

class Headhunter;
class PlayerController;

class HeadhunterDie : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Start);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC void SetPlayerController(PlayerController* player);
	PUBLIC void SetHeadhunter(Headhunter* headhunter);

	PUBLIC void Play();

	PRIVATE void SlowCutScene();
	PRIVATE void ToChangeToNextScene();

	PRIVATE Sprite* m_spr;
	PRIVATE SpriteRenderer* m_renderer;

	PRIVATE bool m_play;
	PRIVATE float m_slowCutSceneElapsed;
	PRIVATE float m_maxPlayTime;

	PRIVATE Headhunter* m_headhunter;
	PRIVATE PlayerController* m_player;
	PRIVATE int m_headhunterRenderOrder;
	PRIVATE int m_playerRenderOrder;

	PRIVATE bool m_readyToChangeToNextScene;
	PRIVATE float m_changeToNextSceneCounter;

	PRIVATE float m_firstBGMVolume;
};

