#include "../../pch.h"
#include "ui_manager.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../MapManager/map_manager.h"
#include "../../Components/MapObject/player.h"
#include "ui_button.h"

std::string buttons[] = { "BtMenu", "BtShop", "BtShort", "EquipKey", "StatKey", "InvenKey",
						  "KeySet", "QuickSlot", "QuickSlotD", "SkillKey" };
std::wstring wbuttons[] = { L"BtMenu", L"BtShop", L"BtShort", L"EquipKey", L"StatKey", L"InvenKey",
						  L"KeySet", L"QuickSlot", L"QuickSlotD", L"SkillKey" };

void UiManager::UpdateUiManager()
{
	for (auto buttonName : buttons)
	{
		auto button = _listButton.find(buttonName);
		if (button != _listButton.end())
		{
			(*button->second)->UpdateButton();
		}
	}
}

void UiManager::ReadyUiManager()
{
	ButtonUiLoad();
	PlayerInfoUiLoad();
}

void UiManager::RednerUiManager(HDC hdc)
{
	(*_quickSlot)->RenderBitmapImage(hdc, 647, 453, (*_quickSlot)->GetWidth(), (*_quickSlot)->GetHeight());
	(*_statusBar)->RenderBitmapImage(hdc, 0, 529, (*_statusBar)->GetWidth(), (*_statusBar)->GetHeight());

	for (auto buttonName : buttons)
	{
		auto button = _listButton.find(buttonName);
		if (button != _listButton.end())
		{
			(*button->second)->RenderButtonUi(hdc);
		}
	}

	StatusGageBarRender(hdc);
	StatusLevelRender(hdc);

}

void UiManager::ButtonUiLoad()
{
	std::shared_ptr<MyBitmap*> backgrnd = std::make_shared<MyBitmap*>(new MyBitmap());
	(*backgrnd)->Insert_Bitmap(_hWnd, L"Client\\Ui\\StatusBar\\backgrnd.bmp");
	_statusBar = backgrnd;
	std::shared_ptr<MyBitmap*> quickSlot = std::make_shared<MyBitmap*>(new MyBitmap());
	(*quickSlot)->Insert_Bitmap(_hWnd, L"Client\\Ui\\StatusBar\\quickSlot.bmp");
	_quickSlot = quickSlot;

	for (auto buttonName : wbuttons)
	{
		wchar_t path[256];
		swprintf_s(path, 256, L"StatusBar\\%s", buttonName.c_str());
		std::shared_ptr<UiButton*> button(std::make_shared<UiButton*>(new UiButton()));
		if (!wcscmp(buttonName.c_str(), L"BtShop"))
		{
			(*button)->SetObjectPos({ 575, 565 });
		}
		else if (!wcscmp(buttonName.c_str(), L"BtMenu"))
		{
			(*button)->SetObjectPos({ 651, 565 });
		}
		else if (!wcscmp(buttonName.c_str(), L"BtShort"))
		{
			(*button)->SetObjectPos({ 726, 565 });
		}
		else if (!wcscmp(buttonName.c_str(), L"EquipKey"))
		{
			(*button)->SetObjectPos({ 618, 537 });
		}
		else if (!wcscmp(buttonName.c_str(), L"InvenKey"))
		{
			(*button)->SetObjectPos({ 648, 537 });
		}
		else if (!wcscmp(buttonName.c_str(), L"KeySet"))
		{
			(*button)->SetObjectPos({ 738, 537 });
		}
		else if (!wcscmp(buttonName.c_str(), L"QuickSlotD"))
		{
			(*button)->SetObjectPos({ 768, 537 });
		}
		else if (!wcscmp(buttonName.c_str(), L"QuickSlot"))
		{
			(*button)->SetObjectPos({ 768, 537 });
		}
		else if (!wcscmp(buttonName.c_str(), L"SkillKey"))
		{
			(*button)->SetObjectPos({ 708, 537 });
		}
		else if (!wcscmp(buttonName.c_str(), L"StatKey"))
		{
			(*button)->SetObjectPos({ 678, 537 });
		}

		(*button)->ReadyButton(path);

		_listButton.insert({ StringTools::WStringToString(buttonName.c_str()), button });

	}

}

void UiManager::PlayerInfoUiLoad()
{

	for (int i = 0; i < 10; i++)
	{
		std::shared_ptr<MyBitmap*> backgrnd = std::make_shared<MyBitmap*>(new MyBitmap());
		wchar_t path[256];
		swprintf_s(path, 256, L"Client\\Ui\\StatusBar\\LevelNo.%d.bmp", i);
		(*backgrnd)->Insert_Bitmap(_hWnd, path);
		
		_listLevel.insert({i, backgrnd });
	}
	std::shared_ptr<MyBitmap*> hpBar = std::make_shared<MyBitmap*>(new MyBitmap());
	(*hpBar)->Insert_Bitmap(_hWnd, L"Client\\Ui\\StatusBar\\hpBar.bmp");
	std::shared_ptr<MyBitmap*> mpBar = std::make_shared<MyBitmap*>(new MyBitmap());
	(*mpBar)->Insert_Bitmap(_hWnd, L"Client\\Ui\\StatusBar\\mpBar.bmp");
	std::shared_ptr<MyBitmap*> expBar = std::make_shared<MyBitmap*>(new MyBitmap());
	(*expBar)->Insert_Bitmap(_hWnd, L"Client\\Ui\\StatusBar\\expBar.bmp");
	_hpBar = hpBar;
	_mpBar = mpBar;
	_expBar = expBar;
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

	float hpPercent =
		static_cast<float>(player->GetPlayerInfo()->hp) / static_cast<float>(player->GetPlayerInfo()->max_hp) * 100.f;
	int hpWidth = static_cast<int>((*_hpBar)->GetWidth() * hpPercent / 100);
	(*_hpBar)->RenderBitmapImage(hdc, 221, 582,
		hpWidth,
		(*_hpBar)->GetHeight(),
		hpWidth,
		(*_hpBar)->GetHeight());

	text.clear();
	text.append(std::to_wstring(player->GetPlayerInfo()->mp))
		.append(L"/").append(std::to_wstring(player->GetPlayerInfo()->max_mp));

	StringTools::CreateTextOut(hdc, 348, 569, L"[", 11, RGB(153, 204, 51));
	StringTools::CreateTextOut(hdc, 348 + 5, 569, text.c_str());
	StringTools::CreateTextOut(hdc, 348 + 5 + (static_cast<int>(text.size()) * 6), 569, L"]", 11, RGB(153, 204, 51));

	float mpPercent =
		static_cast<float>(player->GetPlayerInfo()->mp) / static_cast<float>(player->GetPlayerInfo()->max_mp) * 100.f;
	int mpWidth = static_cast<int>((*_mpBar)->GetWidth() * mpPercent / 100);
	(*_mpBar)->RenderBitmapImage(hdc, 329, 582,
		mpWidth,
		(*_mpBar)->GetHeight(),
		mpWidth,
		(*_mpBar)->GetHeight());

	float expPercent =
		static_cast<float>(player->GetPlayerInfo()->exp) / 10000 * 100.f;
	int expWidth = static_cast<int>((*_mpBar)->GetWidth() * expPercent / 100);

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

	(*_expBar)->RenderBitmapImage(hdc, 442, 582,
		expWidth,
		(*_expBar)->GetHeight(),
		expWidth,
		(*_expBar)->GetHeight());


	text.clear();
	text.append(StringTools::StringToWString(player->GetPlayerInfo()->name.c_str()).c_str());
	StringTools::CreateTextOut(hdc, 89, 584, text.c_str(), 13, RGB(255, 255, 255), L"µ¸¿ò");

	text.clear();
	text.append(L"ÃÊº¸ÀÚ");
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
		auto data = _listLevel.find(*level)->second;
		(*data)->RenderBitmapImage(hdc, x, 576, (*data)->GetWidth(), (*data)->GetHeight());
		x += 12;
	}
}
