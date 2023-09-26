#include "stdafx.h"
#include "PlayerAnimator.h"
#include "Animation.h"
#include "AnimationManager.h"
#include "SoundMgr.h"

void PlayerAnimator::Awake()
{
	Animator::Awake();

	m_idle = AnimationManager::GetAnimation(L"../Animation/player/idle.txt");
	m_idle_to_run = AnimationManager::GetAnimation(L"../Animation/player/idle_to_run.txt");
	m_run = AnimationManager::GetAnimation(L"../Animation/player/run.txt");
	m_run_to_idle = AnimationManager::GetAnimation(L"../Animation/player/run_to_idle.txt");
	m_jump = AnimationManager::GetAnimation(L"../Animation/player/jump.txt");
	m_fall = AnimationManager::GetAnimation(L"../Animation/player/fall.txt");
	m_attack = AnimationManager::GetAnimation(L"../Animation/player/attack.txt");
	m_roll = AnimationManager::GetAnimation(L"../Animation/player/roll.txt");
	m_crouch_pre = AnimationManager::GetAnimation(L"../Animation/player/crouch_pre.txt");
	m_crouch = AnimationManager::GetAnimation(L"../Animation/player/crouch.txt");
	m_crouch_post = AnimationManager::GetAnimation(L"../Animation/player/crouch_post.txt");
	m_crouch_walk = AnimationManager::GetAnimation(L"../Animation/player/crouch_walk.txt");
	m_wallgrab = AnimationManager::GetAnimation(L"../Animation/player/wallgrab.txt");
	m_flip = AnimationManager::GetAnimation(L"../Animation/player/flip.txt");
	m_hurtfly_begin = AnimationManager::GetAnimation(L"../Animation/player/hurtfly_begin.txt");
	m_hurtfly_loop = AnimationManager::GetAnimation(L"../Animation/player/hurtfly_loop.txt");
	m_hurtground_begin = AnimationManager::GetAnimation(L"../Animation/player/hurtground_begin.txt");
	m_hurtground_loop = AnimationManager::GetAnimation(L"../Animation/player/hurtground_loop.txt");
	m_hurtground_cover = AnimationManager::GetAnimation(L"../Animation/player/hurtground_cover.txt");
	m_dooropen = AnimationManager::GetAnimation(L"../Animation/player/dooropen.txt");
	
	m_horizontalControl = false;
	m_hasGround = true;
	m_verticalVelocity = 0;

	PlayAnimation(m_idle);
	SetDefaultAnimation(m_idle);
}

void PlayerAnimator::Update()
{
	Animator::Update();
	const Animation* current = GetCurrentAnimation();
	const Animation* defaultAnimation = GetDefaultAnimation();

	if (m_hasGround)
	{
		if (current == m_hurtfly_loop)
		{
			PlayAnimation(m_hurtground_begin);
		}
	}

	if (IsPlayingAttack() || IsPlayingRoll() || IsPlayingFlip() || IsPlayingHurt() || IsPlayingDoorOpen()) return;

	if (m_hasGround)
	{
		if (!m_crouching) SetDefaultAnimation(m_idle);
		else SetDefaultAnimation(m_crouch);
	}
	else
	{
		if (m_wallGrabbing)
		{
			SetDefaultAnimation(m_wallgrab);
		}
		else
		{
			if (m_verticalVelocity > 0) SetDefaultAnimation(m_jump);
			else SetDefaultAnimation(m_fall);
		}
	}

	if (m_hasGround)
	{
		if (m_crouching)
		{
			if (current != m_crouch_pre &&
				current != m_crouch &&
				current != m_crouch_post &&
				current != m_crouch_walk)
			{
				PlayAnimation(m_crouch_pre);
			}
		}
		else
		{
			if (current == m_crouch_pre ||
				current == m_crouch ||
				current == m_crouch_post ||
				current == m_crouch_walk)
			{
				PlayAnimation(m_crouch_post);
			}
		}

		if (m_horizontalControl)
		{
			if (current == m_idle)
			{
				PlayAnimation(m_idle_to_run);
			}
			if (current == m_crouch)
			{
				PlayAnimation(m_crouch_walk);
			}
		}
		else
		{
			if (current == m_run || current == m_idle_to_run)
			{
				PlayAnimation(m_run_to_idle);
			}
			if (current == m_crouch_walk)
			{
				PlayAnimation(m_crouch_pre);
			}
		}
	}
}

void PlayerAnimator::OnDestroy()
{
}

void PlayerAnimator::OnAnimationEnd(const Animation* current)
{
	if (current == m_idle_to_run)
	{
		PlayAnimation(m_run);
	}
	if (current == m_hurtfly_begin)
	{
		PlayAnimation(m_hurtfly_loop);
	}
	if (current == m_hurtground_begin)
	{
		if (m_canWake)
		{
			PlayAnimation(m_hurtground_cover);
		}
		else
		{
			PlayAnimation(m_hurtground_loop);
			OnEndPlayHurtflyGround();
		}
	}
}

void PlayerAnimator::OnAnimationChange(const Animation* current, Animation** next)
{
}

void PlayerAnimator::SetHorizontalControl(bool enable)
{
	m_horizontalControl = enable;
}

void PlayerAnimator::SetGroundState(bool hasGround)
{
	m_hasGround = hasGround;
}

void PlayerAnimator::SetVerticalVelocity(float verticalVelocity)
{
	m_verticalVelocity = verticalVelocity;
}

void PlayerAnimator::PlayAttack()
{
	PlayAnimation(m_attack);
}

void PlayerAnimator::PlayRoll()
{
	PlayAnimation(m_roll);
}

void PlayerAnimator::SetCrouch(bool enable)
{
	m_crouching = enable;
}

void PlayerAnimator::PlayFlip()
{
	PlayAnimation(m_flip);
}

void PlayerAnimator::SetWallGrab(bool enable)
{
	m_wallGrabbing = enable;
}

void PlayerAnimator::PlayHurt(bool canWake)
{
	m_canWake = canWake;
	PlayAnimation(m_hurtfly_begin);

	if (!canWake)
		CSoundMgr::Get_Instance()->PlaySound(L"sound_playerdie.wav", CSoundMgr::CHANNELID::PLAYER);
}

void PlayerAnimator::PlayHurtCover()
{
	PlayAnimation(m_hurtground_cover);
}

void PlayerAnimator::PlayDoorOpen()
{
	PlayAnimation(m_dooropen);
}

bool PlayerAnimator::IsPlayingHorizontalMoveOnGround() const
{
	bool result =
		GetCurrentAnimation() == m_idle_to_run||
		GetCurrentAnimation() == m_run ||
		GetCurrentAnimation() == m_crouch_walk;
	return result;
}

bool PlayerAnimator::IsPlayingToIdle() const
{
	bool result =
		GetCurrentAnimation() == m_run_to_idle;
	return result;
}

bool PlayerAnimator::IsPlayingToRun() const
{
	bool result =
		GetCurrentAnimation() == m_idle_to_run;
	return result;
}

bool PlayerAnimator::IsPlayingAttack() const
{
	bool result =
		GetCurrentAnimation() == m_attack;
	return result;
}

bool PlayerAnimator::IsPlayingRoll() const
{
	bool result =
		GetCurrentAnimation() == m_roll;
	return result;
}

bool PlayerAnimator::IsPlayingWallGrab() const
{
	bool result =
		GetCurrentAnimation() == m_wallgrab;
	return result;
}

bool PlayerAnimator::IsPlayingFlip() const
{
	bool result =
		GetCurrentAnimation() == m_flip;
	return result;
}

bool PlayerAnimator::IsPlayingHurt() const
{
	bool result =
		GetCurrentAnimation() == m_hurtfly_begin		||
		GetCurrentAnimation() == m_hurtfly_loop			||
		GetCurrentAnimation() == m_hurtground_begin		||
		GetCurrentAnimation() == m_hurtground_loop		||
		GetCurrentAnimation() == m_hurtground_cover;
	return result;
}

bool PlayerAnimator::IsPlayingHurtFly() const
{
	bool result =
		GetCurrentAnimation() == m_hurtfly_begin ||
		GetCurrentAnimation() == m_hurtfly_loop;
	return result;
}

bool PlayerAnimator::IsPlayingHurtGroundLoop() const
{
	bool result =
		GetCurrentAnimation() == m_hurtground_loop;
	return result;
}

bool PlayerAnimator::IsPlayingDoorOpen() const
{
	bool result =
		GetCurrentAnimation() == m_dooropen;
	return result;
}

bool PlayerAnimator::CanMove() const
{
	bool result =
		GetCurrentAnimation() == m_idle ||
		GetCurrentAnimation() == m_idle_to_run ||
		GetCurrentAnimation() == m_run ||
		GetCurrentAnimation() == m_run_to_idle ||
		GetCurrentAnimation() == m_jump ||
		GetCurrentAnimation() == m_fall ||
		GetCurrentAnimation() == m_crouch_pre ||
		GetCurrentAnimation() == m_crouch ||
		GetCurrentAnimation() == m_crouch_walk ||
		GetCurrentAnimation() == m_wallgrab ||
		GetCurrentAnimation() == m_flip;
	return result;
}

bool PlayerAnimator::CanAttack() const
{
	bool result =
		GetCurrentAnimation() == m_idle ||
		GetCurrentAnimation() == m_idle_to_run ||
		GetCurrentAnimation() == m_run ||
		GetCurrentAnimation() == m_run_to_idle ||
		GetCurrentAnimation() == m_jump ||
		GetCurrentAnimation() == m_fall ||
		GetCurrentAnimation() == m_crouch_pre ||
		GetCurrentAnimation() == m_crouch ||
		GetCurrentAnimation() == m_crouch_walk ||
		GetCurrentAnimation() == m_wallgrab ||
		GetCurrentAnimation() == m_flip ||
		GetCurrentAnimation() == m_roll;
	return result;
}

bool PlayerAnimator::GetGroundState() const
{
	return m_hasGround;
}

bool PlayerAnimator::GetHorizontaControlState() const
{
	return m_horizontalControl;
}

bool PlayerAnimator::IsCrouch() const
{
	auto current = GetCurrentAnimation();
	bool result =
		current == m_crouch_pre ||
		current == m_crouch ||
		current == m_crouch_post ||
		current == m_crouch_walk;
	return result;
}
