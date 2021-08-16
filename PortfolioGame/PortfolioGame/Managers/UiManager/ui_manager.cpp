#include "../../pch.h"
#include "ui_manager.h"
#include "ui_button.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../Components/MapObject/Player/player.h"
#include "../MapManager/map_manager.h"
#include "Inventory/inventory_window.h"
#include "QuickSlot/quick_slot.h"
#include "Skill/skill_window.h"
#include "Equip/equipment_window.h"
#include "ToolTipUi/item_info_tool_tip.h"
#include "Stat/stat_window.h"

const std::string buttons[] = { "BtMenu", "BtShop", "BtShort", "EquipKey", "StatKey", "InvenKey",
						  "KeySet", "QuickSlot", "QuickSlotD", "SkillKey" };
const std::wstring wbuttons[] = { L"BtMenu", L"BtShop", L"BtShort", L"EquipKey", L"StatKey", L"InvenKey",
						  L"KeySet", L"QuickSlot", L"QuickSlotD", L"SkillKey" };

UiManager::UiManager() = default;

UiManager::~UiManager() = default;

UiManager* UiManager::GetInstance()
{
	static UiManager instance;
	return &instance;
}

void UiManager::UpdateUiManager()
{
	for (auto buttonName : buttons)
	{
		auto button = _list_button.find(buttonName);
		if (button != _list_button.end())
		{
			button->second->UpdateButton();
		}
	}
	_skill_window->UpdateWindow();
	_inventory_window->DoUpdateWindow();
	_equipment_window->DoUpdateWindow();
	_stat_window->DoUpdateWindow();
}

void UiManager::ReadyUiManager()
{
	ButtonUiLoad();
	PlayerInfoUiLoad();
	_skill_window = std::make_shared<SkillWindow>(SkillWindow());
	_skill_window->ReadyWindow();
	_inventory_window = std::make_shared<InventoryWindow>(InventoryWindow());
	_inventory_window->DoReadyWindow();
	_equipment_window = std::make_shared<EquipmentWindow>(EquipmentWindow());
	_equipment_window->DoReadyWindow();
	_stat_window = std::make_shared<StatWindow>(StatWindow());
	_stat_window->DoReadyWindow();
	_item_info_tool_tip = std::make_shared<ItemInfoToolTip>(ItemInfoToolTip());
}

void UiManager::RenderUiManager(HDC hdc)
{
	_quick_slot->RenderQuickSlot(hdc);
	_status_bar->RenderBitmapImage(hdc, 0, 529, _status_bar->GetWidth(), _status_bar->GetHeight());

	for (auto buttonName : buttons)
	{
		auto button = _list_button.find(buttonName);
		if (button != _list_button.end())
		{
			button->second->RenderButtonUi(hdc);
		}
	}

	StatusGageBarRender(hdc);
	StatusLevelRender(hdc);

	_skill_window->RenderWindow(hdc);
	_inventory_window->DoRenderWindow(hdc);
	_equipment_window->DoRenderWindow(hdc);
	_stat_window->DoRenderWindow(hdc);
	_item_info_tool_tip->BaseToolToolTipRender(hdc);
}

void UiManager::ButtonUiLoad()
{
	std::shared_ptr<MyBitmap> backgrnd = std::make_shared<MyBitmap>(MyBitmap());
	backgrnd->Insert_Bitmap(_hWnd, L"Client\\Ui\\StatusBar\\backgrnd.bmp");
	_status_bar = backgrnd;
	_quick_slot = std::make_shared<QuickSlot>(QuickSlot());
	_quick_slot->ReadyQuickSlot();
	for (auto buttonName : wbuttons)
	{
		wchar_t path[256];
		swprintf_s(path, 256, L"StatusBar\\%s", buttonName.c_str());
		std::shared_ptr<UiButton> button(std::make_shared<UiButton>(UiButton()));
		if (!wcscmp(buttonName.c_str(), L"BtShop"))
		{
			button->SetObjectPos({ 575, 565 });
		}
		else if (!wcscmp(buttonName.c_str(), L"BtMenu"))
		{
			button->SetObjectPos({ 651, 565 });
		}
		else if (!wcscmp(buttonName.c_str(), L"BtShort"))
		{
			button->SetObjectPos({ 726, 565 });
		}
		else if (!wcscmp(buttonName.c_str(), L"EquipKey"))
		{
			button->SetObjectPos({ 618, 537 });
		}
		else if (!wcscmp(buttonName.c_str(), L"InvenKey"))
		{
			button->SetObjectPos({ 648, 537 });
		}
		else if (!wcscmp(buttonName.c_str(), L"KeySet"))
		{
			button->SetObjectPos({ 738, 537 });
		}
		else if (!wcscmp(buttonName.c_str(), L"QuickSlotD"))
		{
			button->SetObjectPos({ 768, 537 });
		}
		else if (!wcscmp(buttonName.c_str(), L"QuickSlot"))
		{
			button->SetObjectPos({ 768, 537 });
		}
		else if (!wcscmp(buttonName.c_str(), L"SkillKey"))
		{
			button->SetObjectPos({ 708, 537 });
		}
		else if (!wcscmp(buttonName.c_str(), L"StatKey"))
		{
			button->SetObjectPos({ 678, 537 });
		}

		button->ReadyButton(path);

		_list_button.insert({ StringTools::WStringToString(buttonName.c_str()), button });

	}

}

void UiManager::PlayerInfoUiLoad()
{

	for (int i = 0; i < 10; i++)
	{
		std::shared_ptr<MyBitmap> backgrnd = std::make_shared<MyBitmap>(MyBitmap());
		wchar_t path[256];
		swprintf_s(path, 256, L"Client\\Ui\\StatusBar\\LevelNo.%d.bmp", i);
		backgrnd->Insert_Bitmap(_hWnd, path);
		
		_list_level.insert({i, backgrnd });
	}
	std::shared_ptr<MyBitmap> hpBar = std::make_shared<MyBitmap>(MyBitmap());
	hpBar->Insert_Bitmap(_hWnd, L"Client\\Ui\\StatusBar\\hpBar.bmp");
	std::shared_ptr<MyBitmap> mpBar = std::make_shared<MyBitmap>(MyBitmap());
	mpBar->Insert_Bitmap(_hWnd, L"Client\\Ui\\StatusBar\\mpBar.bmp");
	std::shared_ptr<MyBitmap> expBar = std::make_shared<MyBitmap>(MyBitmap());
	expBar->Insert_Bitmap(_hWnd, L"Client\\Ui\\StatusBar\\expBar.bmp");
	_hp_bar = hpBar;
	_mp_bar = mpBar;
	_exp_bar = expBar;
}

void UiManager::StatusGageBarRender(HDC hdc)
{
	Player* player = MapManager::GetInstance()->GetPlayer();
	std::wstring text;
	text.append(std::to_wstring(player->GetPlayerInfo()->hp))
		.append(L"/").append(std::to_wstring(player->GetPlayerInfo()->max_hp));

	StringTools::CreateTextOut(hdc, 238, 569, L"[", 11, RGB(153, 204, 51));
	StringTools::CreateTextOut(hdc, 238 + 5, 569, text.c_str());
	StringTools::CreateTextOut(hdc, 238 + 5 + (static_cast<int>(text.size()) * 6), 569, L"]", 11, RGB(153, 204, 51));

	const float hpPercent =
		static_cast<float>(player->GetPlayerInfo()->hp) / static_cast<float>(player->GetPlayerInfo()->max_hp) * 100.f;
	int hpWidth = static_cast<int>(_hp_bar->GetWidth() * hpPercent / 100);
	_hp_bar->RenderBitmapImage(hdc, 221, 582,
		hpWidth,
		_hp_bar->GetHeight(),
		hpWidth,
		_hp_bar->GetHeight());

	text.clear();
	text.append(std::to_wstring(player->GetPlayerInfo()->mp))
		.append(L"/").append(std::to_wstring(player->GetPlayerInfo()->max_mp));

	StringTools::CreateTextOut(hdc, 348, 569, L"[", 11, RGB(153, 204, 51));
	StringTools::CreateTextOut(hdc, 348 + 5, 569, text.c_str());
	StringTools::CreateTextOut(hdc, 348 + 5 + (static_cast<int>(text.size()) * 6), 569, L"]", 11, RGB(153, 204, 51));

	const float mpPercent =
		static_cast<float>(player->GetPlayerInfo()->mp) / static_cast<float>(player->GetPlayerInfo()->max_mp) * 100.f;
	const int mpWidth = static_cast<int>(_mp_bar->GetWidth() * mpPercent / 100);
	_mp_bar->RenderBitmapImage(hdc, 329, 582,
		mpWidth,
		_mp_bar->GetHeight(),
		mpWidth,
		_mp_bar->GetHeight());

	const float expPercent =
		static_cast<float>(player->GetPlayerInfo()->exp) / 10000 * 100.f;
	const int expWidth = static_cast<int>(_mp_bar->GetWidth() * expPercent / 100);

	text.clear();
	text.append(std::to_wstring(player->GetPlayerInfo()->exp));

	int textWidth = static_cast<int>(text.size()) * 6;
	StringTools::CreateTextOut(hdc, 468, 569, text.c_str());
	StringTools::CreateTextOut(hdc, 468 + textWidth, 569, L"[", 11, RGB(153, 204, 51));
	wchar_t str[10];
	swprintf_s(str, 10, L"%0.2f", expPercent);
	text.clear();
	text.append(str).append(L"%");
	StringTools::CreateTextOut(hdc, 468 + 5 + textWidth, 569, text.c_str());
	textWidth += 5;
	StringTools::CreateTextOut(hdc, 468 + textWidth + (static_cast<int>(text.size()) * 6), 569, L"]", 11, RGB(153, 204, 51));

	_exp_bar->RenderBitmapImage(hdc, 442, 582,
		expWidth,
		_exp_bar->GetHeight(),
		expWidth,
		_exp_bar->GetHeight());


	text.clear();
	text.append(StringTools::StringToWString(player->GetPlayerInfo()->name.c_str()));
	StringTools::CreateTextOut(hdc, 89, 584, text.c_str(), 13, RGB(255, 255, 255), L"µ¸¿ò");

	text.clear();
	text.append(L"¸¶¹ý»ç");
	StringTools::CreateTextOut(hdc, 89, 568, text.c_str(), 13, RGB(255, 255, 255), L"µ¸¿ò");
}

void UiManager::StatusLevelRender(HDC hdc)
{
	Player* player = MapManager::GetInstance()->GetPlayer();
	uint8_t level = player->GetPlayerInfo()->level;
	
	std::vector<uint8_t> nums;
	while (level != 0) {
		nums.push_back(level % 10);
		level /= 10;
	}
	int x = 40;
	for (auto level = nums.rbegin(); level != nums.rend(); ++level)
	{
		auto data = _list_level.find(*level)->second;
		data->RenderBitmapImage(hdc, x, 576, data->GetWidth(), data->GetHeight());
		x += 12;
	}
}

std::shared_ptr<QuickSlot> UiManager::GetQuickSlot() const
{
	return _quick_slot;
}

std::shared_ptr<InventoryWindow> UiManager::GetInventoryWindow() const
{
	return _inventory_window;
}

std::shared_ptr<EquipmentWindow> UiManager::GetEquipmentWindow() const
{
	return _equipment_window;
}

std::shared_ptr<StatWindow> UiManager::GetStatWindow() const
{
	return _stat_window;
}
