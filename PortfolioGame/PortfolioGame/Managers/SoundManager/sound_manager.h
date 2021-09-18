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
	// ���� ���ҽ� ������ ���� ��ü 
	std::map<TCHAR*, FMOD_SOUND*> m_mapSound; 
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[kMaxChannel]; 
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem; 
	FMOD_CHANNELGROUP* groupA;
};

