#include "../../pch.h"
#include "ui_manager.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"
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
	(*_hpBar)->RenderBitmapImage(hdc, 221, 582, 
		(*_hpBar)->GetWidth() - 50, 
		(*_hpBar)->GetHeight(), 
		(*_hpBar)->GetWidth() - 50, 
		(*_hpBar)->GetHeight());
	(*_mpBar)->RenderBitmapImage(hdc, 329, 582,
		(*_mpBar)->GetWidth(),
		(*_mpBar)->GetHeight(),
		(*_mpBar)->GetWidth(),
		(*_mpBar)->GetHeight());
	(*_expBar)->RenderBitmapImage(hdc, 442, 582, 
		(*_expBar)->GetWidth(), 
		(*_expBar)->GetHeight(), 
		(*_expBar)->GetWidth(), 
		(*_expBar)->GetHeight());
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
