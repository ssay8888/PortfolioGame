#include "../../../pch.h"
#include "in_game_scene.h"
#include "../../Skins/skin_manager.h"
#include "../../MapManager/map_manager.h"
#include "../../StringManager/string_manager.h"
#include "../../UiManager/ui_manager.h"
#include "../../UiManager/ui_button.h"
#include "../../EffectManager/effect_manager.h"
#include "../../MonsterMnager/monster_manager.h"
#include "../../../Components/game_mouse.h"

GameMouse* InGameScene::_mouse = nullptr;


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
	for (int i = 0; i < 2; i++)
	{
		object_manager->LoadMapData(i);
	}
	object_manager->ChangeMap(0, {0, 0});
	RECT rc{ 0, 0, static_cast<LONG>(800), static_cast<LONG>(600) };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(_hWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 0);
	_mouse = new GameMouse();
	_mouse->DoReadyGame();
	ShowCursor(false);
	UiManager::GetInstance()->ReadyUiManager();
	EffectManager::GetInstance()->LoadDamageNumber();
	StringManager::GetInstance()->LoadStringInfo();
	return 0;
}

void InGameScene::UpdateScene()
{
	MapManager::GetInstance()->UpdateGameObjectManager(0);
	UiManager::GetInstance()->UpdateUiManager();

	_mouse->DoUpdateObject(0);
}

void InGameScene::LateUpdateScene()
{
	MapManager::GetInstance()->LateUpdateGameObjectManager();
}

void InGameScene::RenderScene(HDC hdc)
{
	MapManager::GetInstance()->RenderGameObjectManager(hdc);
	MapManager::GetInstance()->RenderFootHoldManager(hdc);

	UiManager::GetInstance()->RenderUiManager(hdc);
	_mouse->DoRenderGameObject(hdc);


}

void InGameScene::ReleaseScene()
{
}

GameMouse* InGameScene::GetMouse()
{
	return _mouse;
}
