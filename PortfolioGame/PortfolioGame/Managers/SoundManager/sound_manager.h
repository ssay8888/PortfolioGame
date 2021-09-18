#pragma once
class SoundManager
{
public:
	static SoundManager* GetInstance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new SoundManager;

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
	enum CHANNELID {kBgm, kPlayer, kMonster, kMonsterHit, kSkillUse, kSkillHit, kLevelUp, kPortal, kEffect, kUi, kEnchant, kMaxChannel};
private:
	SoundManager();
	~SoundManager();

public:
	void Initialize(); 

	void Release(); 
public:
	void PlaySound(const TCHAR* pSoundKey, CHANNELID eID);
	void PlayBGM(const TCHAR* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();

private:
	void LoadSoundFile(); 

private:
	static SoundManager* m_pInstance;
	// 사운드 리소스 정보를 갖는 객체 
	std::map<TCHAR*, FMOD_SOUND*> m_mapSound; 
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[kMaxChannel]; 
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem; 
	FMOD_CHANNELGROUP* groupA;
};

