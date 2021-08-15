#include "../../../pch.h"
#include "equipment_window.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../Components/game_mouse.h"
#include "../../../Components/MapObject/Player/player.h"
#include "../../KeyManaer/key_manager.h"
#include "../../ScenManager/InGameScene/in_game_scene.h"
#include "../../MapManager/map_manager.h"
#include "../../../Components/MapObject/Player/Equip/equipment.h"
#include "../../../Components/MapObject/Player/Inventory/eqp_inventory.h"
#include "../../../Components/MapObject/Player/Inventory/inventory.h"
#include "../../Skins/skin_info.h"

EquipmentWindow::EquipmentWindow()
{
}

EquipmentWindow::~EquipmentWindow()
{
}

void EquipmentWindow::EquipmentItemRender(const HDC hdc) const
{
	const auto player = MapManager::GetInstance()->GetPlayer();
	const auto equipment = player->GetEquipment();
	const auto items = equipment->GetEquipItems();
	for (const auto eqp : items)
	{
		const auto pos = FindEquipRenderPoint(eqp.first);
		const auto icon = eqp.second->GetIcon();
		icon->RenderBitmapImage(hdc,
			static_cast<int>(_info.x + pos.x),
			static_cast<int>(_info.y + pos.y),
			icon->GetWidth(),
			icon->GetHeight());
	}
}

void EquipmentWindow::ReadyWindow()
{
	_background = std::make_shared<MyBitmap>(MyBitmap());
	_background->Insert_Bitmap(_hWnd, L"Client\\Ui\\Equipment.img\\backgrnd.bmp");
	_info.cx = _background->GetWidth();
	_info.cy = _background->GetHeight();

	_key_manager = new KeyManager();
}

void EquipmentWindow::UpdateWindow()
{
	_key_manager->KeyUpdate();
	const POINT mouse = InGameScene::GetMouse()->GetPoint();
	if (_key_manager->KeyDown(KEY_E))
	{
		SetShow(!IsShow());
	}
	if (InMouserSkillWindow())
	{
		if (_key_manager->KeyDown(KEY_LBUTTON))
		{
			TitleBarClick(mouse);
			SelectTab(mouse);
		}
		if (_key_manager->KeyPressing(KEY_LBUTTON))
		{
			TitleBarMove(mouse);
		}
		if (_key_manager->KeyUp(KEY_LBUTTON))
		{
			TitleBarUp(mouse);
		}
		if (_key_manager->KeyUp(KEY_RBUTTON))
		{
			UnEquipItem(mouse);
		}
	}

}

void EquipmentWindow::RenderWinodw(const HDC hdc)
{
	if (_show)
	{
		BaseUiRender(hdc);
		EquipmentItemRender(hdc);
	}
}

void EquipmentWindow::UnEquipItem(POINT mouse)
{
	const auto player = MapManager::GetInstance()->GetPlayer();
	for (int i = 0; i < ObjectType::EquipPosition::kEquipEnd; ++i)
	{
		auto pos_enum = static_cast<ObjectType::EquipPosition>(i);
		auto pos = FindEquipRenderPoint(pos_enum);

		RECT rc{
			static_cast<int>(_info.x + pos.x),
			static_cast<int>(_info.y + pos.y),
			static_cast<int>(_info.x + pos.x + 30),
			static_cast<int>(_info.y + pos.y + 30)};
		if (PtInRect(&rc, mouse))
		{
			auto item = player->GetEquipment()->FindItem(pos_enum);
			if (item != nullptr)
			{
				auto free_slot = player->GetEqpInventory()->FindFreeSlot();
				player->GetEqpInventory()->AddItem(free_slot, item);
				player->GetEquipment()->RemoveItem(pos_enum);
				return;
			}
		}
	}
}

POINT EquipmentWindow::FindEquipRenderPoint(const ObjectType::EquipPosition type)
{
	POINT rc{};
	switch (type) {
	case ObjectType::kCap:
		rc.x = 39;
		rc.y = 36;
		break;
	case ObjectType::kCoat:
		rc.x = 39;
		rc.y = 135;
		break;
	case ObjectType::kPants:
		rc.x = 39;
		rc.y = 168;
		break;
	case ObjectType::kShoes:
		rc.x = 72;
		rc.y = 201;
		break;
	case ObjectType::kWeapon:
		rc.x = 105;
		rc.y = 135;
		break;
	case ObjectType::kEquipEnd: 
		break;
	default: ;
	}
	return rc;
}
