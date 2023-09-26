#pragma once

#include "RecordInfo.h"

class Recorder : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Render);
	COMPONENT_OVERRIDE(AfterRender);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC static Recorder* GetInstance();

	PUBLIC void RecordSprite(const SpriteRecordInfo& info);
	PUBLIC void RecordLine(const WorldLineRecordInfo& info);
	PUBLIC void Play();
	PUBLIC void SetSpeed(float speed);
	PUBLIC void PlayReverse();
	PUBLIC bool IsPlaying() const;
	PUBLIC bool IsPlayingReverse() const;
	PUBLIC void Skip();
	PUBLIC void Pause();
	PUBLIC void Resume();

	PUBLIC void SetCanChangeNextSceneWhenPlay(bool enable);
	PUBLIC void SetCanReloadSceneWhenPlayReverse(bool enable);

	PRIVATE float GetClosetKeyFromCameraRecords(float value);
	PRIVATE float GetClosetKeyFromSpriteRecords(float value);
	PRIVATE float GetClosetKeyFromLineRecords(float value);
	PRIVATE void DrawSprite(SpriteRecordInfo* info);
	PRIVATE void DrawLine(WorldLineRecordInfo* info);
	PRIVATE void DrawNoise();
	PRIVATE void DrawBackgroundColor();
	PRIVATE void WritePlayStateText();
	PRIVATE void SetPlayStateText(const wstring& text);
	PRIVATE wstring ToTimeFormat(float secondsValue);

	PRIVATE float GetFirstKeyFromCameraRecords() const;
	PRIVATE float GetFirstKeyFromSpriteRecords() const;
	PRIVATE float GetFirstKeyFromLineRecords() const;
	PRIVATE float GetLastKeyFromCameraRecords() const;
	PRIVATE float GetLastKeyFromSpriteRecords() const;
	PRIVATE float GetLastKeyFromLineRecords() const;

	PRIVATE float m_elapsed;
	PRIVATE std::map<float, std::vector<SpriteRecordInfo*>> m_spriteRecords;
	PRIVATE std::map<float, std::vector<WorldLineRecordInfo*>> m_lineRecords;
	PRIVATE std::map<float, CameraRecordInfo*> m_cameraPosition;

	PRIVATE std::vector<float> m_cameraKeys;
	PRIVATE std::vector<float> m_spriteKeys;
	PRIVATE std::vector<float> m_lineKeys;

	PRIVATE bool m_playing;
	PRIVATE bool m_playingReverse;
	PRIVATE bool m_isEndPlay;
	PRIVATE bool m_pause;
	PRIVATE float m_cursor;
	PRIVATE float m_playSpeed;

	PRIVATE Sprite* m_sprNoise[4];
	PRIVATE Sprite* m_sprWhite;
	PRIVATE float m_noiseElapsed;

	PRIVATE bool m_showPlayStateText;
	PRIVATE float m_playStateTextAlpha;
	PRIVATE wstring m_playStateText;

	PRIVATE bool m_canChangeNextSceneWhenPlay;
	PRIVATE bool m_canReloadSceneWhenPlayReverse;

	PRIVATE static Recorder* g_instance;

	PUBLIC Delegate<void> OnEndPlayReverse;
};

