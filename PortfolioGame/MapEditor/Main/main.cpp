#include "../pch.h"
#include "main.h"
#include "../Object/mouse.h"
#include "../Managers/KeyManaer/key_manager.h"
#include "../Managers/ScrollManager/scroll_manager.h"
#include "../Managers/MapManager/map_manager.h"

Main::Main(HDC hdc) :
	_hdc(hdc),
	_bitmap(nullptr),
	_hdc_buffer(nullptr),
	_old_bitmap(nullptr)
{
}

Main::~Main()
{
}

void Main::Ready_Edit()
{
	_hdc_buffer = CreateCompatibleDC(_hdc);
	_bitmap = CreateCompatibleBitmap(_hdc, 1600, 900);
	_old_bitmap = (HBITMAP)SelectObject(_hdc_buffer, _bitmap);
	MapManager::GetInstance()->Ready_Map();
}

void Main::Update_Edit()
{
	KeyManager::GetInstance()->KeyUpdate();
	MapManager::GetInstance()->Update_Map();
}

void Main::Render_Edit()
{
	Rectangle(_hdc_buffer, 0, 0, 1600, 900);
	Rectangle(_hdc_buffer, 0, 0, 1024, 768);
	MapManager::GetInstance()->Render_Map(_hdc_buffer);
	BitBlt(_hdc, 0, 0, 1600, 900, _hdc_buffer, 0, 0, SRCCOPY);

}

void Main::Release_Edit()
{
}