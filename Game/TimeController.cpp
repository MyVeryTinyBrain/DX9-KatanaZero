#include "stdafx.h"
#include "TimeController.h"
#include "GameRenderOrder.h"
#include "SoundMgr.h"

TimeController* TimeController::g_instance = nullptr;

void TimeController::Awake()
{
	g_instance = this;
	m_currentScale = 1.0f;

	m_scale = 1.0f;
	SetScale(1);
	SetSubScale(1, 0);

	m_sprBlack = new Sprite(L"../Texture/effect/used/black/spr_black.png");
	m_renderOrder = RENDER_ORDER_BLACKSCREEN;
}

void TimeController::Update()
{
	if (m_stop) return;

	auto Lerp = [](float a, float b, float t)
	{
		return a + (b - a) * t;
	};

	if (m_subScaleTimer > 0)
	{
		m_subScaleTimer -= Time::GetUnscaledDeltaTime();
		Time::SetTimeScale(m_subScale);
		Time::SetFixedTimeScale(m_subScale);
		
		if (m_subScaleTimer < 0)
		{
			Time::SetTimeScale(m_scale);
			Time::SetFixedTimeScale(m_scale);
		}
	}
	else
	{
		float t = Time::GetUnscaledDeltaTime() * 10.0f;
		if (t > 1.0f) t = 1.0f;

		m_currentScale = Lerp(Time::GetTimeScale(), m_scale, t);
		constexpr float time_epsilon = 0.001f;

		if (fabsf(m_currentScale - m_scale) <= time_epsilon)
		{
			m_currentScale = m_scale;
		}

		Time::SetTimeScale(m_currentScale);
		Time::SetFixedTimeScale(m_currentScale);
	}

	if (m_scale < 0.5f)
	{
		CSoundMgr::Get_Instance()->SetPitch(0.25f);
	}
	else
	{
		CSoundMgr::Get_Instance()->SetPitch(1.0f);
	}
}

void TimeController::Render()
{
	if (m_stop) return;

	if (m_currentScale < 1.0f)
	{
		Vec2 sprSize = m_sprBlack->GetSize();
		float xRatio = float(Screen::GetWidth()) / sprSize.x;
		float yRatio = float(Screen::GetHeight()) / sprSize.y;

		Mat4 m = Mat4::SRT(Vec3(xRatio, yRatio, 1.0f), Vec3::zero(), Vec3(Screen::GetWidth() * 0.5f, Screen::GetHeight() * 0.5f, 0.5f));
		auto sprDev = GraphicDevice::GetSpriteDevice();
		float alpha = 1.0f - Time::GetTimeScale();
		if (alpha > 0.7f) alpha = 0.7f;
		sprDev->DrawSprite(m_sprBlack, m, Color(1, 1, 1, alpha));
	}
}

void TimeController::OnDestroy()
{
	if (g_instance == this)
	{
		SetScale(1);
		SetSubScale(1, 0);
		g_instance = nullptr;
	}

	SAFE_DELETE(m_sprBlack);
}

TimeController* TimeController::GetInstance()
{
	return g_instance;
}

void TimeController::SetScale(float scale)
{
	if (m_stop) return;

	if (scale != m_scale)
	{
		if (scale < 1.0f && m_scale >= 1.0f)
			CSoundMgr::Get_Instance()->PlaySound(L"sound_slomo_engage.wav", CSoundMgr::CHANNELID::SLOWMOTION);
		else if (scale >= 1.0f && m_scale < 1.0f)
			CSoundMgr::Get_Instance()->PlaySound(L"sound_slomo_disengage.wav", CSoundMgr::CHANNELID::SLOWMOTION);
	}

	m_scale = scale;
}

void TimeController::SetSubScale(float scale, float time)
{
	if (m_stop) return;

	m_subScaleTimer = time;
	m_subScale = scale;
}

void TimeController::Stop()
{
	m_stop = true;
	Time::SetTimeScale(0);
	Time::SetFixedTimeScale(0);
}

float TimeController::GetCurrentScale() const
{
	return m_currentScale;
}

bool TimeController::IsSlowMode() const
{
	return false;
}

bool TimeController::IsStop() const
{
	return m_stop;
}
