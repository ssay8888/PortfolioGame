#pragma once
class Player;
class MainGame
{
public:
	MainGame(HDC hdc);
	~MainGame();
	void ReadeyGame();
	void UpdateGame();
	void RenderGame();
	void ReleaseGame();
	void SetPlayer(Player*);
	Player* GetPlayer() const;

private:
	HDC _hdc;
	HDC _hdc_buffer;
	HBITMAP _hBitmap;
	HBITMAP _oldBitmap;

	uint64_t _ticksCount;
	Player* _player;
};

