#pragma once
#define KEY_LBUTTON 0x00000001 
#define KEY_RBUTTON 0x00000002
#define KEY_LEFT	0x00000004
#define KEY_RIGHT	0x00000008
#define KEY_UP		0x00000010
#define KEY_DOWN	0x00000020

class KeyManager
{
public:
	static KeyManager* Get_Instance()
	{
		static KeyManager instance;
		return &instance;
	}
private:
	KeyManager();
	~KeyManager();
private:
	void ReadyKeyManaer();
public:
	void KeyUpdate();
	bool KeyUp(DWORD dwKey);
	bool KeyDown(DWORD dwKey);
	bool KeyPressing(DWORD dwKey);

private:
	DWORD m_dwKey;
	DWORD m_dwKeyDown;
	DWORD m_dwKeyUp;
};

