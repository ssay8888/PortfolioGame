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
	UiLoad();
}

void UiManager::RednerUiManager(HDC hdc)
{
	(*_statusBar)->RenderBitmapImage(hdc, 0, 529, (*_statusBar)->GetWidth(), (*_statusBar)->GetHeight());
	(*_quickSlot)->RenderBitmapImage(hdc, 647, 453, (*_quickSlot)->GetWidth(), (*_quickSlot)->GetHeight());

	for (auto buttonName : buttons)
	{
		auto button = _listButton.find(buttonName);
		if (button != _listButton.end())
		{
			(*button->second)->RenderButtonUi(hdc);
		}
	}
}

void UiManager::UiLoad()
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
