#include "editor_stdafx.h"
#include "EditorManager.h"
#include "Background.h"
#include "CameraController.h"
#include "Parallax.h"
#include "EditorCollider.h"
#include "EditorObject.h"
#include "CameraBorderBox.h"
#include "TriggerBox.h"

#include "Serializer.h"

EditorManager* EditorManager::g_instance = nullptr;

void EditorManager::Awake()
{
	g_instance = this;
	m_focus = true;

	New();
}

void EditorManager::OnDestroy()
{
	g_instance = nullptr;
}

EditorManager* EditorManager::GetInstance()
{
	return g_instance;
}

void EditorManager::SetFocus(bool focus)
{
	m_focus = focus;
}

bool EditorManager::IsFocus() const
{
	return m_focus;
}

void EditorManager::SaveAs(const wstring& path)
{
	Serializer::Serialize(path);
	m_currentPath = path;
}

void EditorManager::Open(const wstring& path)
{
	bool success = Serializer::Deserialize(path);
	if (success)
		m_currentPath = path;
}

const wstring& EditorManager::GetCurrentPath() const
{
	return m_currentPath;
}

void EditorManager::New()
{
	// camera controller
	if (!m_camController)
	{
		auto camCtrObj = new GameObject;
		m_camController = camCtrObj->AddComponent<CameraController>();
	}
	m_camController->Reset();

	// background
	for (int i = 0; i < 3; ++i)
	{
		if (!m_background[i])
		{
			auto bgObj = new GameObject;
			m_background[i] = bgObj->AddComponent<Background>();
		}
		m_background[i]->SetSprite(L"../Texture/");
		m_background[i]->SetEmptyColor(Color(0, 0, 0, 1));
		m_background[i]->GetTransform()->SetPosition(Vec2::zero());
	}
	m_background[0]->SetOrder(RD_BACKGROUND_FRONT);
	m_background[1]->SetOrder(RD_BACKGROUND_MIDDLE);
	m_background[2]->SetOrder(RD_BACKGROUND_BACK);

	// parallax
	for (int i = 0; i < 3; ++i)
	{
		if (!m_parallax[i])
		{
			auto pllxObj = new GameObject;
			m_parallax[i] = pllxObj->AddComponent<Parallax>();
		}
		m_parallax[i]->SetSprite(L"../Texture/");
		m_parallax[i]->SetBase(Vec2::zero());
		m_parallax[i]->SetParallaxScale(Vec2::zero());
		m_parallax[i]->SetScale(1.0f);
	}
	m_parallax[0]->SetOrder(RD_PARALLAX_FRONT);
	m_parallax[1]->SetOrder(RD_PARALLAX_MIDDLE);
	m_parallax[2]->SetOrder(RD_PARALLAX_BACK);

	if (!m_cameraBorderBox)
	{
		auto o = new GameObject;
		m_cameraBorderBox = o->AddComponent<CameraBorderBox>();
	}
	m_cameraBorderBox->Reset();

	for (auto& trigger : TriggerBox::triggers)
		trigger->GetGameObject()->Destroy();
	TriggerBox::triggers.clear();

	for (auto& collider : EditorCollider::colliders)
		collider->GetGameObject()->Destroy();
	EditorCollider::colliders.clear();

	for (auto& object : EditorObject::objects)
		object->GetGameObject()->Destroy();
	EditorObject::objects.clear();

	m_currentPath = L"";
}

Background* EditorManager::GetFrontBackground() const
{
	return m_background[0];
}

Background* EditorManager::GetMiddleBackground() const
{
	return m_background[1];
}

Background* EditorManager::GetBackBackground() const
{
	return m_background[2];
}

Parallax* EditorManager::GetFrontParallax() const
{
	return m_parallax[0];
}

Parallax* EditorManager::GetMiddleParallax() const
{
	return m_parallax[1];
}

Parallax* EditorManager::GetBackParallax() const
{
	return m_parallax[2];
}

CameraBorderBox* EditorManager::GetCameraBorderBox() const
{
	return m_cameraBorderBox;
}
