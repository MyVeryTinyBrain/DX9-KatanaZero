#pragma once

class Cursor;

class UI : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Render);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC static UI* GetInstance();

	PUBLIC void SetBatteryPercent(float percent);
	PUBLIC void SetTimePercent(float percent);
	PUBLIC void SetThrowIconKey(const wstring& key);
	PUBLIC void SetThrowIconToEmpty();

	PRIVATE Sprite* m_sprBackHud;
	PRIVATE Sprite* m_sprBattery;
	PRIVATE Sprite* m_sprBatteryFront;
	PRIVATE Sprite* m_sprShift;
	PRIVATE Sprite* m_sprShiftPress;
	PRIVATE Sprite* m_sprBatteryPart;
	PRIVATE Sprite* m_sprTimer;
	PRIVATE Sprite* m_sprTimerIcon;
	PRIVATE Sprite* m_sprTimerBar;
	PRIVATE Sprite* m_sprSlot;
	PRIVATE Sprite* m_sprSlotFront;
	PRIVATE Sprite* m_sprKatanaIcon;
	PRIVATE Sprite* m_sprHandIcon;
	PRIVATE Cursor* m_cursor;

	PRIVATE Vec2 m_scale;

	PRIVATE size_t m_batteryMax;
	PRIVATE float m_batteryPercent;
	PRIVATE float m_timePercent;
	PRIVATE Sprite* m_weaponIcon;
	PRIVATE Sprite* m_throwIcon;

	PRIVATE static UI* g_instance;
};

