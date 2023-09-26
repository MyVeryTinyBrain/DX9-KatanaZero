#pragma once

#ifdef SpriteRenderer
#undef SpriteRenderer
#endif

class MainSceneScreen : public Component
{
	enum SPRITE_MIDDLEGRASS { SPRITE_MIDDLEGRASS_SIZE = 12 };

	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Start);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(OnDestroy);

	PRIVATE void AsyncResourceLoad();
	PRIVATE void MoveToNextScene();
	PRIVATE void PlayLoopAnimation();
	PRIVATE void PlayNeonBlinkAnimation();
	PRIVATE void PressSpaceNotification();

	PRIVATE Sprite* m_sprBack;
	PRIVATE Sprite* m_sprNeonFrame;
	PRIVATE Sprite* m_sprNeonZER;
	PRIVATE Sprite* m_sprNeonO;
	PRIVATE Sprite* m_sprKatana;
	PRIVATE Sprite* m_sprFrontFence;
	PRIVATE Sprite* m_arrSprLoop[SPRITE_MIDDLEGRASS_SIZE];
	PRIVATE Sprite* m_sprPressSpace;
	PRIVATE Sprite* m_sprBlack;

	PRIVATE SpriteRenderer* m_BackRenderer;
	PRIVATE SpriteRenderer* m_NeonFrameRenderer;
	PRIVATE SpriteRenderer* m_NeonZERRenderer;
	PRIVATE SpriteRenderer* m_NeonORenderer;
	PRIVATE SpriteRenderer* m_KatanaRenderer;
	PRIVATE SpriteRenderer* m_FrontFenceRenderer;
	PRIVATE SpriteRenderer* m_LoopRenderer;
	PRIVATE SpriteRenderer* m_PressSpaceRenderer;
	PRIVATE SpriteRenderer* m_BlackRenderer;

	PRIVATE float m_loopElapsed;
	PRIVATE float m_blinkElapsed;
	PRIVATE float m_pressSpaceElapsed;
	PRIVATE bool m_sceneChanging;
	PRIVATE float m_sceneChangeCounter;

	PRIVATE std::thread m_resourceLoadThread;
};

