#include "stdafx.h"
#include "Door.h"
#include "AnimationManager.h"
#include "PlayerController.h"
#include "GameLayer.h"
#include "GameTag.h"
#include "DebugDraw.h"
#include "GameRenderOrder.h"
#include "SoundMgr.h"

void Door::Awake()
{
	GetGameObject()->SetTag(TAG_DOOR);
	m_renderer = AddComponent<SpriteRenderer>();
	m_renderer->SetRenderOrder(RENDER_ORDER_DOOR);
	m_body = AddComponent<Body>();
	m_body->SetType(BodyType::Static);
	m_body->SetLayerIndex(LAYERINDEX_DOOR);
	GameObject* colliderObject = new GameObject(GetTransform()->GetPosition() + Vec2::right() * 6, GetTransform());
	m_boxCollider = colliderObject->AddComponent<BoxCollider>();
	m_boxCollider->SetWidth(12);
	m_boxCollider->SetHeight(60);
	m_boxCollider->DrawDebug = DRAW_OBJECT;

	Animator::Awake();
	SetAdjust(false);

	m_type = Door::Type::DOOR;
	m_state = Door::State::CLOSED;
	SetType(Door::Type::DOOR);
	PlayAnimation(m_closed);
}

void Door::Update()
{
	Animator::Update();

	if (Input::GetKeyDown(Key::Return))
	{
		Close();
	}
}

void Door::OnAnimationEnd(const Animation* current)
{
	if (current == m_closing)
	{
		PlayAnimation(m_closed);
		m_state = Door::State::CLOSED;
	}
	if (current == m_opening)
	{
		PlayAnimation(m_opened);
		m_state = Door::State::OPENED;
	}
}

void Door::OnAnimationChange(const Animation* current, Animation** next)
{
}

void Door::SetType(Door::Type type)
{
	Door::Type prev = m_type;
	switch (type)
	{
		case Door::Type::CLUB:
			m_closing = AnimationManager::GetAnimation(L"../Animation/object/door/club_closing.txt");
			m_closed = AnimationManager::GetAnimation(L"../Animation/object/door/club_closed.txt");
			m_opening = AnimationManager::GetAnimation(L"../Animation/object/door/club_opening.txt");
			m_opened = AnimationManager::GetAnimation(L"../Animation/object/door/club_opened.txt");
			break;
		case Door::Type::DOOR:
			m_closing = AnimationManager::GetAnimation(L"../Animation/object/door/door_closing.txt");
			m_closed = AnimationManager::GetAnimation(L"../Animation/object/door/door_closed.txt");
			m_opening = AnimationManager::GetAnimation(L"../Animation/object/door/door_opening.txt");
			m_opened = AnimationManager::GetAnimation(L"../Animation/object/door/door_opened.txt");
			break;
		case Door::Type::IRON:
			m_closing = AnimationManager::GetAnimation(L"../Animation/object/door/iron_closing.txt");
			m_closed = AnimationManager::GetAnimation(L"../Animation/object/door/iron_closed.txt");
			m_opening = AnimationManager::GetAnimation(L"../Animation/object/door/iron_opening.txt");
			m_opened = AnimationManager::GetAnimation(L"../Animation/object/door/iron_opened.txt");
			break;
		case Door::Type::MANSION:
			m_closing = AnimationManager::GetAnimation(L"../Animation/object/door/mansion_closing.txt");
			m_closed = AnimationManager::GetAnimation(L"../Animation/object/door/mansion_closed.txt");
			m_opening = AnimationManager::GetAnimation(L"../Animation/object/door/mansion_opening.txt");
			m_opened = AnimationManager::GetAnimation(L"../Animation/object/door/mansion_opened.txt");
			break;
	}
	m_type = type;
	if (m_type != prev)
	{
		PlayAgain();
	}
}

void Door::Open()
{
	Open(GetTransform()->GetScale().x);
}

void Door::Open(float horizontalDirection)
{
	if (m_state != Door::State::CLOSED) return;
	PlayAnimation(m_opening);
	m_state = Door::State::OPENING;
	m_boxCollider->SetEnable(false);

	if (horizontalDirection < 0) GetTransform()->SetScale(Vec2(-1, 1));
	else GetTransform()->SetScale(Vec2(+1, 1));

	CSoundMgr::Get_Instance()->PlaySound(L"sound_dooropen.wav", CSoundMgr::CHANNELID::PLAYER_EFFECT);
}

void Door::Close()
{
	if (m_state != Door::State::OPENED) return;
	PlayAnimation(m_closing);
	m_state = Door::State::CLOSING;
	m_boxCollider->SetEnable(true);
}

bool Door::IsOpen() const
{
	return GetCurrentAnimation() == m_opening || GetCurrentAnimation() == m_opened;
}

BoxCollider* Door::GetCollider() const
{
	return m_boxCollider;
}

void Door::PlayAgain()
{
	switch (m_state)
	{
		case Door::State::CLOSING:
			PlayAnimation(m_closing);
			break;
		case Door::State::CLOSED:
			PlayAnimation(m_closed);
			break;
		case Door::State::OPENING:
			PlayAnimation(m_opening);
			break;
		case Door::State::OPENED:
			PlayAnimation(m_opened);
			break;
	}
}
