#include "my_bitmap.h"

MyBitmap::MyBitmap() :
	_memDC(nullptr),
	_bitmap(nullptr),
	_old_bitmap(nullptr),
	_width(0),
	_height(0)
{
}

MyBitmap::~MyBitmap()
{
}

HDC MyBitmap::GetMemDC()
{
	return _memDC;
}

void MyBitmap::RenderBitmapImage(HDC hdc, int left, int top, int right, int bottom)
{
	RenderBitmapImage(hdc, left, top, right, bottom, _width, _height);
}

void MyBitmap::RenderBitmapImage(HDC hdc, int left, int top, int right, int bottom, int width, int height)
{
	RenderBitmapImage(hdc, left, top, right, bottom, _width, _height, RGB(255, 0, 255));
}

void MyBitmap::RenderBitmapImage(HDC hdc, int left, int top, int right, int bottom, int width, int height, UINT rgb)
{
	GdiTransparentBlt(hdc,
		left, top, right, bottom,
		_memDC,
		0, 0,
		width,
		height,
		rgb);
}

void MyBitmap::Insert_Bitmap(HWND hwnd,const TCHAR* pFilePath)
{
	HDC hDC = GetDC(hwnd);

	_memDC = CreateCompatibleDC(hDC);

	ReleaseDC(hwnd, hDC);

	_bitmap = (HBITMAP)LoadImage(NULL, pFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);


	_old_bitmap = (HBITMAP)SelectObject(_memDC, _bitmap);
	BITMAP bmp;
	GetObject(_bitmap, sizeof(BITMAP), &bmp);
	_width = bmp.bmWidth;
	_height = bmp.bmHeight;

}

void MyBitmap::Release_Bitmap()
{
	SelectObject(_memDC, _old_bitmap);
	DeleteObject(_bitmap);
	DeleteDC(_memDC);
}

int MyBitmap::GetWidth()
{
	return _width;
}

int MyBitmap::GetHeight()
{
	return _height;
}
