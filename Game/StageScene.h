#pragma once

#include "SceneChangeEffectDirection.h"

class StageScene : public Scene
{
	PUBLIC StageScene();
	PROTECTED virtual Scene* NextScene() = 0;
	PROTECTED virtual wstring StagePath() = 0;

	PUBLIC void ChangeToNextScene();
	PUBLIC void UseFadeOutEffect(const SceneChangeEffectDirection& direction);
	PUBLIC void UseFadeInEffect(const SceneChangeEffectDirection& direction);
	PUBLIC void DontUseFadeOutEffect();
	PUBLIC void DontUseFadeInEffect();
	PUBLIC void UsePlayback();
	PUBLIC void DontUsePlayback();

	PROTECTED virtual void OnLoad(Scene* prevScene) override;
	PROTECTED virtual void OnChange(Scene* nextScene) override;
	PUBLIC virtual Scene* Copy() override = 0;

	PRIVATE void OnFadeIn();
	PRIVATE void OnFadeOut();

	PRIVATE void LoadStageJson(const wstring& path);

	PRIVATE bool m_usePlayback;

	PRIVATE float m_fadeOutDelay;
	PRIVATE float m_fadeInDelay;
	PRIVATE float m_fadeOutDuration;
	PRIVATE float m_fadeInDuration;
	PRIVATE bool m_useFadeOutEffect;
	PRIVATE bool m_useFadeInEffect;
	PRIVATE SceneChangeEffectDirection m_fadeInDirection;
	PRIVATE SceneChangeEffectDirection m_fadeOutDirection;
};

