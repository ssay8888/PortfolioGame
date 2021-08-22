#include "../../../pch.h"
#include "shop_window.h"
#include "../ui_button.h"
#include "../ui_manager.h"
#include "../ui_tab_button.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../Components/game_mouse.h"
#include "../../../Components/MapObject/Item/item.h"
#include "../../../Components/MapObject/Player/player.h"
#include "../../../Components/MapObject/Player/Inventory/eqp_inventory.h"
#include "../../../Components/MapObject/Player/Inventory/inventory.h"
#include "../../ItemManager/item_manager.h"
#include "../../KeyManaer/key_manager.h"
#include "../../MapManager/map_manager.h"
#include "../../ScenManager/InGameScene/in_game_scene.h"
#include "../../ShopManager/shop_item.h"
#include "../../ShopManager/shop_manager.h"
#include "../../Skins/item_info.h"
#include "../../Skins/skin_info.h"
#include "../../StringManager/string_Info.h"
#include "../../StringManager/string_manager.h"
#include "../UiScroll/ui_scroll.h"

ShopWindow::ShopWindow(): _this_tab(ObjectType::InventoryTabState::kEqp),
                          _inven_scroll(nullptr),
                          _shop_scroll(nullptr),
                          _item_distance(40),
                          _buttonKeyManager(nullptr)
{
}

ShopWindow::~ShopWindow()
{
	if (_buttonKeyManager != nullptr)
	delete _buttonKeyManager;
	if (_inven_scroll != nullptr)
	{
		delete _inven_scroll;
	}
	if (_shop_scroll != nullptr)
	{
		delete _shop_scroll;
	}
}

void ShopWindow::ReadyWindow()
{
	_inven_scroll = new UiScroll();
	_shop_scroll = new UiScroll();
	_background = std::make_shared<MyBitmap>(MyBitmap());
	_background->Insert_Bitmap(_hWnd, L"Client\\Ui\\Shop.img\\Shop.backgrnd.bmp");
	_select_image = std::make_shared<MyBitmap>(MyBitmap());
	_select_image->Insert_Bitmap(_hWnd, L"Client\\Ui\\Shop.img\\Shop.select.bmp");

	_info.x = static_cast<float>(400 - (_background->GetWidth() >> 1));
	_info.y = static_cast<float>(300 - (_background->GetHeight() >> 1));

	_info.cx = _background->GetWidth();
	_info.cy = _background->GetHeight();

	_key_manager = new KeyManager();
	int x = 234;
	for (int i = 0; i < 5; ++i)
	{
		wchar_t path[150];
		swprintf_s(path, 150, L"Shop.img\\%d", i);
		auto button = std::make_shared<UiTabButton>(UiTabButton());
		button->ReadyButton(path);
		button->SetInfoPosX(static_cast<float>(400 - (_background->GetWidth() >> 1) + x));
		button->SetInfoPosY(static_cast<float>(300 - (_background->GetHeight() >> 1) + 94));
		x += 45;
		if (i == 0)
		{
			button->SetState(UiTabButton::ButtonState::kEnable);
		}
		_tab_buttons.emplace_back(button);
	}

	_buttonKeyManager = new KeyManager();
	auto button = std::make_shared<UiButton>(UiButton(_buttonKeyManager));
	button->SetObjectPos({ _info.x + 154, _info.y + 34 });
	button->ReadyButton(L"Shop.img\\Shop.BtBuy");
	button->SetCallBack(BuyItemShop);
	_shop_buy_button = button;
	button = std::make_shared<UiButton>(UiButton(_buttonKeyManager));
	button->SetObjectPos({ _info.x + 385, _info.y + 35 });
	button->ReadyButton(L"Shop.img\\Shop.BtSell");
	button->SetCallBack(SellItemShop);
	_shop_sell_button = button;
	button = std::make_shared<UiButton>(UiButton(_buttonKeyManager));
	button->SetObjectPos({ _info.x + 154, _info.y + 14 });
	button->ReadyButton(L"Shop.img\\Shop.BtExit");
	button->SetCallBack(ExitShop);
	_shop_exit_button = button;
	LoadShopData(0);
}

void ShopWindow::UpdateWindow()
{
	if(_show)
	{
		const POINT mouse = InGameScene::GetMouse()->GetPoint();
		_key_manager->KeyUpdate();

		if (InMouserWindow())
		{
			if (_key_manager->KeyDown(KEY_LBUTTON))
			{
				TitleBarClick(mouse);
				SelectTab(mouse);
				SetTab(FindTab());
				SelectItem(mouse);
				SelectBuyItem(mouse);
				InvenScrollUp(mouse);
				InvenScrollDown(mouse);
				ShopScrollUp(mouse);
				ShopScrollDown(mouse);
			}
			if (_key_manager->KeyPressing(KEY_LBUTTON))
			{
				if (_move_window)
				{
					_click_pos.x = _click_pos.x - mouse.x;
					_click_pos.y = _click_pos.y - mouse.y;
					_info.x -= static_cast<float>(_click_pos.x);
					_info.y -= static_cast<float>(_click_pos.y);
					for (const auto tab_button : _tab_buttons)
					{
						tab_button->GainInfoPosXY(static_cast<float>(-_click_pos.x), static_cast<float>(-_click_pos.y));
					}
					_shop_sell_button->GainObjectPosXY(static_cast<float>(-_click_pos.x), static_cast<float>(-_click_pos.y));
					_shop_exit_button->GainObjectPosXY(static_cast<float>(-_click_pos.x), static_cast<float>(-_click_pos.y));
					_shop_buy_button->GainObjectPosXY(static_cast<float>(-_click_pos.x), static_cast<float>(-_click_pos.y));
					_click_pos = mouse;
				}
			}

		}

		if (_key_manager->KeyUp(KEY_LBUTTON))
		{
			TitleBarUp(mouse);
		}
		UpdateShopButton(mouse);
	}
}

void ShopWindow::RenderWinodw(HDC hdc)
{
	if (_show)
	{
		BaseUiRender(hdc);
		CharacterInventoryItemRender(hdc);
		RenderShopItems(hdc);
		RenderMeso(hdc);
		RenderShopButton(hdc);
		RenderText(hdc);
	}
}

ObjectType::InventoryTabState ShopWindow::FindTab()
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

void ShopWindow::RenderShopItems(HDC hdc)
{
	auto paddingsize = 127;
	for (const auto shop_item : _shop_items)
	{
		auto item = ItemManager::GetInstance()->FindItem(shop_item->GetItemId());

		auto icon = item->GetIcon();
		if (icon)
		{

			if (static_cast<int>(_info.y) + paddingsize + _shop_scroll->GetScrollY() >= static_cast<int>(_info.y) + 127 &&
				static_cast<int>(_info.y) + paddingsize + _shop_scroll->GetScrollY() <= static_cast<int>(_info.y) + 287)
			{

				if (_select_buy_item != nullptr && 
					_select_buy_item->GetItemId() == item->GetItemId() && 
					_select_buy_item->GetPrice() == shop_item->GetPrice())
				{
					_select_image->RenderBitmapImage(hdc, static_cast<int>(_info.x) + 42,
						static_cast<int>(_info.y + paddingsize + _shop_scroll->GetScrollY()),
						_select_image->GetWidth(), _select_image->GetHeight());
				}

				icon->RenderBitmapImage(hdc,
					static_cast<int>(_info.x) + 6,
					static_cast<int>(_info.y + paddingsize + _shop_scroll->GetScrollY()),
					icon->GetWidth(),
					icon->GetHeight());
				auto string_info = StringManager::GetInstance()->FindItemStringInfo(item->GetItemId());
				StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 45,
					static_cast<int>(_info.y + paddingsize + _shop_scroll->GetScrollY()) + 2, string_info.GetName().c_str(), 13, RGB(0, 0, 0));
				std::wstring str;
				str.append(std::to_wstring(shop_item->GetMeso())).append(L" 메소");
				StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 45,
					static_cast<int>(_info.y + paddingsize + _shop_scroll->GetScrollY()) + 21,
					str, 13, RGB(0, 0, 0));
				str.clear();
				str.append(std::to_wstring(shop_item->GetPrice()));
				StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 40,
					static_cast<int>(_info.y + paddingsize + _shop_scroll->GetScrollY()) + 21,
					str, 13, RGB(0, 0, 0), L"돋움", TA_RIGHT);
			}
		}
		paddingsize += _item_distance;
	}

}

void ShopWindow::SelectBuyItem(POINT mouse)
{
	auto paddingsize = 127;
	for (const auto shop_item : _shop_items)
	{
		auto item = ItemManager::GetInstance()->FindItem(shop_item->GetItemId());

		auto icon = item->GetIcon();
		if (icon)
		{

			if (static_cast<int>(_info.y) + paddingsize + _shop_scroll->GetScrollY() >= static_cast<int>(_info.y) + 127 &&
				static_cast<int>(_info.y) + paddingsize + _shop_scroll->GetScrollY() <= static_cast<int>(_info.y) + 287)
			{
				RECT icon_rect{ static_cast<int>(_info.x + 6),
					static_cast<int>(_info.y + paddingsize + _shop_scroll->GetScrollY()),
					static_cast<int>(_info.x + 6) + 198,
					static_cast<int>(_info.y + paddingsize + _shop_scroll->GetScrollY()) + icon->GetHeight() };
				if (PtInRect(&icon_rect, mouse))
				{
					_select_buy_item = shop_item;
					_select_item = nullptr;
					_select_eqp = nullptr;
				}
			}
		}
		paddingsize += _item_distance;
	}
}

void ShopWindow::RenderMeso(HDC hdc)
{
	auto player = MapManager::GetInstance()->GetPlayer();
	

	StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 445, static_cast<int>(_info.y) + 66,
		std::to_wstring(player->GetMeso()), 11, RGB(0, 0, 0), L"돋움", TA_RIGHT);
}

void ShopWindow::SelectItem(POINT mouse)
{
	if (_this_tab == ObjectType::InventoryTabState::kEqp)
	{
		auto player = MapManager::GetInstance()->GetPlayer();
		auto inventory = player->GetEqpInventory();
		auto item_list = inventory->GetItem();
		auto paddingsize = 127;
		for (int i = 0; i < inventory_slot_max; ++i)
		{
			if (item_list[i] != nullptr)
			{
				auto icon = item_list[i]->GetIcon();
				if (icon)
				{
					if (static_cast<int>(_info.y) + paddingsize + _inven_scroll->GetScrollY() >= static_cast<int>(_info.y) + 127 &&
						static_cast<int>(_info.y) + paddingsize + _inven_scroll->GetScrollY() <= static_cast<int>(_info.y) + 287)
					{
						RECT icon_rect{ static_cast<int>(_info.x + 237),
							static_cast<int>(_info.y + paddingsize + _inven_scroll->GetScrollY()),
							static_cast<int>(_info.x + 237) + 198,
							static_cast<int>(_info.y + paddingsize + _inven_scroll->GetScrollY()) + icon->GetHeight() };
						if (PtInRect(&icon_rect, mouse))
						{
							_select_eqp = item_list[i];
							_select_buy_item = nullptr;
							_select_item = nullptr;
						}
					}
				}
			}
			paddingsize += _item_distance;
		}
		return;
	}
	auto player = MapManager::GetInstance()->GetPlayer();
	auto inventory = player->GetInventory(_this_tab);
	auto item_list = inventory->GetItem();
	auto paddingsize = 127;
	for (int i = 0; i < inventory_slot_max; ++i)
	{
		if (item_list[i] != nullptr)
		{
			auto icon = item_list[i]->GetIcon();
			if (icon)
			{
				if (static_cast<int>(_info.y) + paddingsize + _inven_scroll->GetScrollY() >= static_cast<int>(_info.y) + 127 &&
					static_cast<int>(_info.y) + paddingsize + _inven_scroll->GetScrollY() <= static_cast<int>(_info.y) + 287)
				{
					RECT icon_rect{ static_cast<int>(_info.x + 237),
						static_cast<int>(_info.y + paddingsize + _inven_scroll->GetScrollY()),
						static_cast<int>(_info.x + 237) + 198,
						static_cast<int>(_info.y + paddingsize + _inven_scroll->GetScrollY()) + icon->GetHeight() };
					if (PtInRect(&icon_rect, mouse))
					{
						_select_item = item_list[i];
						_select_buy_item = nullptr;
						_select_eqp = nullptr;
					}
				}
			}
		}
		paddingsize += _item_distance;
	}
}

void ShopWindow::InvenScrollUp(POINT mouse)
{
	const RECT up_button{
		static_cast<int>(_info.x + 443),
		static_cast<int>(_info.y + 127),
		static_cast<int>(_info.x + 454),
		static_cast<int>(_info.y) + 138 };
	if (PtInRect(&up_button, mouse))
	{
		if (_inven_scroll->GetScrollY() != 0)
		{
			_inven_scroll->GainScrollY(static_cast<float>(_item_distance));
		}
	}
}

void ShopWindow::InvenScrollDown(POINT mouse)
{
	const RECT up_button{
		static_cast<int>(_info.x + 444),
		static_cast<int>(_info.y + 308),
		static_cast<int>(_info.x + 453),
		static_cast<int>(_info.y + 319) };

	if (PtInRect(&up_button, mouse))
	{
		const auto max_distance = _item_distance * 24;
		if (-max_distance != _inven_scroll->GetScrollY())
		{
			_inven_scroll->GainScrollY(static_cast<float>(-_item_distance));
		}
	}
}

void ShopWindow::ShopScrollUp(POINT mouse)
{
	const RECT up_button{
		static_cast<int>(_info.x + 211),
		static_cast<int>(_info.y + 127),
		static_cast<int>(_info.x + 222),
		static_cast<int>(_info.y) + 138 };
	if (PtInRect(&up_button, mouse))
	{
		if (_shop_scroll->GetScrollY() != 0)
		{
			_shop_scroll->GainScrollY(static_cast<float>(_item_distance));
		}
	}
}

void ShopWindow::ShopScrollDown(POINT mouse)
{
	const RECT up_button{
		static_cast<int>(_info.x + 211),
		static_cast<int>(_info.y + 308),
		static_cast<int>(_info.x + 222),
		static_cast<int>(_info.y + 319) };

	if (PtInRect(&up_button, mouse))
	{
		const auto max_distance = static_cast<int>(_item_distance * (_shop_items.size() - 5));
		if (-max_distance != _shop_scroll->GetScrollY())
		{
			_shop_scroll->GainScrollY(static_cast<float>(-_item_distance));
		}
	}
}

void ShopWindow::RenderShopButton(HDC hdc)
{
	_shop_buy_button->RenderButtonUi(hdc);
	_shop_sell_button->RenderButtonUi(hdc);
	_shop_exit_button->RenderButtonUi(hdc);
}

void ShopWindow::UpdateShopButton(POINT mouse)
{
	_shop_buy_button->UpdateButton();
	_shop_sell_button->UpdateButton();
	_shop_exit_button->UpdateButton();
}

void ShopWindow::RenderText(HDC hdc)
{
	if (GetTickCount64() < _log_render_tick + 1000)
	{
		StringTools::CreateTextOut(hdc, 400, 100, _log, 24, RGB(0, 0, 0), L"돋움", TA_CENTER);
	}
}

void ShopWindow::ResetShop()
{
	this->SetShow(false);
	this->SetTab(ObjectType::InventoryTabState::kEqp);
	_inven_scroll->SetScrollY(0);
	_inven_scroll->SetScrollX(0);
	_select_item = nullptr;
	_select_eqp = nullptr;
	_select_buy_item = nullptr;
}

void ShopWindow::CharacterInventoryItemRender(const HDC hdc)
{
	auto player = MapManager::GetInstance()->GetPlayer();
	if (_this_tab == ObjectType::InventoryTabState::kEqp)
	{
		auto inventory = player->GetEqpInventory();
		auto item_list = inventory->GetItem();
		auto paddingsize = 127;
		for (int i = 0; i < inventory_slot_max; ++i)
		{
			if (item_list[i] != nullptr)
			{
				auto icon = item_list[i]->GetIcon();
				if (icon)
				{
					if (static_cast<int>(_info.y) + paddingsize + _inven_scroll->GetScrollY() >= static_cast<int>(_info.y) + 127 &&
						static_cast<int>(_info.y) + paddingsize + _inven_scroll->GetScrollY() <= static_cast<int>(_info.y) + 287)
					{
						if (_select_eqp != nullptr && _select_eqp == item_list[i])
						{
							_select_image->RenderBitmapImage(hdc, static_cast<int>(_info.x) + 273,
								static_cast<int>(_info.y + paddingsize + _inven_scroll->GetScrollY()),
								_select_image->GetWidth(), _select_image->GetHeight());
						}
						icon->RenderBitmapImage(hdc,
							static_cast<int>(_info.x) + 237,
							static_cast<int>(_info.y + paddingsize + _inven_scroll->GetScrollY()),
							icon->GetWidth(),
							icon->GetHeight());
						auto string_info = StringManager::GetInstance()->FindItemStringInfo(item_list[i]->GetItemId());
						StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 237 + 40,
							static_cast<int>(_info.y + paddingsize + _inven_scroll->GetScrollY()) + 2, string_info.GetName().c_str(), 13, RGB(0, 0, 0));
						std::wstring str;
						str.append(std::to_wstring(item_list[i]->GetItemInfo().GetPrice())).append(L" 메소");
						StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 237 + 40,
							static_cast<int>(_info.y + paddingsize + _inven_scroll->GetScrollY()) + 21,
							str, 13, RGB(0, 0, 0));
					}
				}
			}
			paddingsize += _item_distance;
		}
		return;
	}
	auto inventory = player->GetInventory(_this_tab);
	auto item_list = inventory->GetItem();
	auto paddingsize = 127;
	for (int i = 0; i < inventory_slot_max; ++i)
	{
		if (item_list[i] != nullptr)
		{
			auto icon = item_list[i]->GetIcon();
			if (icon)
			{
				if (static_cast<int>(_info.y) + paddingsize + _inven_scroll->GetScrollY() >= static_cast<int>(_info.y) + 127 &&
					static_cast<int>(_info.y) + paddingsize + _inven_scroll->GetScrollY() <= static_cast<int>(_info.y) + 287)
				{
					if (_select_item != nullptr && _select_item == item_list[i])
					{
						_select_image->RenderBitmapImage(hdc, static_cast<int>(_info.x) + 273,
							static_cast<int>(_info.y + paddingsize + _inven_scroll->GetScrollY()),
							_select_image->GetWidth(), _select_image->GetHeight());
					}
					icon->RenderBitmapImage(hdc,
						static_cast<int>(_info.x) + 237,
						static_cast<int>(_info.y + paddingsize + _inven_scroll->GetScrollY()),
						icon->GetWidth(),
						icon->GetHeight());
					std::wstring quantity;
					quantity.append(std::to_wstring(item_list[i]->GetQuantity()));
					StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 237 + 15,
						static_cast<int>(_info.y + paddingsize + _inven_scroll->GetScrollY()) + 20, quantity, 13, RGB(0, 0, 0));
					auto string_info = StringManager::GetInstance()->FindItemStringInfo(item_list[i]->GetItemId());
					StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 237 + 40,
						static_cast<int>(_info.y + paddingsize + _inven_scroll->GetScrollY()) + 2, string_info.GetName().c_str(), 13, RGB(0, 0, 0));
					quantity.clear();
					quantity.append(std::to_wstring(item_list[i]->GetPrice())).append(L" 메소");
					StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 237 + 40,
						static_cast<int>(_info.y + paddingsize + _inven_scroll->GetScrollY()) + 21, 
						quantity, 13, RGB(0, 0, 0));
				}
			}
		}
		paddingsize += _item_distance;
	}
}

void ShopWindow::SetTab(const ObjectType::InventoryTabState tab)
{
	_this_tab = tab;
}

ObjectType::InventoryTabState ShopWindow::GetTab() const
{
	return _this_tab;
}

std::shared_ptr<Item> ShopWindow::GetSelectItem() const
{
	return _select_item;
}

std::shared_ptr<SkinInfo> ShopWindow::GetSelectEqp() const
{
	return _select_eqp;
}

std::shared_ptr<ShopItem> ShopWindow::GetSelectBuyItem() const
{
	return _select_buy_item;
}

void ShopWindow::SetSelectItem(std::shared_ptr<Item> item)
{
	_select_item = item;
}

void ShopWindow::SetSelectEqp(std::shared_ptr<SkinInfo> item)
{
	_select_eqp = item;
}

void ShopWindow::ExitShop()
{
	auto shop = UiManager::GetInstance()->GetShopWindow();
	shop->ResetShop();
}

void ShopWindow::SellItemShop()
{
	auto shop = UiManager::GetInstance()->GetShopWindow();
	auto string_manager = StringManager::GetInstance();
	auto player = MapManager::GetInstance()->GetPlayer();
	switch (shop->GetTab()) {
	case ObjectType::kEqp:
	{
		auto item = shop->GetSelectEqp();
		if (item != nullptr)
		{
			player->GetEqpInventory()->ReMoveItem(item);
			player->GainMeso(item->GetItemInfo().GetPrice());
			shop->SetSelectEqp(nullptr);
			shop->SetTextTick();
			shop->ClearText();
			shop->AppendText(string_manager->FindItemStringInfo(item->GetItemId()).GetName());
			shop->AppendText(L"를 팔았습니다.");
		}
		break;
	}
	case ObjectType::kConsume:
	case ObjectType::kInstall: 
	case ObjectType::kEtc: 
	case ObjectType::kCash:
	{
		auto item = shop->GetSelectItem();
		if (item != nullptr)
		{
			if(item->GetQuantity() > 0)
			{
				item->GainQuantity(-1);
				player->GainMeso(item->GetPrice());
				shop->SetTextTick();
				shop->ClearText();
				shop->AppendText(string_manager->FindItemStringInfo(item->GetItemId()).GetName());
				shop->AppendText(L"를 팔았습니다.");
			}
			else
			{
				shop->SetSelectItem(nullptr);
			}
		}
	}
		break;
	case ObjectType::kEnd: break;
	default: ;
	}
}

void ShopWindow::BuyItemShop()
{
	auto shop = UiManager::GetInstance()->GetShopWindow();
	auto string_manager = StringManager::GetInstance();
	auto player = MapManager::GetInstance()->GetPlayer();

	if (shop->GetSelectBuyItem() != nullptr)
	{
		if(player->GetMeso() >= shop->GetSelectBuyItem()->GetMeso())
		{
			auto item = ItemManager::GetInstance()->FindCopyItem(shop->GetSelectBuyItem()->GetItemId());
			item->SetQuantity(shop->GetSelectBuyItem()->GetPrice());
			player->GetInventory(ObjectType::kConsume)->AddItem(player->GetInventory(ObjectType::kConsume)->FindFreeSlot(), item);
			player->GainMeso(-shop->GetSelectBuyItem()->GetMeso());
			shop->SetTextTick();
			shop->ClearText();
			shop->AppendText(string_manager->FindItemStringInfo(item->GetItemId()).GetName());
			shop->AppendText(L"를 ");
			shop->AppendText(std::to_wstring(shop->GetSelectBuyItem()->GetPrice()));
			shop->AppendText(L"개 구입했습니다.");
		}
	}

}

void ShopWindow::LoadShopData(int32_t shop_id)
{
	auto shop_manager = ShopManager::GetInstance();
	auto shop = shop_manager->FindShop(shop_id);
	if (!shop.empty())
	{
		_shop_items = shop;
	}
}

void ShopWindow::SetTextTick()
{
	_log_render_tick = GetTickCount64();
}

void ShopWindow::AppendText(std::wstring str)
{
	_log.append(str);
}

void ShopWindow::ClearText()
{
	_log.clear();
}
