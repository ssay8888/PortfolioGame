#include "../pch.h"
#include "main_game.h"
#include "../Components/MapObject/player.h"
#include "../Managers/Skins/skin_info.h"
#include "../Managers/Skins/skin_frame.h"
#include "../Managers/Skins/skin_item.h"
#include "../Managers/Skins/skin_manager.h"
#include "../Utility/xml_reader.h"
#include "../Managers/KeyManaer/key_manager.h"
#include "../Managers/MapManager/map_manager.h"
#include "../Managers/ScrollManager/scroll_manager.h"
#include "../Managers/ScenManager/scene_manager.h"
#include <fstream>
#include <algorithm>
MainGame::MainGame(HDC hdc) :
	_hdc(hdc),
	_hdc_buffer(nullptr),
	_hBitmap(nullptr),
	_oldBitmap(nullptr),
	_ticksCount(GetTickCount64()),
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

	auto object_manager = MapManager::GetInstance();
	auto object = new Player();
	this->SetPlayer(object);
	object_manager->AddGameObject(object);
	SceneManager::GetInstance()->SceneChange(SceneManager::SceneState::kLoading);
}

void MainGame::UpdateGame()
{


	float deltaTime = (GetTickCount64() - _ticksCount) / 1000.0f;

	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	_ticksCount = GetTickCount64();
	KeyManager::GetInstance()->KeyUpdate();

	SceneManager::GetInstance()->UpdateSceneManager();
	ScrollManager::ScrollLock();
}

void MainGame::LateUpdateGame()
{
	SceneManager::GetInstance()->LateUpdateSceneManager();
	//MapManager::GetInstance()->LateUpdateGameObjectManager();
}

void MainGame::RenderGame()
{
	Rectangle(_hdc_buffer, -10, -10, WindowCX + 10, WindowCY + 10);
	//MapManager::GetInstance()->RenderGameObjectManager(_hdc_buffer);
	//MapManager::GetInstance()->RenderFootHoldManager(_hdc_buffer);
	SceneManager::GetInstance()->RenderSceneManager(_hdc_buffer);
	++_iFPS;
	if (_dwFPSTime + 1000 < GetTickCount64())
	{
		swprintf_s(_szFPS, L"FPS : %d", _iFPS);
		_iFPS = 0;
		_dwFPSTime = GetTickCount64();
	}
	TextOut(_hdc_buffer, 0, 0, _szFPS, lstrlen(_szFPS));
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
