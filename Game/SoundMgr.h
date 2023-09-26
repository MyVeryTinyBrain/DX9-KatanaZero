#pragma once

class CSoundMgr
{
public:
	static CSoundMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSoundMgr; 

		return m_pInstance; 
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance; 
			m_pInstance = nullptr; 
		}
	}
public:
	enum CHANNELID 
	{
		BGM, 
		SLOWMOTION,
		PLAYER, 
		PLAYER_EFFECT, 
		PLAYER_UI,
		PLAYER_WALLSLIDE,
		SWORDEFFECT,
		SWORDEFFECT_HIT,
		BULLET_HIT,
		BULLET_FIRE,
		ENEMY_MELEE,
		ENEMY_MELEE_HIT,
		TURRET,
		TURRET_EFFECT,
		EXPLOSION,
		THROW_OBJECT,
		BOSS_VOICE,
		BOSS_EFFECT_0,
		BOSS_EFFECT_1,
		BOSS_WEAPON_0,
		BOSS_WEAPON_1,
		BOSS_WEAPON_2,
		BOSS_PROJECTILE,
		ENVIRONMENT,
		RECORDER,
		MAXCHANNEL
	};
private:
	CSoundMgr();
	~CSoundMgr();

public:
	void Initialize(); 

	void Release(); 
public:
	void PlaySound(CONST WCHAR* pSoundKey, CHANNELID eID, float volume = 1.0f);
	void PlaySoundContiue(CONST WCHAR* pSoundKey, CHANNELID eID, float volume = 1.0f);
	void PlayBGM(CONST WCHAR* pSoundKey, float volume = 0.5f);

	void SetSoundVolume(CHANNELID eID, float volume);
	void SetBGMVolume(float volume);
	float GetSoumdVolume(CHANNELID eID) const;
	float GetBGMVolume() const;

	void SetPitch(float pitch);
	void StopSound(CHANNELID eID);
	void StopBGM();
	void StopAll();

private:
	void LoadSoundFile(); 

private:
	static CSoundMgr* m_pInstance; 
	// 사운드 리소스 정보를 갖는 객체 
	std::map<WCHAR*, FMOD_SOUND*> m_mapSound; 
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL]; 
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem; 
	// 현재 BGM
	wstring m_currentBGM;
	// Pitch 영향을 받지 않을 채널그룹
	//FMOD_CHANNELGROUP* m_unSlowmoChannelGroup;
};

