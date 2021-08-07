#pragma once
class Player;
class MainGame
{
public:
	MainGame(HDC hdc);
	~MainGame();
	void ReadeyGame();
	void UpdateGame();
	void LateUpdateGame();
	void RenderGame();
	void ReleaseGame();
	void SetPlayer(Player*);
	Player* GetPlayer() const;

private:
	HDC _hdc;
	HDC _hdc_buffer;
	HBITMAP _bitmap;
	HBITMAP _old_bitmap;

	uint64_t _ticksCount;
	Player* _player;


	int _iFPS;
	ULONGLONG _dwFPSTime;
	TCHAR _szFPS[32];
};

