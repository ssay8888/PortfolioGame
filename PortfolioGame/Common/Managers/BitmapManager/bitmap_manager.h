#pragma once
#include <Windows.h>
#include <map>
class MyBitmap;
class BitmapManager
{
	BitmapManager();
	~BitmapManager();

public:
	static BitmapManager* GetInstance()
	{
		static BitmapManager instance;
		return &instance;
	}

	HDC FindImage(const TCHAR* pImageKey);
	void Insert_Bitmap(HWND hwnd, const TCHAR* pPath, const TCHAR* pImageKey);
	void Release_Bitmap();

private:

	std::map<const TCHAR*, MyBitmap* > _mapBmp;
};

