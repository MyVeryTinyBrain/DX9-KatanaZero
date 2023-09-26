#include "stdafx.h"
#include "HeadHunterAnimator.h"
#include "AnimationManager.h"
#include "Animation.h"
#include "SoundMgr.h"

void HeadHunterAnimator::Awake()
{
	Animator::Awake();

	SetAdjust(false);

	//m_renderer->GetTransform()->GetParent()->SetLocalPosition(Vec2::down() * 17);

	m_idle = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/idle.txt");
	m_hurtfly_begin = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/hurtfly_begin.txt");
	m_hurtfly_loop = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/hurtfly_loop.txt");
	m_hurtfly_land = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/hurtfly_land.txt");
	m_jump_ready = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/jump_ready.txt");
	m_jump_loop = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/jump_loop.txt");
	m_jump_land = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/jump_land.txt");
	m_wallgrab_begin = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/wallgrab_begin.txt");
	m_wallgrab_loop = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/wallgrab_loop.txt");
	m_wallJump = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/walljump.txt");
	m_wallJump_loop = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/walljump_loop.txt");
	m_roll = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/roll.txt");
	m_rifle_aim_begin = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/rifle_aim_begin.txt");
	m_rifle_aim_loop = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/rifle_aim_loop.txt");
	m_rifle_aim_end = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/rifle_aim_end.txt");
	m_granade_begin = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/granade_begin.txt");
	m_granade_shoot = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/granade_shoot.txt");
	m_granade_end = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/granade_end.txt");
	m_dash_ready = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/dash_ready.txt");
	m_dash_end = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/dash_end.txt");
	m_dash_fastready = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/dash_fastready.txt");
	m_die_land = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/die_land.txt");
	m_die_land_loop = AnimationManager::GetAnimation(L"../Animation/boss/headhunter/die_land_loop.txt");

	wstring localDirectory = L"../Texture/boss/headhunter/rifle_aim_stand/";
	wstring fileName = L"spr_headhunter_aim_rifle_";
	for (int i = 0; i < 19; ++i)
	{
		WCHAR buf[32]{};
		_itow_s(i, buf, 10);
		m_aim[i] = new Sprite(localDirectory + fileName + buf + L".png");
	}

	SetDefaultAnimation(m_idle);
}

void HeadHunterAnimator::Update()
{
	Animator::Update();

	auto current = GetCurrentAnimation();

	if (current == m_hurtfly_loop && m_ground)
	{
		if (!m_shouldDie) PlayAnimation(m_hurtfly_land);
		else PlayAnimation(m_die_land);
		CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntress_wallslam_01.wav", CSoundMgr::CHANNELID::BOSS_EFFECT_0);

	}
	if ((current == m_jump_loop || current == m_wallJump || current == m_wallJump_loop) && m_ground)
	{
		PlayAnimation(m_jump_land);
		CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntress_wallslam_01.wav", CSoundMgr::CHANNELID::BOSS_EFFECT_0);
	}
	if (current == m_granade_shoot && GetFrameIndex() >= 1 && m_readyToGranadeShoot)
	{
		m_readyToGranadeShoot = false;
		OnGranadeShootFrame();
	}
}

void HeadHunterAnimator::BeforeRender()
{
	Animator::BeforeRender();

	auto current = GetCurrentAnimation();

	if (current == m_rifle_aim_loop)
	{
		m_renderer->SetSprite(m_aim[m_aimIndex]);
	}
}

void HeadHunterAnimator::OnDestroy()
{
	for (auto& aim : m_aim)
		SAFE_DELETE(aim);
}

void HeadHunterAnimator::OnAnimationEnd(const Animation* current)
{
	if (current == m_hurtfly_begin)
	{
		PlayAnimation(m_hurtfly_loop);
	}
	if (current == m_hurtfly_land)
	{
		OnPlayEndHurtFlyLand();
		CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntress_vanish_01.wav", CSoundMgr::CHANNELID::BOSS_EFFECT_1);
	}
	if (current == m_jump_ready)
	{
		PlayAnimation(m_jump_loop);
		OnPlayJump();
		CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntress_jump_01.wav", CSoundMgr::CHANNELID::BOSS_EFFECT_0);
	}
	if (current == m_wallgrab_begin)
	{
		PlayAnimation(m_wallgrab_loop);
	}
	if (current == m_wallJump)
	{
		PlayAnimation(m_wallJump_loop);
	}
	if (current == m_rifle_aim_begin)
	{
		PlayAnimation(m_rifle_aim_loop);
		OnPlayEndRifleReady();
	}
	if (current == m_granade_begin)
	{
		PlayAnimation(m_granade_end);
		OnPlayEndGranadeReady();
	}
	if (current == m_granade_shoot)
	{
		PlayAnimation(m_granade_end);
		OnPlayEndGranadeShoot();
	}
	if (current == m_dash_ready || current == m_dash_fastready)
	{
		PlayDashImmediately();
	}
	if (current == m_roll)
	{
		OnPlayEndRoll();
	}
	if (current == m_die_land)
	{
		PlayAnimation(m_die_land_loop);
	}
}

void HeadHunterAnimator::OnAnimationChange(const Animation* current, Animation** next)
{
}

void HeadHunterAnimator::SetGround(bool ground)
{
	m_ground = ground;
}

void HeadHunterAnimator::PlayAimming()
{
	PlayAnimation(m_rifle_aim_begin);
	CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntressgun_prep_01.wav", CSoundMgr::CHANNELID::BOSS_EFFECT_0);
	m_aimIndex = 9;
}

void HeadHunterAnimator::PlayStopAimming()
{
	PlayAnimation(m_rifle_aim_end);
}

void HeadHunterAnimator::SetAimAngle(float angle)
{
	if (angle < -90) angle = -90;
	if (angle > +90) angle = +90;
	float percent = (angle + 90) / 180.0f;
	float fIndex = percent * 18.0f;
	m_aimIndex = size_t(fIndex);
}

void HeadHunterAnimator::PlayHurt()
{
	PlayAnimation(m_hurtfly_begin);
}

void HeadHunterAnimator::PlayRoll()
{
	PlayAnimation(m_roll);
}

void HeadHunterAnimator::PlayJump()
{
	PlayAnimation(m_jump_ready);
}

void HeadHunterAnimator::PlayJumpImmediately()
{
	PlayAnimation(m_jump_loop);
	OnPlayJump();
}

void HeadHunterAnimator::PlayWallGrab()
{
	auto current = GetCurrentAnimation();
	if (current == m_wallgrab_begin || current == m_wallgrab_loop) return;
	PlayAnimation(m_wallgrab_begin);
}

void HeadHunterAnimator::PlayWallJump()
{
	PlayAnimation(m_wallJump);
	CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntress_wallslam_01.wav", CSoundMgr::CHANNELID::BOSS_EFFECT_0, 0.75f);
}

void HeadHunterAnimator::PlayGranadeTakeout()
{
	PlayAnimation(m_granade_begin);
	CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntressgun_prep_01.wav", CSoundMgr::CHANNELID::BOSS_EFFECT_0);
}

void HeadHunterAnimator::PlayGranadeShoot()
{
	m_readyToGranadeShoot = true;
	PlayAnimation(m_granade_shoot);
	CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntressbomb_shot_01.wav", CSoundMgr::CHANNELID::BOSS_EFFECT_1, 0.6f);
}

void HeadHunterAnimator::PlayGranadePutback()
{
	PlayAnimation(m_granade_end);
}

void HeadHunterAnimator::PlayDash()
{
	PlayAnimation(m_dash_ready);
	CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntressknife_prep_01.wav", CSoundMgr::CHANNELID::BOSS_EFFECT_0);
}

void HeadHunterAnimator::PlayDashImmediately()
{
	PlayAnimation(m_dash_end);
	OnPlayEndDashReady();
	CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntress_dash_01.wav", CSoundMgr::CHANNELID::BOSS_EFFECT_0);
}

void HeadHunterAnimator::PlayFastDash()
{
	PlayAnimation(m_dash_fastready);
	CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntressknife_prep_01.wav", CSoundMgr::CHANNELID::BOSS_EFFECT_0);
}

void HeadHunterAnimator::SetShouldDie()
{
	m_shouldDie = true;
}

bool HeadHunterAnimator::IsPlayingIdle() const
{
	return GetCurrentAnimation() == m_idle;
}

bool HeadHunterAnimator::IsPlayingHurt() const
{
	auto cur = GetCurrentAnimation();
	return
		cur == m_hurtfly_begin ||
		cur == m_hurtfly_loop ||
		cur == m_hurtfly_land;
}

bool HeadHunterAnimator::IsPlayingHurtLand() const
{
	auto cur = GetCurrentAnimation();
	return cur == m_hurtfly_land;
}

bool HeadHunterAnimator::IsPlayingRoll() const
{
	auto cur = GetCurrentAnimation();
	return cur == m_roll;
}

bool HeadHunterAnimator::IsPlayingJumpReady() const
{
	auto cur = GetCurrentAnimation();
	return
		cur == m_jump_ready;
}

bool HeadHunterAnimator::IsPlayingJumpLoop() const
{
	auto cur = GetCurrentAnimation();
	return
		cur == m_jump_loop;
}

bool HeadHunterAnimator::IsPlayingWallGrab() const
{
	auto cur = GetCurrentAnimation();
	return
		cur == m_wallgrab_begin ||
		cur == m_wallgrab_loop;
}

bool HeadHunterAnimator::IsPlayingAimming() const
{
	auto cur = GetCurrentAnimation();
	return
		cur == m_rifle_aim_loop;
}

bool HeadHunterAnimator::IsPlayingDashReady() const
{
	auto cur = GetCurrentAnimation();
	return cur == m_dash_ready;
}

bool HeadHunterAnimator::IsPlayingFastDashReady() const
{
	auto cur = GetCurrentAnimation();
	return cur == m_dash_fastready;
}

bool HeadHunterAnimator::IsPlayingAnimationAboutJump() const
{
	auto cur = GetCurrentAnimation();
	return
		cur == m_jump_loop ||
		cur == m_wallJump ||
		cur == m_wallJump_loop;
}

bool HeadHunterAnimator::IsPlayingWallJump() const
{
	auto cur = GetCurrentAnimation();
	return
		cur == m_wallJump;
}

bool HeadHunterAnimator::IsPlayingAnimationAboutGranade() const
{
	auto cur = GetCurrentAnimation();
	return
		cur == m_granade_begin ||
		cur == m_granade_shoot ||
		cur == m_granade_end;
}

bool HeadHunterAnimator::IsPlayingAnimationAboutAimming() const
{
	auto cur = GetCurrentAnimation();
	return
		cur == m_rifle_aim_begin ||
		cur == m_rifle_aim_loop ||
		cur == m_rifle_aim_end;
}

bool HeadHunterAnimator::IsPlayingJumpLand() const
{
	return GetCurrentAnimation() == m_jump_land;
}

bool HeadHunterAnimator::IsPlayingDie() const
{
	if (m_shouldDie)
	{
		return
			GetCurrentAnimation() == m_hurtfly_begin ||
			GetCurrentAnimation() == m_hurtfly_loop ||
			GetCurrentAnimation() == m_hurtfly_land ||
			GetCurrentAnimation() == m_die_land ||
			GetCurrentAnimation() == m_die_land_loop;
	}
	else
	{
		return
			GetCurrentAnimation() == m_die_land ||
			GetCurrentAnimation() == m_die_land_loop;
	}
}

float HeadHunterAnimator::GetWallJumpPercent() const
{
	if (!IsPlayingWallJump()) return 0;
	if (GetFrameIndex() <= 4) return 0;
	if (GetFrameIndex() >= 10) return 0;

	float frameCount = float(m_wallJump->GetSpriteCount());
	float aniMaxTime = m_wallJump->GetMaximumTime();
	float frameInterval = m_wallJump->GetInterval();

	constexpr float begFrame = 4;
	constexpr float endFrame = 10;
	float begTime = begFrame * frameInterval;
	float endTime = endFrame * frameInterval;
	float validTime = endTime - begTime;

	float elapsed = GetElapsed();
	float localTime = elapsed - begTime;
	float percent = localTime / validTime;

	if (percent > 1.0f) percent = 1.0f;

	return percent;
}
