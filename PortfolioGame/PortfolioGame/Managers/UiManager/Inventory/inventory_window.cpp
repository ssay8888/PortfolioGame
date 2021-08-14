#include "../../../pch.h"
#include "inventory_window.h"
#include "../../UiManager/Inventory/inventory_window.h"
#include "../ui_tab_button.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../Components/game_mouse.h"
#include "../../../Components/MapObject/Item/item.h"
#include "../../../Components/MapObject/Player/player.h"
#include "../../../Components/MapObject/Player/Inventory/inventory.h"
#include "../../../Components/MapObject/Player/Inventory/eqp_inventory.h"
#include "../../KeyManaer/key_manager.h"
#include "../../MapManager/map_manager.h"
#include "../../Skins/skin_info.h"
#include "../../ScenManager/InGameScene/in_game_scene.h"
#include "../UiScroll/ui_scroll.h"

InventoryWindow::InventoryWindow(): _this_tab(),
                                    _select_position(-1),
                                    _scroll(new UiScroll()),
                                    _item_distance(34),
                                    _is_select_item(false)
{
}

InventoryWindow::~InventoryWindow()
{
	delete _scroll;
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
					quantity.append(std::to_wstring(item_list[i]->Getquantity()));
					StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + x + 20,
						static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()) + 20, quantity, 13, RGB(0, 0, 0));
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

void InventoryWindow::ReadyWindow()
{
	_background = std::make_shared<MyBitmap>(MyBitmap());
	_background->Insert_Bitmap(_hWnd, L"Client\\Ui\\Inventory.img\\Item.backgrnd.bmp");
	_info.cx = _background->GetWidth();
	_info.cy = _background->GetWidth();

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
	if (_key_manager->KeyUp(KEY_LBUTTON))
	{
		TitleBarUp(mouse);
		CancelSelectItem(mouse);
	}

	if (_key_manager->KeyDown(KEY_I))
	{
		SetShow(!IsShow());
	}
}

void InventoryWindow::RenderWinodw(const HDC hdc)
{
	BaseUiRender(hdc);
	if (_show)
	{
		CharacterInventoryItemRender(hdc);
		SelectItemMoveing(hdc);
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
		static_cast<int>(_info.y) + 250 };
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
