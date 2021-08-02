#pragma once
#include <Windows.h>
class MyBitmap
{
public:
	MyBitmap();
	~MyBitmap();
public:
	HDC Get_MemDC();
	void Insert_Bitmap(HWND hwnd, const TCHAR* pFilePath);
	void Release_Bitmap();
private:
	HDC _memDC;
	HBITMAP _bitmap; 
	HBITMAP _oldBitmap;
};

