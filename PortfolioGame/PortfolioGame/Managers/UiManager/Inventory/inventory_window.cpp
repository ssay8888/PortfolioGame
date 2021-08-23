#include "../../../pch.h"
#include "inventory_window.h"
#include "../../UiManager/Inventory/inventory_window.h"
#include "../ui_tab_button.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../Components/game_mouse.h"
#include "../../../Components/MapObject/Item/item.h"
#include "../../../Components/MapObject/Player/player.h"
#include "../../../Components/MapObject/Player/Equip/equipment.h"
#include "../../../Components/MapObject/Player/Inventory/inventory.h"
#include "../../../Components/MapObject/Player/Inventory/eqp_inventory.h"
#include "../../KeyManaer/key_manager.h"
#include "../../MapManager/map_manager.h"
#include "../../UiManager/Equip/equipment_window.h"
#include "../../UiManager/ui_manager.h"
#include "../../Skins/skin_info.h"
#include "../../ScenManager/InGameScene/in_game_scene.h"
#include "../QuickSlot/quick_slot.h"
#include "../UiScroll/ui_scroll.h"

InventoryWindow::InventoryWindow() : _this_tab(),
_select_position(-1),
_scroll(new UiScroll()),
_item_distance(34),
_is_select_item(false)
{
}

InventoryWindow::~InventoryWindow()
{

}

void InventoryWindow::SetTab(const ObjectType::InventoryTabState tab)
{
	_this_tab = tab;
}

ObjectType::InventoryTabState InventoryWindow::GetTab() const
{
	return _this_tab;
}

void InventoryWindow::CharacterInventoryItemRender(const HDC hdc)
{
	auto player = MapManager::GetInstance()->GetPlayer();
	if (_this_tab == ObjectType::InventoryTabState::kEqp)
	{
		auto inventory = player->GetEqpInventory();
		auto item_list = inventory->GetItem();
		int32_t x = 9;
		int32_t y = 51;
		auto paddingsize = 51;
		for (int i = 0; i < inventory_slot_max; ++i)
		{
			if (item_list[i] != nullptr)
			{
				auto icon = item_list[i]->GetIcon();
				if (icon)
				{
					if (static_cast<int>(_info.y) + paddingsize + _scroll->GetScrollY() >= static_cast<int>(_info.y) + 51 &&
						static_cast<int>(_info.y) + paddingsize + _scroll->GetScrollY() <= static_cast<int>(_info.y) + 224)
					{
						icon->RenderBitmapImage(hdc,
							static_cast<int>(_info.x) + x,
							static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()),
							icon->GetWidth(),
							icon->GetHeight());
					}
				}
			}
			x += (36 + (36 * (i + 1) % 4));
			if ((i > 0 && (i + 1) % 4 == 0))
			{
				x = 9;
				y += _item_distance;
				paddingsize += _item_distance;
			}
		}
		return;
	}
	auto inventory = player->GetInventory(_this_tab);
	auto item_list = inventory->GetItem();
	int32_t x = 9;
	int32_t y = 51;
	auto paddingsize = 51;
	for (int i = 0; i < inventory_slot_max; ++i)
	{
		if (item_list[i] != nullptr)
		{
			auto icon = item_list[i]->GetIcon();
			if (icon)
			{
				if (static_cast<int>(_info.y) + paddingsize + _scroll->GetScrollY() >= static_cast<int>(_info.y) + 51 &&
					static_cast<int>(_info.y) + paddingsize + _scroll->GetScrollY() <= static_cast<int>(_info.y) + 224)
				{
					icon->RenderBitmapImage(hdc,
						static_cast<int>(_info.x) + x,
						static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()),
						icon->GetWidth(),
						icon->GetHeight());
					std::wstring quantity;
					quantity.append(std::to_wstring(item_list[i]->GetQuantity()));
					StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + x + 28,
						static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()) + 20, quantity, 13, RGB(0, 0, 0), L"±¼¸²Ã¼", TA_RIGHT);
				}
			}
		}
		x += (36 + (36 * (i + 1) % 4));
		if ((i > 0 && (i + 1) % 4 == 0))
		{
			x = 9;
			y += _item_distance;
			paddingsize += _item_distance;
		}
	}
}

std::shared_ptr<Item> InventoryWindow::PointCollisionItem(POINT pos)
{
	auto player = MapManager::GetInstance()->GetPlayer();
	auto inventory = player->GetInventory(_this_tab);
	auto item_list = inventory->GetItem();
	int32_t x = 9;
	int32_t y = 51;
	auto paddingsize = 51;
	for (int i = 0; i < inventory_slot_max; ++i)
	{
		if (item_list[i] != nullptr)
		{
			auto icon = item_list[i]->GetIcon();
			if (icon)
			{
				if (static_cast<int>(_info.y) + paddingsize + _scroll->GetScrollY() >= static_cast<int>(_info.y) + 51 &&
					static_cast<int>(_info.y) + paddingsize + _scroll->GetScrollY() <= static_cast<int>(_info.y) + 224)
				{
					RECT icon_rect{ static_cast<int>(_info.x + x),
						static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()),
								static_cast<int>(_info.x + x) + icon->GetWidth(),
						static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()) + icon->GetHeight() };
					if (PtInRect(&icon_rect, pos))
					{
						return item_list[i];
					}
				}
			}
		}
		x += (36 + (36 * (i + 1) % 4));
		if ((i > 0 && (i + 1) % 4 == 0))
		{
			x = 9;
			y += _item_distance;
			paddingsize += _item_distance;
		}
	}
	return nullptr;
}

std::shared_ptr<SkinInfo> InventoryWindow::PointCollisionEqp(POINT pos)
{
	auto player = MapManager::GetInstance()->GetPlayer();
	auto inventory = player->GetEqpInventory();
	auto item_list = inventory->GetItem();
	int32_t x = 9;
	int32_t y = 51;
	auto paddingsize = 51;
	for (int i = 0; i < inventory_slot_max; ++i)
	{
		if (item_list[i] != nullptr)
		{
			auto icon = item_list[i]->GetIcon();
			if (icon)
			{
				if (static_cast<int>(_info.y) + paddingsize + _scroll->GetScrollY() >= static_cast<int>(_info.y) + 51 &&
					static_cast<int>(_info.y) + paddingsize + _scroll->GetScrollY() <= static_cast<int>(_info.y) + 224)
				{
					RECT icon_rect{ static_cast<int>(_info.x + x),
						static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()),
								static_cast<int>(_info.x + x) + icon->GetWidth(),
						static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()) + icon->GetHeight() };
					if (PtInRect(&icon_rect, pos))
					{
						return item_list[i];
					}
				}
			}
		}
		x += (36 + (36 * (i + 1) % 4));
		if ((i > 0 && (i + 1) % 4 == 0))
		{
			x = 9;
			y += _item_distance;
			paddingsize += _item_distance;
		}
	}
	return nullptr;
}

std::shared_ptr<SkinInfo> InventoryWindow::PointCollisionEquipment(POINT pos)
{
	auto player = MapManager::GetInstance()->GetPlayer();
	auto inventory = player->GetEquipment();
	auto item_list = inventory->GetEquipItems();
	int32_t x = 9;
	int32_t y = 51;
	auto paddingsize = 51;
	for (auto& data: item_list)
	{
		auto icon = data.second->GetIcon();
		if (icon)
		{
			auto ui_eqpmanage = UiManager::GetInstance()->GetEquipmentWindow();
			auto point = ui_eqpmanage->FindEquipRenderPoint(data.first);
			RECT icon_rect{
			 static_cast<int>(ui_eqpmanage->GetInfo().x) + point.x,
			static_cast<int>(ui_eqpmanage->GetInfo().y) + point.y,
			static_cast<int>(ui_eqpmanage->GetInfo().x) + point.x + 30,
			static_cast<int>(ui_eqpmanage->GetInfo().y) + point.y + 30 };
			if (PtInRect(&icon_rect, pos))
			{
				return data.second;
			}
		}
	}
	return nullptr;
}

ObjectType::InventoryTabState InventoryWindow::SearchItemTab(int32_t item_id)
{
	const int item_division = item_id / 1000000;
	switch (item_division)
	{
	case 2:
		return ObjectType::InventoryTabState::kConsume;
	case 4:
		return ObjectType::InventoryTabState::kEtc;
	}
	return ObjectType::InventoryTabState::kEnd;
}

void InventoryWindow::ReadyWindow()
{
	_background = std::make_shared<MyBitmap>(MyBitmap());
	_background->Insert_Bitmap(_hWnd, L"Client\\Ui\\Inventory.img\\Item.backgrnd.bmp");
	_info.cx = _background->GetWidth();
	_info.cy = _background->GetHeight();

	_key_manager = new KeyManager();
	float x = 2;
	for (int i = 0; i < 5; ++i)
	{
		wchar_t path[150];
		swprintf_s(path, 150, L"Inventory.img\\%d", i);
		auto button = std::make_shared<UiTabButton>(UiTabButton());
		button->ReadyButton(path);
		button->SetInfoPosX(x);
		button->SetInfoPosY(23);
		x += 34;
		if (i == 0)
		{
			button->SetState(UiTabButton::ButtonState::kEnable);
		}
		_tab_buttons.emplace_back(button);
	}
}

void InventoryWindow::UpdateWindow()
{
	const POINT mouse = InGameScene::GetMouse()->GetPoint();
	_key_manager->KeyUpdate();
	//BaseUiEvent();

	auto player = MapManager::GetInstance()->GetPlayer();

	for (int i = 0; i < static_cast<int>(ObjectType::InventoryTabState::kEnd); ++i)
	{
		auto inventory = player->GetInventory(static_cast<ObjectType::InventoryTabState>(i));
		auto item_list = inventory->GetItem();
		for (int i = 0; i < inventory_slot_max; ++i)
		{
			if (item_list[i] != nullptr)
			{
				if (item_list[i]->GetQuantity() <= 0)
				{
					item_list[i] = nullptr;
				}
			}
		}
	}

	if (_key_manager->KeyDown(KEY_I))
	{
		SetShow(!IsShow());
	}
	if (InMouserWindow())
	{
		if (_key_manager->KeyDown(KEY_LBUTTON))
		{
			TitleBarClick(mouse);
			SelectTab(mouse);
			SetTab(FindTab());
			SelectItem(mouse);
		}
		if (_key_manager->KeyPressing(KEY_LBUTTON))
		{
			TitleBarMove(mouse);
			ScrollBarUp(mouse);
			ScrollBarDown(mouse);
		}

		if (_key_manager->KeyDown(KEY_RBUTTON))
		{
			if (GetTab() == ObjectType::InventoryTabState::kEqp)
			{
				EquipMove(mouse);
			}
		}
	}

	if (_key_manager->KeyUp(KEY_LBUTTON))
	{
		TitleBarUp(mouse);
		CancelSelectItem(mouse);
	}
}

void InventoryWindow::RenderWinodw(const HDC hdc)
{
	BaseUiRender(hdc);
	if (_show)
	{
		CharacterInventoryItemRender(hdc);
		SelectItemMoveing(hdc);
		const auto player = MapManager::GetInstance()->GetPlayer();
		std::wstring str;
		str.append(std::to_wstring(player->GetMeso()));
		StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 140, static_cast<int>(_info.y) + 268, str, 11, RGB(0, 0, 0), L"µ¸¿ò", TA_RIGHT);

	}
}

void InventoryWindow::ScrollBarUp(POINT mouse)
{
	const RECT up_button{
		static_cast<int>(_info.x + 155),
		static_cast<int>(_info.y + 51),
		static_cast<int>(_info.x + 165),
		static_cast<int>(_info.y) + 61 };
	if (PtInRect(&up_button, mouse))
	{
		if (_scroll->GetScrollY() != 0)
		{
			_scroll->GainScrollY(static_cast<float>(_item_distance));
		}
	}
}

void InventoryWindow::ScrollBarDown(POINT mouse)
{
	const RECT up_button{
		static_cast<int>(_info.x + 156),
		static_cast<int>(_info.y + 242),
		static_cast<int>(_info.x + 164),
		static_cast<int>(_info.y + 250) };

	if (PtInRect(&up_button, mouse))
	{
		const auto max_distance = _item_distance * 24;
		if (-max_distance != _scroll->GetScrollY())
		{
			_scroll->GainScrollY(static_cast<float>(-_item_distance));
		}
	}
}

void InventoryWindow::SelectItem(POINT mouse)
{
	if (_this_tab == ObjectType::InventoryTabState::kEqp)
	{
		auto player = MapManager::GetInstance()->GetPlayer();
		auto inventory = player->GetEqpInventory();
		auto item_list = inventory->GetItem();
		int32_t x = 9;
		int32_t y = 51;
		auto paddingsize = 51;
		for (int i = 0; i < inventory_slot_max; ++i)
		{
			if (item_list[i] != nullptr)
			{
				auto icon = item_list[i]->GetIcon();
				if (icon)
				{
					if (static_cast<int>(_info.y) + paddingsize + _scroll->GetScrollY() >= static_cast<int>(_info.y) + 51 &&
						static_cast<int>(_info.y) + paddingsize + _scroll->GetScrollY() <= static_cast<int>(_info.y) + 224)
					{
						RECT icon_rect{ static_cast<int>(_info.x + x),
							static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()),
									static_cast<int>(_info.x + x) + icon->GetWidth(),
							static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()) + icon->GetHeight() };
						if (PtInRect(&icon_rect, mouse))
						{
							_is_select_item = true;
							_select_eqp_item = item_list[i];
							_select_position = i;
						}
					}
				}
			}
			x += (36 + (36 * (i + 1) % 4));
			if ((i > 0 && (i + 1) % 4 == 0))
			{
				x = 9;
				y += _item_distance;
				paddingsize += _item_distance;
			}
		}
		return;
	}
	auto player = MapManager::GetInstance()->GetPlayer();
	auto inventory = player->GetInventory(_this_tab);
	auto item_list = inventory->GetItem();
	int32_t x = 9;
	int32_t y = 51;
	auto paddingsize = 51;
	for (int i = 0; i < inventory_slot_max; ++i)
	{
		if (item_list[i] != nullptr)
		{
			auto icon = item_list[i]->GetIcon();
			if (icon)
			{
				if (static_cast<int>(_info.y) + paddingsize + _scroll->GetScrollY() >= static_cast<int>(_info.y) + 51 &&
					static_cast<int>(_info.y) + paddingsize + _scroll->GetScrollY() <= static_cast<int>(_info.y) + 224)
				{
					RECT icon_rect{ static_cast<int>(_info.x + x),
						static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()),
								static_cast<int>(_info.x + x) + icon->GetWidth(),
						static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()) + icon->GetHeight() };
					if (PtInRect(&icon_rect, mouse))
					{
						_is_select_item = true;
						_select_item = item_list[i];
						_select_position = i;
					}
				}
			}
		}
		x += (36 + (36 * (i + 1) % 4));
		if ((i > 0 && (i + 1) % 4 == 0))
		{
			x = 9;
			y += _item_distance;
			paddingsize += _item_distance;
		}
	}
}

void InventoryWindow::SelectItemMoveing(HDC hdc)
{
	if (_is_select_item && (_select_item || _select_eqp_item))
	{
		const POINT mouse = InGameScene::GetMouse()->GetPoint();
		std::shared_ptr<MyBitmap> icon;
		if (_select_item)
		{
			icon = ((_select_item)->GetIconRaw());
		}
		else
		{
			icon = ((_select_eqp_item)->GetIconRaw());
		}
		icon->RenderBitmapImage(hdc, 
			mouse.x - (icon->GetWidth() >> 1),
			mouse.y - (icon->GetHeight() >> 1),
			icon->GetWidth(), 
			icon->GetHeight());
	}
}

void InventoryWindow::CancelSelectItem(POINT mouse)
{
	if (_select_position == -1)
	{
		return;
	}
	if (_this_tab == ObjectType::InventoryTabState::kEqp)
	{
		auto player = MapManager::GetInstance()->GetPlayer();
		auto inventory = player->GetEqpInventory();
		auto item_list = inventory->GetItem();
		int32_t x = 9;
		int32_t y = 51;
		auto paddingsize = 51;
		for (int i = 0; i < inventory_slot_max; ++i)
		{
			if (item_list[i] != nullptr)
			{
				auto icon = item_list[i]->GetIcon();
				if (icon)
				{
					if (static_cast<int>(_info.y) + paddingsize + _scroll->GetScrollY() >= static_cast<int>(_info.y) + 51 &&
						static_cast<int>(_info.y) + paddingsize + _scroll->GetScrollY() <= static_cast<int>(_info.y) + 224)
					{
						RECT icon_rect{ static_cast<int>(_info.x + x),
							static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()),
									static_cast<int>(_info.x + x) + 30,
							static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()) + 30 };
						if (PtInRect(&icon_rect, mouse))
						{
							inventory->MoveItem(_select_position, i);
						}
					}
				}
			}
			else
			{
				RECT icon_rect{ static_cast<int>(_info.x + x),
					static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()),
							static_cast<int>(_info.x + x) + 30,
					static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()) + 30 };
				if (PtInRect(&icon_rect, mouse))
				{
					inventory->MoveItem(_select_position, i);
				}
			}
			x += (36 + (36 * (i + 1) % 4));
			if ((i > 0 && (i + 1) % 4 == 0))
			{
				x = 9;
				y += _item_distance;
				paddingsize += _item_distance;
			}
		}

		_select_item = nullptr;
		_is_select_item = false;
		_select_eqp_item = nullptr;
		_select_position = -1;
		return;
	}
	if (_this_tab == ObjectType::InventoryTabState::kConsume)
	{
		RECT quick_slot_rect{ 655, 462, 655 + 134, 462 + 63 };
		if (PtInRect(&quick_slot_rect, mouse))
		{
			int totalNum = 0;
			for (int y = 0; y <= 2; ++y)
			{
				for (int x = 0; x < 4; ++x)
				{
					RECT slot{
						655 + (34 * x),
						462 + (y * 31),
						655 + (34 * x) + 34,
						462 + (31 * y) + 31 };

					if (PtInRect(&slot, mouse))
					{
						auto quick_slot = UiManager::GetInstance()->GetQuickSlot();
						quick_slot->ChangeSlotItem(static_cast<QuickSlot::KeyBoard>(totalNum), _select_item);
					}
					++totalNum;
				}
			}
		}
		else
		{

			if (_select_item->GetItemId() / 10000 == 204  && UiManager::GetInstance()->GetEquipmentWindow()->IsShow())
			{
				auto eqp_window = UiManager::GetInstance()->GetEquipmentWindow();

				RECT eqp_rect{
					static_cast<int>(eqp_window->GetInfo().x),
					static_cast<int>(eqp_window->GetInfo().y),
					static_cast<int>(eqp_window->GetInfo().x + eqp_window->GetInfo().cx),
					static_cast<int>(eqp_window->GetInfo().y + eqp_window->GetInfo().cy) };

				if (PtInRect(&eqp_rect, mouse))
				{
					const auto player = MapManager::GetInstance()->GetPlayer();
					for (int i = 0; i < ObjectType::EquipPosition::kEquipEnd; ++i)
					{
						auto pos_enum = static_cast<ObjectType::EquipPosition>(i);
						auto pos = eqp_window->FindEquipRenderPoint(pos_enum);

						RECT rc{
							static_cast<int>(eqp_window->GetInfo().x + pos.x),
							static_cast<int>(eqp_window->GetInfo().y + pos.y),
							static_cast<int>(eqp_window->GetInfo().x + pos.x + 30),
							static_cast<int>(eqp_window->GetInfo().y + pos.y + 30) };
						if (PtInRect(&rc, mouse))
						{
							auto item = player->GetEquipment()->FindItem(pos_enum);
							if (item != nullptr)
							{
								player->GetEquipment()->UseScrollItem(item, _select_item);
								_select_item = nullptr;
								_is_select_item = false;
								_select_eqp_item = nullptr;
								_select_position = -1;
								return;
							}
						}
					}
				}
			}
			auto player = MapManager::GetInstance()->GetPlayer();
			auto inventory = player->GetInventory(_this_tab);
			auto item_list = inventory->GetItem();
			int32_t x = 9;
			int32_t y = 51;
			auto paddingsize = 51;
			for (int i = 0; i < inventory_slot_max; ++i)
			{
				if (item_list[i] != nullptr)
				{
					auto icon = item_list[i]->GetIcon();
					if (icon)
					{
						if (static_cast<int>(_info.y) + paddingsize + _scroll->GetScrollY() >= static_cast<int>(_info.y) + 51 &&
							static_cast<int>(_info.y) + paddingsize + _scroll->GetScrollY() <= static_cast<int>(_info.y) + 224)
						{
							RECT icon_rect{ static_cast<int>(_info.x + x),
								static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()),
										static_cast<int>(_info.x + x) + 30,
								static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()) + 30 };
							if (PtInRect(&icon_rect, mouse))
							{
								inventory->MoveItem(_select_position, i);
							}
						}
					}
				}
				else
				{
					RECT icon_rect{ static_cast<int>(_info.x + x),
						static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()),
								static_cast<int>(_info.x + x) + 30,
						static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()) + 30 };
					if (PtInRect(&icon_rect, mouse))
					{
						inventory->MoveItem(_select_position, i);
					}
				}
				x += (36 + (36 * (i + 1) % 4));
				if ((i > 0 && (i + 1) % 4 == 0))
				{
					x = 9;
					y += _item_distance;
					paddingsize += _item_distance;
				}
			}
		}

		_select_item = nullptr;
		_is_select_item = false;
		_select_eqp_item = nullptr;
		_select_position = -1;
		return;
	}
	auto player = MapManager::GetInstance()->GetPlayer();
	auto inventory = player->GetInventory(_this_tab);
	auto item_list = inventory->GetItem();
	int32_t x = 9;
	int32_t y = 51;
	auto paddingsize = 51;
	for (int i = 0; i < inventory_slot_max; ++i)
	{
		if (item_list[i] != nullptr)
		{
			auto icon = item_list[i]->GetIcon();
			if (icon)
			{
				if (static_cast<int>(_info.y) + paddingsize + _scroll->GetScrollY() >= static_cast<int>(_info.y) + 51 &&
					static_cast<int>(_info.y) + paddingsize + _scroll->GetScrollY() <= static_cast<int>(_info.y) + 224)
				{
					RECT icon_rect{ static_cast<int>(_info.x + x),
						static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()),
								static_cast<int>(_info.x + x) + 30,
						static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()) + 30 };
					if (PtInRect(&icon_rect, mouse))
					{
						inventory->MoveItem(_select_position, i);
					}
				}
			}
		} else
		{
			RECT icon_rect{ static_cast<int>(_info.x + x),
				static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()),
						static_cast<int>(_info.x + x) + 30,
				static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()) + 30 };
			if (PtInRect(&icon_rect, mouse))
			{
				inventory->MoveItem(_select_position, i);
			}
		}
		x += (36 + (36 * (i + 1) % 4));
		if ((i > 0 && (i + 1) % 4 == 0))
		{
			x = 9;
			y += _item_distance;
			paddingsize += _item_distance;
		}
	}

	_select_item = nullptr;
	_is_select_item = false;
	_select_eqp_item = nullptr;
	_select_position = -1;
}

void InventoryWindow::EquipMove(POINT mouse)
{
	const auto player = MapManager::GetInstance()->GetPlayer();
	const auto inventory = player->GetEqpInventory();
	auto equipment = player->GetEquipment();
	auto item_list = inventory->GetItem();
	int32_t x = 9;
	int32_t y = 51;
	auto paddingsize = 51;
	for (int i = 0; i < inventory_slot_max; ++i)
	{
		if (item_list[i] != nullptr)
		{
			auto icon = item_list[i]->GetIcon();
			if (icon)
			{
				if (static_cast<int>(_info.y) + paddingsize + _scroll->GetScrollY() >= static_cast<int>(_info.y) + 51 &&
					static_cast<int>(_info.y) + paddingsize + _scroll->GetScrollY() <= static_cast<int>(_info.y) + 224)
				{
					RECT icon_rect{ static_cast<int>(_info.x + x),
						static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()),
								static_cast<int>(_info.x + x) + 30,
						static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()) + 30 };
					if (PtInRect(&icon_rect, mouse))
					{
						equipment->InsertEquipItem(item_list[i]);
						item_list[i] = nullptr;
						player->RecalcEqpStat();
						break;
						//inventory->MoveItem(_select_position, i);
					}
				}
			}
		}
		x += (36 + (36 * (i + 1) % 4));
		if ((i > 0 && (i + 1) % 4 == 0))
		{
			x = 9;
			y += _item_distance;
			paddingsize += _item_distance;
		}
	}

	_select_item = nullptr;
	_is_select_item = false;
	_select_eqp_item = nullptr;
	_select_position = -1;
}

ObjectType::InventoryTabState InventoryWindow::FindTab()
{
	for (int i = 0; i < _tab_buttons.size(); ++i)
	{
		const auto state = _tab_buttons[i]->GetState();
		if (UiTabButton::ButtonState::kEnable == state)
		{
			return static_cast<ObjectType::InventoryTabState>(i);
		}
	}
	return static_cast<ObjectType::InventoryTabState>(5);
}
