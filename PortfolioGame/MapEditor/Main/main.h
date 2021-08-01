#pragma once
class Mouse;
class Main
{
public:
	Main(HDC hdc);
	~Main();
public:
	void Ready_Edit();
	void Update_Edit();
	void Render_Edit();
	void Release_Edit();


private:
	HDC _hdc;
	HDC _hdc_buffer;
	HBITMAP _hBitmap;
	HBITMAP _oldBitmap;
};

