#pragma once
#define KEY_LBUTTON 0x00000001 
#define KEY_RBUTTON 0x00000002
#define KEY_LEFT	0x00000004
#define KEY_RIGHT	0x00000008
#define KEY_UP		0x00000010
#define KEY_DOWN	0x00000020
#define KEY_A		0x00000040
#define KEY_B		0x00000080
#define KEY_S		0x00000100
#define KEY_L		0x00000200
#define KEY_Z		0x00000400
#define KEY_CONTROL 0x00000800
#define KEY_X		0x00001000
#define KEY_C		0x00002000

class KeyManager
{
public:
	static KeyManager* GetInstance()
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

