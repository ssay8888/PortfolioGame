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
	if (!_isWindowsActive) {
		return;
	}
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwKey |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwKey |= KEY_RBUTTON;
	if (GetAsyncKeyState('S') & 0x8000)
		m_dwKey |= KEY_S;
	if (GetAsyncKeyState('L') & 0x8000)
		m_dwKey |= KEY_L;
	if (GetAsyncKeyState('Z') & 0x8000)
		m_dwKey |= KEY_Z;
	if (GetAsyncKeyState('C') & 0x8000)
		m_dwKey |= KEY_C;
	if (GetAsyncKeyState('X') & 0x8000)
		m_dwKey |= KEY_X;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwKey |= KEY_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwKey |= KEY_RIGHT;
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwKey |= KEY_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwKey |= KEY_DOWN;
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		m_dwKey |= KEY_CONTROL;
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
