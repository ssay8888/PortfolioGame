#include "../../pch.h"
#include "sound_manager.h"

SoundManager* SoundManager::m_pInstance = nullptr;
SoundManager::SoundManager(): m_pChannelArr{},
                              groupA(nullptr)
{
	m_pSystem = nullptr;
}


SoundManager::~SoundManager()
{
	Release(); 
}

void SoundManager::Initialize()
{
	FMOD_System_Create(&m_pSystem);
	
	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);
	FMOD_System_CreateChannelGroup(m_pSystem, "group1", &groupA);
	LoadSoundFile(); 
}

void SoundManager::Release()
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




void SoundManager::PlaySound(const TCHAR * pSoundKey, CHANNELID eID)
{
	std::map<TCHAR*, FMOD_SOUND*>::iterator iter; 

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
	FMOD_System_Update(m_pSystem);
}

void SoundManager::PlayBGM(const TCHAR * pSoundKey)
{
	std::map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[kBgm]);
	FMOD_Channel_SetMode(m_pChannelArr[kBgm], FMOD_LOOP_NORMAL);
	FMOD_System_Update(m_pSystem);
}

void SoundManager::StopSound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void SoundManager::StopAll()
{
	for (int i = 0 ; i < kMaxChannel ; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void SoundManager::LoadSoundFile()
{
	_finddata_t fd; 

	intptr_t handle = _findfirst("Client/Sound/*.*", &fd);

	if (handle == 0)
		return; 

	int iResult = 0; 

	char szCurPath[128] = "Client/Sound/";
	char szFullPath[128] = ""; 

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath); 
		strcat_s(szFullPath, fd.name);
		FMOD_SOUND* pSound = nullptr; 

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_LOOP_OFF, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			size_t iLength = strlen(fd.name) + 1; 

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);
			MultiByteToWideChar(CP_ACP, 0, fd.name, static_cast<int>(iLength), pSoundKey, static_cast<int>(iLength));

			m_mapSound.emplace(pSoundKey, pSound);
		}
		iResult = _findnext(handle, &fd);
	}
	FMOD_System_Update(m_pSystem);
	_findclose(handle);
}
