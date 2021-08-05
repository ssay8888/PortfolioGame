#include "../../../pch.h"
#include "in_game_scene.h"
#include "../../Skins/skin_manager.h"
#include "../../MapManager/map_manager.h"


InGameScene::InGameScene()
{
	ReadyScene();
}

InGameScene::~InGameScene()
{
}

int InGameScene::ReadyScene()
{
	SkinManager::GetInstance()->LoadSkin();
	auto object_manager = MapManager::GetInstance();
	object_manager->ReadyMapManager();
	object_manager->LoadMapData();
	RECT rc{ 0, 0, static_cast<LONG>(WindowCX), static_cast<LONG>(WindowCY) };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(_hWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 0);
	return 0;
}

void InGameScene::UpdateScene()
{
	MapManager::GetInstance()->UpdateGameObjectManager(0);
}

void InGameScene::LateUpdateScene()
{
	MapManager::GetInstance()->LateUpdateGameObjectManager();
}

void InGameScene::RenderScene(HDC hdc)
{
	MapManager::GetInstance()->RenderGameObjectManager(hdc);
	MapManager::GetInstance()->RenderFootHoldManager(hdc);

}

void InGameScene::ReleaseScene()
{
}
