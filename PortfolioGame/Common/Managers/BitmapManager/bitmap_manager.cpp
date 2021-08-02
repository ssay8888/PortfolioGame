#include "bitmap_manager.h"
#include "my_bitmap.h"
BitmapManager::BitmapManager()
{
}

BitmapManager::~BitmapManager()
{
	Release_Bitmap();
}


HDC BitmapManager::FindImage(const TCHAR* pImageKey)
{
	auto iter_find = _mapBmp.find(pImageKey);
	if (iter_find == _mapBmp.end())
		return nullptr;

	return iter_find->second->Get_MemDC();
}

void BitmapManager::Insert_Bitmap(HWND  hwnd, const TCHAR* pPath, const TCHAR* pImageKey)
{
	auto iter_find = _mapBmp.find(pImageKey);
	if (iter_find != _mapBmp.end())
		return;
	MyBitmap* pBitMap = new MyBitmap;
	pBitMap->Insert_Bitmap(hwnd, pPath);

	_mapBmp.emplace(pImageKey, pBitMap);

}

void BitmapManager::Release_Bitmap()
{
	for (auto rPair : _mapBmp)
	{
		delete rPair.second;
	}
	_mapBmp.clear();
}