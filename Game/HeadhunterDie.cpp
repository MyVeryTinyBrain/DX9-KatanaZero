#include "stdafx.h"
#include "HeadhunterDie.h"
#include "StaticBackground.h"
#include "GameRenderOrder.h"
#include "PlayerController.h"
#include "Headhunter.h"
#include "TimeController.h"
#include "UI.h"
#include "SoundMgr.h"
#include "StageScene.h"

#define DELAT_FOR_CHANGE_TO_NEXT_SCENE (2.5f)

void HeadhunterDie::Awake()
{
	m_spr = new Sprite(L"../Texture/map/spr_outside_vault_bg_0_white.png");
	m_renderer = AddComponent<SpriteRenderer>();
	m_renderer->SetRenderOrder(RENDER_ORDER_FILL_SCREEN);
	m_renderer->SetSprite(m_spr);
	m_renderer->SetEnable(false);

	m_maxPlayTime = 2.5f;
}

void HeadhunterDie::Start()
{
	for (auto& bg : StaticBackground::g_backgrounds)
	{
		if (bg->GetOrder() == RENDER_ORDER_BG_BACK)
		{
			GetTransform()->SetPosition(bg->GetTransform()->GetPosition());
			GetTransform()->SetAngle(bg->GetTransform()->GetAngle());
			GetTransform()->SetScale(bg->GetTransform()->GetScale());
			break;
		}
	}
}

void HeadhunterDie::Update()
{
	SlowCutScene();

	if (m_readyToChangeToNextScene)
	{
		if (m_changeToNextSceneCounter > 0)
		{
			m_changeToNextSceneCounter -= Time::GetDeltaTime();
		}
		else
		{
			ToChangeToNextScene();
			GetGameObject()->Destroy();
		}
	}
}

void HeadhunterDie::OnDestroy()
{
	SAFE_DELETE(m_spr);
}

void HeadhunterDie::SetPlayerController(PlayerController* player)
{
	m_player = player;
}

void HeadhunterDie::SetHeadhunter(Headhunter* headhunter)
{
	m_headhunter = headhunter;
}

void HeadhunterDie::Play()
{
	m_play = true;
	m_slowCutSceneElapsed = 0;

	m_player->SetSlowControll(false);
	m_player->SetSlowModeColorChange(false);
	m_player->godMode = true;
	m_headhunter->SetSlowModeColorChange(false);

	m_renderer->SetEnable(true);
	m_renderer->SetColor(Color(1, 1, 1, 1));

	auto headhunterRenderer = m_headhunter->GetRenderer();
	auto playerRenderer = m_player->GetRenderer();

	m_headhunterRenderOrder = headhunterRenderer->GetRenderOrder();
	m_playerRenderOrder = playerRenderer->GetRenderOrder();

	headhunterRenderer->SetColor(Color(0, 0, 0, 1));
	playerRenderer->SetColor(Color(0, 0, 0, 1));

	playerRenderer->SetRenderOrder(RENDER_ORDER_FILL_SCREEN_OBJECT + 1);
	headhunterRenderer->SetRenderOrder(RENDER_ORDER_FILL_SCREEN_OBJECT);

	TimeController::GetInstance()->SetScale(0.0f);
	TimeController::GetInstance()->SetSubScale(0.0f, 0.0f);

	UI::GetInstance()->SetEnable(false);

	m_firstBGMVolume = CSoundMgr::Get_Instance()->GetBGMVolume();
}

void HeadhunterDie::SlowCutScene()
{
	if (!m_play) return;

	float percent = m_slowCutSceneElapsed / m_maxPlayTime;
	if (percent >= 1.0f) percent = 1.0f;

	float timeScale = powf(percent, 10);;
	TimeController::GetInstance()->SetScale(timeScale);

	float screenAlpha = 1 - powf(percent, 10);
	m_renderer->SetColor(Color(1, 1, 1, screenAlpha));

	auto playerRenderer = m_player->GetRenderer();
	auto headhunterRenderer = m_headhunter->GetRenderer();
	float characterColorPercent = powf(percent, 10);;
	playerRenderer->SetColor(Color(characterColorPercent, characterColorPercent, characterColorPercent, 1.0f));
	headhunterRenderer->SetColor(Color(characterColorPercent, characterColorPercent, characterColorPercent, 1.0f));

	CSoundMgr::Get_Instance()->SetBGMVolume((1 - percent) * m_firstBGMVolume);

	if (percent >= 1.0f)
	{
		m_player->SetSlowControll(true);
		m_player->SetSlowModeColorChange(true);
		m_player->godMode = false;
		m_headhunter->SetSlowModeColorChange(true);

		playerRenderer->SetRenderOrder(m_playerRenderOrder);
		headhunterRenderer->SetRenderOrder(m_playerRenderOrder);

		playerRenderer->SetColor(Color(characterColorPercent, characterColorPercent, characterColorPercent, 1.0f));
		headhunterRenderer->SetColor(Color(characterColorPercent, characterColorPercent, characterColorPercent, 1.0f));

		TimeController::GetInstance()->SetScale(1.0f);

		UI::GetInstance()->SetEnable(true);

		CSoundMgr::Get_Instance()->SetBGMVolume(0);

		m_readyToChangeToNextScene = true;
		m_changeToNextSceneCounter = DELAT_FOR_CHANGE_TO_NEXT_SCENE;
		m_play = false;
	}

	m_slowCutSceneElapsed += Time::GetUnscaledDeltaTime();
}

void HeadhunterDie::ToChangeToNextScene()
{
	auto scene = SceneManager::GetCurrentScene();
	StageScene* stage = dynamic_cast<StageScene*>(scene);
	stage->ChangeToNextScene();
}
