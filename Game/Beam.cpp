#include "stdafx.h"
#include "Beam.h"
#include "GameLayer.h"
#include "GameTag.h"
#include "DebugDraw.h"
#include "PlayerController.h"
#include "GameRenderOrder.h"
#include "PlayerCamera.h"
#include "TimeController.h"
#include "PlayerCamera.h"
#include "Recorder.h"
#include "SoundMgr.h"
#include "StaticEffect.h"

void Beam::Awake()
{
	m_renderOrder = RENDER_ORDER_EFFECT_FRONT;
	m_sprCircle = new Sprite(L"../Texture/effect/used/circle/spr_circle.png");

	m_body = AddComponent<Body>();
	m_body->SetType(BodyType::Dynamic);
	m_body->SetGravityMode(false);
	m_body->SetLayerIndex(LAYERINDEX_TRIGGER);

	auto triggerObject = GameObject::CreateChild(GetGameObject());
	m_trigger = triggerObject->AddComponent<BoxCollider>();
	m_trigger->SetTriggerMode(true);
	m_trigger->DrawDebug = DRAW_TRIGGER;
	m_trigger->OnTriggerEnter += Function<void, Collider*>(this, &Beam::OnTriggerEnter);

	m_width = 13;
	m_length = 1000;
	m_invPercent = 1.0f;
	FitTriggerTransform();

	m_colors[0] = Color(1, 1, 1, 1);
	m_colors[1] = D3DCOLOR_ARGB(255, 255, 171, 64);

	m_duration = 1.0f;
	m_elapsed = 0;
	m_waveform = 0.025f;

	PlayerCamera::GetInstance()->Shake(Vec2::Direction(float(rand() % 360)), 0.1f, 150, 10);

	m_soundType = SoundType::SHORT;
}

void Beam::Start()
{
	switch (m_soundType)
	{
		case SoundType::NOPLAY:
			break;
		case SoundType::SHORT:
			{
				int soundCase = rand() % 3;
				switch (soundCase)
				{
					case 0:
						CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntresslaser_vertical_01.wav", CSoundMgr::CHANNELID::BOSS_WEAPON_1);
						break;
					case 1:
						CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntresslaser_vertical_02.wav", CSoundMgr::CHANNELID::BOSS_WEAPON_1);
						break;
					case 2:
						CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntresslaser_vertical_03.wav", CSoundMgr::CHANNELID::BOSS_WEAPON_1);
						break;
				}
			}
			break;
		case SoundType::NORMAL:
			{
				int soundCase = rand() % 3;
				switch (soundCase)
				{
					case 0:
						CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntresslaser_shot_01.wav", CSoundMgr::CHANNELID::BOSS_WEAPON_1);
						break;
					case 1:
						CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntresslaser_shot_02.wav", CSoundMgr::CHANNELID::BOSS_WEAPON_1);
						break;
					case 2:
						CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntresslaser_shot_03.wav", CSoundMgr::CHANNELID::BOSS_WEAPON_1);
						break;
				}
			}
			break;
	}
}

void Beam::FixedUpdate()
{
	FitTriggerTransform();
}

void Beam::Update()
{
	float percent = m_elapsed / m_duration;
	float newPercent = 1.0f - powf(percent, 8.0f);
	m_invPercent = newPercent;

	m_elapsed += Time::GetDeltaTime();
	if (m_elapsed >= m_duration)
	{
		GetGameObject()->Destroy();
	}
}

void Beam::Render()
{
	if (Time::GetTimeScale() <= 0) return;

	Vec2 right = GetTransform()->GetRight();
	Vec2 beg = GetTransform()->GetPosition();
	Vec2 end = beg + right * m_length;

	float changeTime = m_waveform;
	float colorSelect = fmod(m_elapsed, changeTime * 2.0f) / changeTime;
	int colorSelectIndex = int(colorSelect);

	auto line = GraphicDevice::GetLineDevice();
	Draw2DLineParameters param;
	param.inWorld = true;
	param.worldScaleToWidth = true;
	param.width = m_invPercent * m_width;
	param.color = m_colors[colorSelectIndex];
	line->Draw2DLine(beg, end, &param);

	WorldLineRecordInfo lineRecord;
	lineRecord.start = beg;
	lineRecord.end = end;
	lineRecord.width = param.width;
	lineRecord.color = param.color;
	Recorder::GetInstance()->RecordLine(lineRecord);

	auto sprDev = GraphicDevice::GetSpriteDevice();
	const Mat4& worldToCamera = CCamera2D::GetInstance()->GetWorldToCameraMatrix();
	const Mat4& cameraToScreen = Screen::GetCameraToScreenMatrix();
	float circleScale = m_invPercent * 1.0f / m_sprCircle->GetSize().x * m_width;
	Mat4 beg_m = Mat4::SRT(Vec3::one() * circleScale, Vec3::zero(), VEC2_WORLD_TO_WINDOW(beg));
	Mat4 end_m = Mat4::SRT(Vec3::one() * circleScale, Vec3::zero(), VEC2_WORLD_TO_WINDOW(end));
	sprDev->DrawSprite(m_sprCircle, beg_m * worldToCamera * cameraToScreen, m_colors[colorSelectIndex]);
	sprDev->DrawSprite(m_sprCircle, end_m * worldToCamera * cameraToScreen, m_colors[colorSelectIndex]);

	SpriteRecordInfo spriteRecord0;
	spriteRecord0.position = beg;
	spriteRecord0.angle = 0;
	spriteRecord0.scale = Vec2::one() * circleScale;
	spriteRecord0.color = m_colors[colorSelectIndex];
	spriteRecord0.order = m_renderOrder;
	spriteRecord0.key = m_sprCircle->GetTexture()->GetKey();
	SpriteRecordInfo spriteRecord1 = spriteRecord0;
	spriteRecord0.position = end;
	Recorder::GetInstance()->RecordSprite(spriteRecord0);
	Recorder::GetInstance()->RecordSprite(spriteRecord1);
}

void Beam::OnDestroy()
{
	SAFE_DELETE(m_sprCircle);
}

void Beam::OnTriggerEnter(Collider* collider)
{
	Body* body = collider->GetBody();
	if (body->GetLayerIndex() != LAYERINDEX_CHARACTER) return;
	if (body->GetGameObject()->GetTag() != TAG_PLAYER) return;

	PlayerController* player = body->GetComponent<PlayerController>();
	if (!player) return;
	if (player->IsRolling()) return;

	Vec2 right = GetTransform()->GetRight();
	float hd = right.x < 0 ? -1.0f : +1.0f;
	Vec2 dir = Vec2(hd, 1.0f).normalized();

	player->Damage(dir * 300, this);

	TimeController::GetInstance()->SetSubScale(0.05f, 0.2f);
	PlayerCamera::GetInstance()->Shake(GetTransform()->GetRight(), 0.2f, 600, 10);

	m_body->SetEnable(false);

	float rangeRadius = 100;
	for (int i = 0; i < 20; ++i)
	{
		float angle = float(rand() % 360);
		float dist = fmodf(float(rand()), rangeRadius) + 10;
		auto effect = StaticEffect::Create(
			L"../Animation/effect/explosion.txt",
			body->GetTransform()->GetPosition() + Vec2::Direction(angle) * dist, true);
		effect->SetWait(float(rand() % 10) * 0.015f);
	}
}

void Beam::SetDuration(float duration)
{
	m_duration = duration;
}

void Beam::SetWidth(float width)
{
	m_width = width;
}

void Beam::SetLength(float length)
{
	m_length = length;
}

void Beam::FitTriggerTransform()
{
	Vec2 right = GetTransform()->GetRight();
	Vec2 beg = GetTransform()->GetPosition();
	Vec2 end = beg + right * m_length;
	Vec2 pos = (beg + end) * 0.5f;
	m_trigger->GetTransform()->SetPosition(pos);
	m_trigger->SetWidth(m_length);

	float widthPercent = m_invPercent;
	if (widthPercent <= 0) widthPercent = 0.01f;
	m_trigger->SetHeight(m_width * m_invPercent);
	m_trigger->SetWidth(m_length);
}
