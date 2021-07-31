#include "../../pch.h"
#include "key_manager.h"

KeyManager::KeyManager()
	:m_dwKey(0)
	, m_dwKeyDown(0)
	, m_dwKeyUp(0)
{
	ReadyKeyManaer();
}


KeyManager::~KeyManager()
{
}

void KeyManager::ReadyKeyManaer()
{
}

void KeyManager::KeyUpdate()
	{
	m_dwKey = 0;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwKey |= KEY_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwKey |= KEY_RIGHT;
}

bool KeyManager::KeyUp(DWORD dwKey)
{
	//auto buffstat = (1 << (31 - (dwKey % 32)));
	if (m_dwKey & dwKey)
	{
		m_dwKeyUp |= dwKey;
		return false;
	}
	else if (m_dwKeyUp & dwKey)
	{
		m_dwKeyUp ^= dwKey;
		return true;
	}
	return false;
}

bool KeyManager::KeyDown(DWORD dwKey)
{
	if (m_dwKey & dwKey && !(m_dwKeyDown & dwKey))
	{
		m_dwKeyDown |= dwKey;
		return true;
	}
	else if (!(m_dwKey & dwKey) && (m_dwKeyDown & dwKey))
	{
		m_dwKeyDown ^= dwKey;
		return false;
	}
	return false;
}

bool KeyManager::KeyPressing(DWORD dwKey)
{
	if (m_dwKey & dwKey)
		return true;
	return false;
}
