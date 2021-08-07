#pragma once
#include <Windows.h>
class MyBitmap
{
public:
	MyBitmap();
	~MyBitmap();
public:
	HDC GetMemDC();
	void RenderBitmapImage(HDC hdc, int left, int top, int right, int bottom);
	void RenderBitmapImage(HDC hdc, int left, int top, int right, int bottom, int width, int height);
	void Insert_Bitmap(HWND hwnd, const TCHAR* pFilePath);
	void Release_Bitmap();

	int GetWidth();
	int GetHeight();
private:
	HDC _memDC;
	HBITMAP _bitmap; 
	HBITMAP _old_bitmap;
	int _width;
	int _height;
};

