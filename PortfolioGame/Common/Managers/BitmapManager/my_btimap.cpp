#include "my_bitmap.h"

MyBitmap::MyBitmap() :
	_memDC(nullptr),
	_bitmap(nullptr),
	_oldBitmap(nullptr)
{
}

MyBitmap::~MyBitmap()
{
}

inline HDC MyBitmap::Get_MemDC()
{
	return _memDC;
}

void MyBitmap::Insert_Bitmap(HWND hwnd,const TCHAR* pFilePath)
{
	HDC hDC = GetDC(hwnd);

	_memDC = CreateCompatibleDC(hDC);

	ReleaseDC(hwnd, hDC);

	_bitmap = (HBITMAP)LoadImage(NULL, pFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	_oldBitmap = (HBITMAP)SelectObject(_memDC, _bitmap);

}

void MyBitmap::Release_Bitmap()
{
	SelectObject(_memDC, _oldBitmap);
	DeleteObject(_bitmap);
	DeleteDC(_memDC);
}
