#include "stdafx.h"
#include "MainSceneScreen.h"
#include "ResourceLoader.h"
#include "Stage0.h"
#include "StageHeadhunter.h"

#define SCENE_CHANGE_WAIT_TIME (1.0f)

#ifdef SpriteRenderer
#undef SpriteRenderer
#endif

void MainSceneScreen::Awake()
{
	GraphicDevice::SetBackgroundColor(Color(0, 0, 0, 1));
	CCamera2D::GetInstance()->SetPosition(Vec2::zero());
	CCamera2D::GetInstance()->SetAngle(0.0f);
	CCamera2D::GetInstance()->SetWidth(640);
	CCamera2D::GetInstance()->SetHeight(360);
	Time::SetTimeScale(1.0f);
	Time::SetFixedTimeScale(1.0f);

	{
		auto texBack = TextureManager::LoadFromFile(L"../Texture/main/spr_title_background_0.png");
		m_sprBack = new Sprite(texBack);
		auto o = GameObject::CreateChild(GetGameObject());
		m_BackRenderer = o->AddComponent<SpriteRenderer>();
		m_BackRenderer->SetRenderOrder(0);
		m_BackRenderer->SetSprite(m_sprBack);
	}

	{
		auto texNeonFrame = TextureManager::LoadFromFile(L"../Texture/main/spr_neon_frame.png");
		m_sprNeonFrame = new Sprite(texNeonFrame);
		auto o = GameObject::CreateChild(GetGameObject());
		m_NeonFrameRenderer = o->AddComponent<SpriteRenderer>();
		m_NeonFrameRenderer->SetRenderOrder(1);
		m_NeonFrameRenderer->SetSprite(m_sprNeonFrame);
	}

	{
		auto texNeonZER = TextureManager::LoadFromFile(L"../Texture/main/spr_neon_zer.png");
		m_sprNeonZER = new Sprite(texNeonZER);
		auto o = GameObject::CreateChild(GetGameObject());
		m_NeonZERRenderer = o->AddComponent<SpriteRenderer>();
		m_NeonZERRenderer->SetRenderOrder(2);
		m_NeonZERRenderer->SetSprite(m_sprNeonZER);
	}

	{
		auto texNeonO = TextureManager::LoadFromFile(L"../Texture/main/spr_neon_o.png");
		m_sprNeonO = new Sprite(texNeonO);
		auto o = GameObject::CreateChild(GetGameObject());
		m_NeonORenderer = o->AddComponent<SpriteRenderer>();
		m_NeonORenderer->SetRenderOrder(3);
		m_NeonORenderer->SetSprite(m_sprNeonO);
	}

	{
		auto texKatana = TextureManager::LoadFromFile(L"../Texture/main/spr_katana.png");
		m_sprKatana = new Sprite(texKatana);
		auto o = GameObject::CreateChild(GetGameObject());
		m_KatanaRenderer = o->AddComponent<SpriteRenderer>();
		m_KatanaRenderer->SetRenderOrder(4);
		m_KatanaRenderer->SetSprite(m_sprKatana);
	}

	{
		auto texFrontFence = TextureManager::LoadFromFile(L"../Texture/main/spr_title_fence_0.png");
		m_sprFrontFence = new Sprite(texFrontFence);
		auto o = GameObject::CreateChild(GetGameObject());
		m_FrontFenceRenderer = o->AddComponent<SpriteRenderer>();
		m_FrontFenceRenderer->SetRenderOrder(5);
		m_FrontFenceRenderer->SetSprite(m_sprFrontFence);
	}

	{
		ZERO_MEM(m_arrSprLoop);
		for (int i = 0; i <= 11; ++i)
		{
			wstring directory = L"../Texture/main/";
			wstring frontName = L"main_loop_";
			wstring num = std::to_wstring(i);
			wstring local = directory + frontName + num + L".png";
			auto texMiddleGrass = TextureManager::LoadFromFile(local);
			m_arrSprLoop[i] = new Sprite(texMiddleGrass);
		}
		auto o = GameObject::CreateChild(GetGameObject());
		o->GetTransform()->SetLocalPosition(Vec2::down() * 100);

		m_LoopRenderer = o->AddComponent<SpriteRenderer>();
		m_LoopRenderer->SetRenderOrder(6);
		m_LoopRenderer->SetSprite(m_arrSprLoop[0]);
	}

	{
		auto texPressSpace = TextureManager::LoadFromFile(L"../Texture/main/spr_press_space.png");
		m_sprPressSpace = new Sprite(texPressSpace);
		auto o = GameObject::CreateChild(GetGameObject());
		m_PressSpaceRenderer = o->AddComponent<SpriteRenderer>();
		m_PressSpaceRenderer->SetRenderOrder(7);
		m_PressSpaceRenderer->SetSprite(m_sprPressSpace);
		m_PressSpaceRenderer->SetColor(Color(1, 1, 1, 0));
	}

	{
		auto texBlack = TextureManager::LoadFromFile(L"../Texture/main/spr_main_black.png");
		m_sprBlack = new Sprite(texBlack);
		auto o = GameObject::CreateChild(GetGameObject());
		m_BlackRenderer = o->AddComponent<SpriteRenderer>();
		m_BlackRenderer->SetRenderOrder(8);
		m_BlackRenderer->SetSprite(m_sprBlack);
		m_BlackRenderer->SetColor(Color(1, 1, 1, 0));
	}
}

void MainSceneScreen::Start()
{
	AsyncResourceLoad();
}

void MainSceneScreen::Update()
{
	CCamera2D::GetInstance()->SetWidth(640);
	CCamera2D::GetInstance()->SetHeight(360);

	MoveToNextScene();
	PlayLoopAnimation();
	PlayNeonBlinkAnimation();
	PressSpaceNotification();
}

void MainSceneScreen::OnDestroy()
{
	SAFE_DELETE(m_sprBack);
	SAFE_DELETE(m_sprNeonFrame);
	SAFE_DELETE(m_sprNeonZER);
	SAFE_DELETE(m_sprNeonO);
	SAFE_DELETE(m_sprKatana);
	SAFE_DELETE(m_sprFrontFence);
	for (auto& spr : m_arrSprLoop)
		SAFE_DELETE(spr);
	SAFE_DELETE(m_sprPressSpace);
	SAFE_DELETE(m_sprBlack);

	if (m_resourceLoadThread.joinable())
		m_resourceLoadThread.join();
}

void MainSceneScreen::AsyncResourceLoad()
{
	if (!ResourceLoader::IsLoadComplete())
	{
		std::thread t([&]() { ResourceLoader::LoadManagedFiles(); });
		m_resourceLoadThread.swap(t);
	}
}

void MainSceneScreen::MoveToNextScene()
{
	if (!ResourceLoader::IsLoadComplete()) return;
	if (!m_sceneChanging && Input::GetKeyDown(Key::Space))
	{
		m_sceneChanging = true;
		m_sceneChangeCounter = SCENE_CHANGE_WAIT_TIME;
	}
	if (m_sceneChanging)
	{
		if (m_sceneChangeCounter > 0)
		{
			m_sceneChangeCounter -= Time::GetUnscaledDeltaTime();
			float blackAlpha = 1 - m_sceneChangeCounter / SCENE_CHANGE_WAIT_TIME;
			m_BlackRenderer->SetColor(Color(1, 1, 1, blackAlpha));
		}
		else
		{
			SceneManager::ChangeScene(new Stage0);
		}
	}
}

void MainSceneScreen::PlayLoopAnimation()
{
	constexpr float interval = 0.1f;
	float fIndex = fmodf(m_loopElapsed / interval, float(SPRITE_MIDDLEGRASS_SIZE));
	int index = int(fIndex);
	m_LoopRenderer->SetSprite(m_arrSprLoop[index]);
	m_loopElapsed += Time::GetUnscaledDeltaTime();
}

void MainSceneScreen::PlayNeonBlinkAnimation()
{
	float neon_o = sinf(m_blinkElapsed * 2 * PI * float(rand() % 100) * 0.01f);
	m_NeonORenderer->SetEnable(neon_o > -0.95f);
	m_blinkElapsed += Time::GetUnscaledDeltaTime();
}

void MainSceneScreen::PressSpaceNotification()
{
	if (!ResourceLoader::IsLoadComplete()) return;

	constexpr float waveformScale = 0.5f;
	float alpha = (sinf(m_pressSpaceElapsed * 2 * PI * waveformScale - PI * 0.5f) + 1) * 0.5f;
	m_PressSpaceRenderer->SetColor(Color(1, 1, 1, alpha));
	
	m_pressSpaceElapsed += Time::GetUnscaledDeltaTime();
}
