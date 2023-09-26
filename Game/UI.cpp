#include "stdafx.h"
#include "UI.h"
#include "Cursor.h"

UI* UI::g_instance = nullptr;

void UI::Awake()
{
	g_instance = this;

	auto cursorObj = new GameObject;
	cursorObj->AddComponent<Cursor>();

	m_sprBackHud = new Sprite(L"../Texture/ui/used/spr_hud_0.png");
	m_sprBattery = new Sprite(L"../Texture/ui/used/spr_hud_battery_0.png");
	m_sprBatteryFront = new Sprite(L"../Texture/ui/used/spr_hud_battery_0_front.png");
	m_sprShift = new Sprite(L"../Texture/ui/used/spr_keyboard_shift_0.png");
	m_sprShiftPress = new Sprite(L"../Texture/ui/used/spr_keyboard_shift_1.png");
	m_sprBatteryPart = new Sprite(L"../Texture/ui/used/spr_battery_part_0.png");
	m_sprTimer = new Sprite(L"../Texture/ui/used/spr_hud_timer_0.png");
	m_sprTimerIcon = new Sprite(L"../Texture/ui/used/spr_hud_timer_icon.png");
	m_sprTimerBar = new Sprite(L"../Texture/ui/used/spr_timer_0.png");
	m_sprSlot = new Sprite(L"../Texture/ui/used/spr_hud_subweapon_0.png");
	m_sprSlotFront = new Sprite(L"../Texture/ui/used/spr_hud_subweapon_0_front.png");
	m_sprKatanaIcon = new Sprite(L"../Texture/ui/used/spr_katanaicons_0.png");
	m_sprHandIcon = new Sprite(L"../Texture/ui/used/spr_itemicons_0.png");

	m_scale = Vec2(2, 2);

	m_batteryMax = 11;
	m_batteryPercent = 1.0f;
	m_timePercent = 1.0f;
	m_weaponIcon = nullptr;
	m_throwIcon = nullptr;
}

void UI::Render()
{
	{
		float scale = Screen::GetWidth() * 0.0015f;
		m_scale = Vec2::one() * scale;
	}

	auto spr = GraphicDevice::GetSpriteDevice();
	float screenWidth = float(Screen::GetWidth());
	float screenHeight = float(Screen::GetHeight());
	const Mat4& cameraToScreen = Screen::GetCameraToScreenMatrix();

	float hudXRatio = screenWidth / m_sprBackHud->GetSize().x;
	float hudYHalf = m_sprBackHud->GetSize().y * 0.5f * m_scale.y;
	Vec2 hudScreenPos = VEC2_WORLD_TO_WINDOW(Vec2(0, screenHeight * 0.5f - hudYHalf));
	Vec2 hudScale = Vec2(hudXRatio, m_scale.y);
	Mat4 matBackHud = Mat4::SRT(hudScale, Vec3::zero(), hudScreenPos);
	spr->DrawSprite(m_sprBackHud, matBackHud * cameraToScreen, Color(1, 1, 1, 1));

	{
		float batteryXPos = -screenWidth * 0.5f + m_sprBattery->GetSize().x * 0.5f * m_scale.x;
		float batteryYPos = screenHeight * 0.5f - m_sprBattery->GetSize().y * 0.5f * m_scale.y;
		Vec2 batteryCenter = Vec2(batteryXPos, batteryYPos);
		Vec2 batteryScreenPos = VEC2_WORLD_TO_WINDOW(batteryCenter);
		Mat4 matBattery = Mat4::SRT(m_scale, Vec3::zero(), batteryScreenPos);
		spr->DrawSprite(m_sprBattery, matBattery * cameraToScreen, Color(1, 1, 1, 1));

		{
			spr->DrawSprite(m_sprBatteryFront, matBattery, Color(1, 1, 1, 1));
		}

		{
			float batteryFrontWidth = m_sprBatteryFront->GetSize().x * m_scale.x;
			float batteryMinX = batteryCenter.x - batteryFrontWidth * 0.5f;
			float batteryMaxX = batteryCenter.x + batteryFrontWidth * 0.5f;
			float batteryPartWidth = batteryFrontWidth / float(m_batteryMax);
			size_t batteryCount = size_t(m_batteryPercent * float(m_batteryMax));

			float xRatio = batteryFrontWidth / ((m_sprBatteryPart->GetSize().x + 0.5f) * m_batteryMax);
			Vec2 partScale = Vec2(xRatio, m_scale.y);

			for (size_t i = 0; i < m_batteryMax; ++i)
			{
				float partX = batteryMinX + batteryPartWidth * 0.5f + batteryPartWidth * i;
				Vec2 partScreenPos = VEC2_WORLD_TO_WINDOW(Vec2(partX, batteryYPos));
				Mat4 matBatteryPart = Mat4::SRT(partScale, Vec3::zero(), partScreenPos);
				spr->DrawSprite(m_sprBatteryPart, matBatteryPart * cameraToScreen, Color(1, 0, 0, 1));
			}

			for (size_t i = 0; i < batteryCount; ++i)
			{
				float partX = batteryMinX + batteryPartWidth * 0.5f + batteryPartWidth * i;
				Vec2 partScreenPos = VEC2_WORLD_TO_WINDOW(Vec2(partX, batteryYPos));
				Mat4 matBatteryPart = Mat4::SRT(partScale, Vec3::zero(), partScreenPos);
				spr->DrawSprite(m_sprBatteryPart, matBatteryPart * cameraToScreen, Color(1, 1, 1, 1));
			}
		}

		{
			Sprite* sprCurrentShift = nullptr;
			if (Input::GetKey(Key::LShift)) sprCurrentShift = m_sprShiftPress;
			else sprCurrentShift = m_sprShift;

			float batteryEnd = batteryXPos + m_sprBattery->GetSize().x * 0.5f * m_scale.x;
			float shiftXPos = batteryEnd + sprCurrentShift->GetSize().x * 0.5f * m_scale.x;
			Vec2 shiftScreenPos = VEC2_WORLD_TO_WINDOW(Vec2(shiftXPos, batteryYPos));
			Mat4 matShift = Mat4::SRT(m_scale, Vec3::zero(), shiftScreenPos);
			spr->DrawSprite(sprCurrentShift, matShift * cameraToScreen, Color(1, 1, 1, 1));
		}
	}

	{
		float timeHudX = 0;
		float timeHudY = screenHeight * 0.5f - m_sprTimer->GetSize().y * 0.5f * m_scale.y + 3;
		Vec2 timeHudScreenPos = VEC2_WORLD_TO_WINDOW(Vec2(timeHudX, timeHudY));
		Vec2 timeHudScale = Vec2(m_scale.x, m_scale.y);
		Mat4 matTimeHud = Mat4::SRT(timeHudScale, Vec3::zero(), timeHudScreenPos);
		spr->DrawSprite(m_sprTimer, matTimeHud * cameraToScreen, Color(1, 1, 1, 1));

		Vec2 timeBarPos = Vec2(timeHudX, timeHudY) + Vec2::left();
		Vec2 timeBarScreenPos = VEC2_WORLD_TO_WINDOW(timeBarPos);
		Vec2 timeBarScale = Vec2(m_scale.x, m_scale.y);
		Mat4 matTimeBar = Mat4::SRT(timeBarScale, Vec3::zero(), timeBarScreenPos);
		m_sprTimerBar->m_type = SpriteType::SlicedRect;
		m_sprTimerBar->U.USlicedRect.slicedRect.left = 0;
		m_sprTimerBar->U.USlicedRect.slicedRect.right = float(m_sprTimerBar->GetTexture()->GetImageInfo().Width) * m_timePercent;
		m_sprTimerBar->U.USlicedRect.slicedRect.top = 0;
		m_sprTimerBar->U.USlicedRect.slicedRect.bottom = m_sprTimerBar->GetTexture()->GetImageInfo().Height;
		m_sprTimerBar->U.USlicedRect.dontChangeCenter = true;
		spr->DrawSprite(m_sprTimerBar, matTimeBar * cameraToScreen, Color(1, 1, 1, 1));

		float timeHudMinX = timeHudX - m_sprTimer->GetSize().x * 0.5f * m_scale.x;
		Vec2 timeIconPos = Vec2(timeHudMinX, timeHudY) + Vec2::left() * m_sprTimerIcon->GetSize().x * 0.5f;
		Vec2 timeIconScreenPos = VEC2_WORLD_TO_WINDOW(timeIconPos);
		Mat4 matTimeIcon = Mat4::SRT(timeHudScale, Vec3::zero(), timeIconScreenPos);
		spr->DrawSprite(m_sprTimerIcon, matTimeIcon * cameraToScreen, Color(1, 1, 1, 1));
	}

	{
		float slotXPos = screenWidth * 0.5f - m_sprSlot->GetSize().x * 0.5f * m_scale.x;
		float slotYPos = screenHeight * 0.5f - m_sprSlot->GetSize().y * 0.5f * m_scale.y;
		Vec2 slotCenter = Vec2(slotXPos, slotYPos);
		Vec2 slotScreenPos = VEC2_WORLD_TO_WINDOW(slotCenter);
		Mat4 matSlot = Mat4::SRT(m_scale, Vec3::zero(), slotScreenPos);
		spr->DrawSprite(m_sprSlot, matSlot * cameraToScreen, Color(1, 1, 1, 1));

		float throwXPos = +17 * m_scale.x;
		Vec2 throwPos = slotCenter + Vec2(throwXPos, 0);
		Vec2 throwScreenPos = VEC2_WORLD_TO_WINDOW(throwPos);
		Mat4 matThrow = Mat4::SRT(m_scale, Vec3::zero(), throwScreenPos);
		if (!m_throwIcon) spr->DrawSprite(m_sprHandIcon, matThrow * cameraToScreen, Color(1, 1, 1, 1));
		else spr->DrawSprite(m_throwIcon, matThrow * cameraToScreen, Color(1, 1, 1, 1));

		float weaponXPos = -17 * m_scale.x;
		Vec2 weaponPos = slotCenter + Vec2(weaponXPos, 0);
		Vec2 weaponScreenPos = VEC2_WORLD_TO_WINDOW(weaponPos);
		Mat4 matWeapon = Mat4::SRT(m_scale, Vec3::zero(), weaponScreenPos);
		if (!m_weaponIcon) spr->DrawSprite(m_sprKatanaIcon, matWeapon * cameraToScreen, Color(1, 1, 1, 1));
		else spr->DrawSprite(m_weaponIcon, matWeapon * cameraToScreen, Color(1, 1, 1, 1));

		spr->DrawSprite(m_sprSlotFront, matSlot * cameraToScreen, Color(1, 1, 1, 1));
	}
}

void UI::OnDestroy()
{
	if (g_instance == this)
		g_instance = nullptr;

	SAFE_DELETE(m_sprBackHud);
	SAFE_DELETE(m_sprBattery);
	SAFE_DELETE(m_sprBatteryFront);
	SAFE_DELETE(m_sprShift);
	SAFE_DELETE(m_sprShiftPress);
	SAFE_DELETE(m_sprBatteryPart);
	SAFE_DELETE(m_sprTimer);
	SAFE_DELETE(m_sprTimerIcon);
	SAFE_DELETE(m_sprTimerBar);
	SAFE_DELETE(m_sprSlot);
	SAFE_DELETE(m_sprSlotFront);
	SAFE_DELETE(m_sprKatanaIcon);
	SAFE_DELETE(m_sprHandIcon);

	SAFE_DELETE(m_weaponIcon);
	SAFE_DELETE(m_throwIcon);
}

UI* UI::GetInstance()
{
	return g_instance;
}

void UI::SetBatteryPercent(float percent)
{
	if (percent > 1.0f) percent = 1.0f;
	else if (percent < 0.0f)percent = 0.0f;
	m_batteryPercent = percent;
}

void UI::SetTimePercent(float percent)
{
	if (percent > 1.0f) percent = 1.0f;
	else if (percent < 0.0f)percent = 0.0f;
	m_timePercent = percent;
}

void UI::SetThrowIconKey(const wstring& key)
{
	SAFE_DELETE(m_throwIcon);
	m_throwIcon = new Sprite(key);
}

void UI::SetThrowIconToEmpty()
{
	SAFE_DELETE(m_throwIcon);
}

