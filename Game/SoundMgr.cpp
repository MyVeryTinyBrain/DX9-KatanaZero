#include "stdafx.h"
#include "SoundMgr.h"

CSoundMgr* CSoundMgr::m_pInstance = nullptr;
CSoundMgr::CSoundMgr()
{
	m_pSystem = nullptr; 
}


CSoundMgr::~CSoundMgr()
{
	Release(); 
}

void CSoundMgr::Initialize()
{
	FMOD_System_Create(&m_pSystem);
	
	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	LoadSoundFile(); 

	m_currentBGM = L"";
}

void CSoundMgr::Release()
{
	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear(); 
	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}

void CSoundMgr::SetSoundVolume(CHANNELID eID, float volume)
{
	FMOD_Channel_SetVolume(m_pChannelArr[eID], volume);
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::SetBGMVolume(float volume)
{
	FMOD_Channel_SetVolume(m_pChannelArr[BGM], volume);
	FMOD_System_Update(m_pSystem);
}

float CSoundMgr::GetSoumdVolume(CHANNELID eID) const
{
	float volume;
	FMOD_Channel_GetVolume(m_pChannelArr[eID], &volume);
	return volume;
}

float CSoundMgr::GetBGMVolume() const
{
	float volume;
	FMOD_Channel_GetVolume(m_pChannelArr[BGM], &volume);
	return volume;
}

void CSoundMgr::SetPitch(float pitch)
{
	FMOD_CHANNELGROUP *pGroup;
	FMOD_System_GetMasterChannelGroup(m_pSystem, &pGroup);
	FMOD_ChannelGroup_SetPitch(pGroup, pitch);
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::PlaySound(CONST WCHAR * pSoundKey, CHANNELID eID, float volume)
{
	StopSound(eID);
	PlaySoundContiue(pSoundKey, eID, volume);
}

void CSoundMgr::PlaySoundContiue(CONST WCHAR* pSoundKey, CHANNELID eID, float volume)
{
	std::map<WCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;
	if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[eID]);
	}
	FMOD_Channel_SetVolume(m_pChannelArr[eID], volume);
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::PlayBGM(CONST WCHAR * pSoundKey, float volume)
{	
	if (pSoundKey == m_currentBGM)
	{
		SetBGMVolume(volume);
		return;
	}
	else
	{
		StopBGM();
	}

	if (m_currentBGM == pSoundKey) return;

	std::map<WCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[BGM], volume);
	FMOD_System_Update(m_pSystem);

	m_currentBGM = pSoundKey;
}

void CSoundMgr::StopSound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSoundMgr::StopBGM()
{
	StopSound(BGM);
	m_currentBGM = L"";
}

void CSoundMgr::StopAll()
{
	for (int i = 0 ; i < MAXCHANNEL ; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSoundMgr::LoadSoundFile()
{
	_finddata_t fd; 

	long handle = _findfirst("../Sound/*.*", &fd);

	if (handle == 0)
		return; 

	int iResult = 0; 

	char szCurPath[128] = "../Sound/";
	char szFullPath[128] = ""; 

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath); 
		strcat_s(szFullPath, fd.name);
		FMOD_SOUND* pSound = nullptr; 

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = strlen(fd.name) + 1; 

			WCHAR* pSoundKey = new WCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(WCHAR) * iLength);
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			m_mapSound.emplace(pSoundKey, pSound);
		}
		iResult = _findnext(handle, &fd);
	}
	FMOD_System_Update(m_pSystem);
	_findclose(handle);
}
