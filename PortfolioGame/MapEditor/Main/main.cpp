#include "../pch.h"
#include "main.h"
#include "../Object/mouse.h"
#include "../Managers/KeyManaer/key_manager.h"
#include "../Managers/ScrollManager/scroll_manager.h"
#include "../Managers/MapManager/map_manager.h"

Main::Main(HDC hdc) :
	_hdc(hdc),
	_hBitmap(nullptr),
	_hdc_buffer(nullptr),
	_oldBitmap(nullptr)
{
}

Main::~Main()
{
}

void Main::Ready_Edit()
{
	_hdc_buffer = CreateCompatibleDC(_hdc);
	_hBitmap = CreateCompatibleBitmap(_hdc, 1600, 768);
	_oldBitmap = (HBITMAP)SelectObject(_hdc_buffer, _hBitmap);
	MapManager::GetInstance()->Ready_Map();
}

void Main::Update_Edit()
{
	KeyManager::Get_Instance()->KeyUpdate();
	MapManager::GetInstance()->Update_Map();
}

void Main::Render_Edit()
{
	Rectangle(_hdc_buffer, 0, 0, 1500, 768);
	Rectangle(_hdc_buffer, 0, 0, 800, 600);
	MapManager::GetInstance()->Render_Map(_hdc_buffer);
	BitBlt(_hdc, 0, 0, 1500, 768, _hdc_buffer, 0, 0, SRCCOPY);

}

void Main::Release_Edit()
{
}