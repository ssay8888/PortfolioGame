#include "../pch.h"
#include "main_game.h"
#include "../Components/MapObject/player.h"
#include "../Managers/Skins/skin_info.h"
#include "../Managers/Skins/skin_frame.h"
#include "../Managers/Skins/skin_item.h"
#include "../Managers/Skins/skin_manager.h"
#include "../Utility/xml_reader.h"
#include "../Managers/KeyManaer/key_manager.h"
#include "../Managers/ObjectManager/object_manager.h"
#include <fstream>
#include <algorithm>
MainGame::MainGame(HDC hdc) :
	_hdc(hdc),
	_hdc_buffer(nullptr),
	_hBitmap(nullptr),
	_oldBitmap(nullptr),
	_ticksCount(0),
	_player(nullptr)
{
}

MainGame::~MainGame()
{
	ReleaseGame();
}

void MainGame::ReadeyGame()
{
	SkinManager::GetInstance()->LoadSkin();
	_hdc_buffer = CreateCompatibleDC(_hdc);
	_hBitmap = CreateCompatibleBitmap(_hdc, 1024, 768);
	_oldBitmap = (HBITMAP)SelectObject(_hdc_buffer, _hBitmap);

	auto object_manager = ObjectManager::Get_Instance();
	auto object = new Player();
	this->SetPlayer(object);
	object_manager->AddGameObject(object);
}

void MainGame::UpdateGame()
{
	KeyManager::Get_Instance()->KeyUpdate();
	while (!GetTickCount64() - (_ticksCount + 16) <= 0);

	float deltaTime = (GetTickCount64() - _ticksCount) / 1000.0f;

	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	_ticksCount = GetTickCount64();

	ObjectManager::Get_Instance()->UpdateGameObjectManager(deltaTime);
}

void MainGame::RenderGame()
{
	Rectangle(_hdc_buffer, -10, -10, WindowCX + 10, WindowCY + 10);
	ObjectManager::Get_Instance()->RenderGameObjectManager(_hdc_buffer);
	BitBlt(_hdc, 0, 0, WindowCX, WindowCY, _hdc_buffer, 0, 0, SRCCOPY);
}

void MainGame::ReleaseGame()
{
	DeleteObject(_hBitmap);
	DeleteObject(_oldBitmap);
	DeleteDC(_hdc_buffer);
}

void MainGame::SetPlayer(Player* player)
{
	_player = player;
}

Player* MainGame::GetPlayer() const
{
	return _player;
}
