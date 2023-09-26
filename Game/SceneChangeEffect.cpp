#include "stdafx.h"
#include "SceneChangeEffect.h"
#include "GameRenderOrder.h"
#include "SoundMgr.h"

SceneChangeEffect* SceneChangeEffect::g_instance;

void SceneChangeEffect::Awake()
{
	m_renderOrder = RENDER_ORDER_EFFECT_SCENE_CHANGE;

	m_changeSprite = new Sprite(L"../Texture/ui/used/spr_scene_change.png");

	m_xCount = 30;
	m_yCount = 20;

	m_duration = 1;
	m_elapsed = 1;
	m_delayCounter = 0;
	m_fadeIn = false;

	m_color = Color(1, 1, 1, 1);
	m_direction = SceneChangeEffectDirection::RIGHT;
	m_scaleFactor = 2.0f;

	SetDirection(SceneChangeEffectDirection::RIGHT);
	SetScaleFactor(2.0f);

	m_fading = false;

	g_instance = this;
}

void SceneChangeEffect::Render()
{
	if (!m_fading) return;

	GraphicDevice::GetSpriteDevice()->EndRender();
	GraphicDevice::GetSpriteDevice()->BeginRender();

	float percent = m_elapsed / m_duration;
	if (percent > 1.0f) percent = 1.0f;
	if (m_delayCounter > 0) percent = 0;

	float fadePercent = percent;

	if (m_fadeIn) percent = percent;
	else percent = 1.0f - percent;

	float screenWidth = float(Screen::GetWidth());
	float screenHeight = float(Screen::GetHeight());

	float xGap = screenWidth / float(m_xCount);
	float yGap = screenHeight / float(m_yCount);

	float imageWidth = m_changeSprite->GetSize().x;
	float imageHeight = m_changeSprite->GetSize().y;
	float xFitScale = xGap / imageWidth + 0.01f;
	float yFitScale = yGap / imageHeight + 0.01f;

	auto spr = GraphicDevice::GetSpriteDevice();
	for (int i = 0; i <= m_yCount; ++i)
	{
		for (int j = 0; j <= m_xCount; ++j)
		{
			float dirPercent = 1.0f;
			switch (m_direction)
			{
				case SceneChangeEffectDirection::LEFT:
					dirPercent = float(j) / float(m_xCount);
					if (m_fadeIn) dirPercent = dirPercent;
					else dirPercent = 1.0f - dirPercent;
					break;
				case SceneChangeEffectDirection::RIGHT:
					dirPercent = 1.0f - float(j) / float(m_xCount);
					if (m_fadeIn) dirPercent = dirPercent;
					else dirPercent = 1.0f - dirPercent;
					break;
			}

			float newPercent = (1.0f + dirPercent) * percent;
			if (newPercent > 1.0f) newPercent = 1.0f;

			Vec3 position = Vec3(j * xGap, i * yGap, 0.0f);
			float scaleX = newPercent * newPercent * xFitScale * m_scaleFactor;
			float scaleY = newPercent * newPercent * yFitScale * m_scaleFactor;

			Mat4 m = Mat4::SRT(Vec3(scaleX, scaleY, 1.0f), Vec3::zero(), position);
			spr->DrawSprite(m_changeSprite, m, m_color);
		}
	}

	if (fadePercent >= 1.0f)
	{
		m_fading = false;
		if (m_fadeIn) OnFadeIn();
		else OnFadeOut();
	}

	if (m_delayCounter > 0)
	{
		m_delayCounter -= Time::GetUnscaledDeltaTime();
	}
	else
	{
		m_elapsed += Time::GetUnscaledDeltaTime();
	}
}

void SceneChangeEffect::OnDestroy()
{
	SAFE_DELETE(m_changeSprite);

	if (g_instance == this)
		g_instance = nullptr;
}

SceneChangeEffect* SceneChangeEffect::GetInstance()
{
	return g_instance;
}

void SceneChangeEffect::SetCounts(size_t xCount, size_t yCount)
{
	m_xCount = xCount;
	m_yCount = yCount;
}

void SceneChangeEffect::SetDirection(SceneChangeEffectDirection direction)
{
	m_direction = direction;
}

void SceneChangeEffect::SetScaleFactor(float factor)
{
	m_scaleFactor = factor;
}

void SceneChangeEffect::FadeIn(float duration, float delay)
{
	m_elapsed = 0;
	m_duration = duration;
	m_delayCounter = delay;
	m_fadeIn = true;
	m_fading = true;

	CSoundMgr::Get_Instance()->PlaySound(L"sound_transition_begin.wav", CSoundMgr::CHANNELID::BULLET_FIRE);
}

void SceneChangeEffect::FadeOut(float duration, float delay)
{
	m_elapsed = 0;
	m_duration = duration;
	m_delayCounter = delay;
	m_fadeIn = false;
	m_fading = true;

	CSoundMgr::Get_Instance()->PlaySound(L"sound_transition_end.wav", CSoundMgr::CHANNELID::BULLET_FIRE);
}

void SceneChangeEffect::FadeInImediatley()
{
	m_elapsed = 1;
	m_duration = 1;
	m_fadeIn = true;
	m_fading = true;
}

void SceneChangeEffect::FadeOutImediatley()
{
	m_elapsed = 1;
	m_duration = 1;
	m_fadeIn = false;
	m_fading = true;
}

bool SceneChangeEffect::IsFading() const
{
	return m_fading;
}
