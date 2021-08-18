#include "../../../pch.h"
#include "item_info_tool_tip.h"

#include "../../../Components/game_mouse.h"
#include "../../ScenManager/InGameScene/in_game_scene.h"
#include "../Inventory/inventory_window.h"
#include "../ui_manager.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../Components/MapObject/Item/item.h"
#include "../../Skins/skin_info.h"
#include "../../StringManager/string_manager.h"
#include "../../StringManager/string_Info.h"
#include "../Equip/equipment_window.h"

ItemInfoToolTip::ItemInfoToolTip() : count(1)
{
	auto hdc = GetDC(_hWnd);
	_memDC = CreateCompatibleDC(hdc);
	_bitmap = CreateCompatibleBitmap(hdc, 800, 600);
	_old_bitmap = static_cast<HBITMAP>(SelectObject(_memDC, _bitmap));

	_memDC2 = CreateCompatibleDC(hdc);
	_bitmap2 = CreateCompatibleBitmap(hdc, 800, 600);
	_old_bitmap2 = static_cast<HBITMAP>(SelectObject(_memDC2, _bitmap2));


	_memDC3 = CreateCompatibleDC(hdc);
	_bitmap3 = CreateCompatibleBitmap(hdc, 800, 600);
	_old_bitmap3 = static_cast<HBITMAP>(SelectObject(_memDC3, _bitmap3));
	ReleaseDC(_hWnd, hdc);
}

ItemInfoToolTip::~ItemInfoToolTip()
{
}

void ItemInfoToolTip::BaseToolToolTipRender(HDC hdc)
{
	const auto mouse = InGameScene::GetMouse();
	const auto ui_manager = UiManager::GetInstance();
	if (ui_manager->GetInventoryWindow()->IsShow())
	{
		if (ui_manager->GetInventoryWindow()->GetTab() != ObjectType::kEqp)
		{
			auto item = ui_manager->GetInventoryWindow()->PointCollisionItem(mouse->GetPoint());
			if (item != nullptr)
			{
				AlphaBlueScreenItemInfo(hdc);
				AlphaItemWhiteMiniScreen(hdc);
				SelectItemRedner(hdc, item);
			}
		}
		else
		{
			auto item = ui_manager->GetInventoryWindow()->PointCollisionEqp(mouse->GetPoint());
			if (item != nullptr)
			{
				AlphaBlueScreenEquipIcon(hdc);
				AlphaEquipWhiteMiniScreen(hdc);
				SelectEquipRedner(hdc, item);
				AlphaBlueScreenEquipInfo(hdc, item);
			}
		}
	}
	if (ui_manager->GetEquipmentWindow()->IsShow())
	{
		auto item = ui_manager->GetInventoryWindow()->PointCollisionEquipment(mouse->GetPoint());
		if (item != nullptr)
		{
			AlphaBlueScreenEquipIcon(hdc);
			AlphaEquipWhiteMiniScreen(hdc);
			SelectEquipRedner(hdc, item);
			AlphaBlueScreenEquipInfo(hdc, item);
		}
	}
}

void ItemInfoToolTip::AlphaBlueScreenItemInfo(HDC hdc) const
{
	const auto mouse = InGameScene::GetMouse();
	auto myBrush = static_cast<HBRUSH>(CreateSolidBrush(RGB(21, 16, 56)));
	auto oldBrush = static_cast<HBRUSH>(SelectObject(_memDC, myBrush));

	BLENDFUNCTION bf{ 0,0 , 175, 0 };
	Rectangle(_memDC,
		0,
		0,
		290,
		115);
	SelectObject(_memDC, oldBrush);
	DeleteObject(myBrush);


	BitBlt(_memDC2, 0, 0,
		290,
		115,
		hdc,
		static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx,
		static_cast<int>(mouse->GetInfo().y) + mouse->GetInfo().cy, SRCCOPY);

	GdiTransparentBlt(_memDC2,
		0,
		0,
		290,
		115,
		_memDC,
		0,
		0,
		290,
		115,
		RGB(255, 0, 255));

	GdiAlphaBlend(hdc,
		static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx,
		static_cast<int>(mouse->GetInfo().y) + mouse->GetInfo().cy,
		290,
		115,
		_memDC2,
		0,
		0,
		290,
		115, bf);

}

void ItemInfoToolTip::AlphaItemWhiteMiniScreen(HDC hdc) const
{
	const auto mouse = InGameScene::GetMouse();
	BLENDFUNCTION bf2{ 0,0 , 200, 0 };

	Rectangle(_memDC, 0,
		0,
		67,
		67);

	BitBlt(_memDC2, 0, 0,
		67,
		67,
		hdc,
		static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx,
		static_cast<int>(mouse->GetInfo().y) + mouse->GetInfo().cy, SRCCOPY);

	GdiTransparentBlt(_memDC2,
		0,
		0,
		67,
		67,
		_memDC,
		0,
		0,
		67,
		67,
		RGB(255, 0, 255));

	GdiAlphaBlend(hdc,
		static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx + 14,
		static_cast<int>(mouse->GetInfo().y) + mouse->GetInfo().cy + 32,
		67,
		67,
		_memDC2,
		0,
		0,
		67,
		67, bf2);
}

void ItemInfoToolTip::SelectItemRedner(HDC hdc, const std::shared_ptr<Item> item) const
{
	const auto mouse = InGameScene::GetMouse();
	const auto ui_manager = UiManager::GetInstance()->GetInventoryWindow();
	if (item != nullptr)
	{
		item->GetIcon()->RenderBitmapImage(hdc,
			static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx + 28,
			static_cast<int>(mouse->GetInfo().y) + mouse->GetInfo().cy + 45, 40, 40);
		auto string_info = StringManager::GetInstance()->FindItemStringInfo(item->GetItemId());

		StringTools::CreateTextOut(hdc,
			static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx + (290 >> 1),
			static_cast<int>(mouse->GetInfo().y) + mouse->GetInfo().cy + 10,
			string_info.GetName(), 14, RGB(255, 255, 255), L"돋움", TA_CENTER, true);

		auto desc = string_info.GetDesc();

		RECT rc{ static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx + 92,
				static_cast<int>(mouse->GetInfo().y) + mouse->GetInfo().cy + 38,
			static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx + 92 + 198,
				static_cast<int>(mouse->GetInfo().y) + mouse->GetInfo().cy + 38 + 115 };

		StringTools::CreateDrawText(hdc,
			rc,
			desc, 11, RGB(255, 255, 255), L"돋움");
	}
}

void ItemInfoToolTip::AlphaBlueScreenEquipIcon(HDC hdc) const
{
	const auto mouse = InGameScene::GetMouse();
	auto myBrush = static_cast<HBRUSH>(CreateSolidBrush(RGB(21, 16, 56)));
	auto oldBrush = static_cast<HBRUSH>(SelectObject(_memDC, myBrush));

	BLENDFUNCTION bf{ 0,0 , 175, 0 };
	Rectangle(_memDC,
		0,
		0,
		232,
		160 + 5);
	SelectObject(_memDC, oldBrush);
	DeleteObject(myBrush);


	BitBlt(_memDC2, 0, 0,
		232,
		160,
		hdc,
		static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx,
		static_cast<int>(mouse->GetInfo().y) + mouse->GetInfo().cy, SRCCOPY);

	GdiTransparentBlt(_memDC2,
		0,
		0,
		232,
		160,
		_memDC,
		0,
		0,
		232,
		160,
		RGB(255, 0, 255));

	GdiAlphaBlend(hdc,
		static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx,
		static_cast<int>(mouse->GetInfo().y) + mouse->GetInfo().cy,
		232,
		160,
		_memDC2,
		0,
		0,
		232,
		160, bf);

}

void ItemInfoToolTip::AlphaEquipWhiteMiniScreen(HDC hdc) const
{
	const auto mouse = InGameScene::GetMouse();
	BLENDFUNCTION bf2{ 0,0 , 200, 0 };

	Rectangle(_memDC, 0,
		0,
		67,
		67);

	BitBlt(_memDC2, 0, 0,
		67,
		67,
		hdc,
		static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx,
		static_cast<int>(mouse->GetInfo().y) + mouse->GetInfo().cy, SRCCOPY);

	GdiTransparentBlt(_memDC2,
		0,
		0,
		67,
		67,
		_memDC,
		0,
		0,
		67,
		67,
		RGB(255, 0, 255));

	GdiAlphaBlend(hdc,
		static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx + 14,
		static_cast<int>(mouse->GetInfo().y) + mouse->GetInfo().cy + 32,
		67,
		67,
		_memDC2,
		0,
		0,
		67,
		67, bf2);
}

void ItemInfoToolTip::SelectEquipRedner(HDC hdc, const std::shared_ptr<SkinInfo> item) const
{
	const auto mouse = InGameScene::GetMouse();
	const auto ui_manager = UiManager::GetInstance()->GetEquipmentWindow();
	if (item != nullptr)
	{
		item->GetIcon()->RenderBitmapImage(hdc,
			static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx + 28,
			static_cast<int>(mouse->GetInfo().y) + mouse->GetInfo().cy + 45, 40, 40);
		auto string_info = StringManager::GetInstance()->FindItemStringInfo(item->GetItemId());

		std::wstring item_name(string_info.GetName());
		if (item->GetItemInfo().GetSucessTuc() > 0)
		{
			item_name.append(L" (+").append(std::to_wstring(item->GetItemInfo().GetSucessTuc())).append(L")");
		}
		StringTools::CreateTextOut(hdc,
			static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx + (232 >> 1),
			static_cast<int>(mouse->GetInfo().y) + mouse->GetInfo().cy + 10,
			item_name, 14, RGB(255, 255, 255), L"돋움", TA_CENTER, true);

		/*std::wstring list[8]
		{
			L"REQ LEV : " , L"REQ STR : " , L"REQ DEX : " , L"REQ INT : ",
			L"REQ LUK : ", L"REQ POP : 0", L"ITEM LEV : 0", L"ITEM EXP : 0"
		};*/
		//for (int i = 0; i < 8; ++i)
		//{
		//	StringTools::CreateTextOut(hdc,
		//		static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx + 14 + 75,
		//		static_cast<int>(mouse->GetInfo().y + 14 + (i * 10)) + mouse->GetInfo().cy + 20,
		//		list[i], 9, RGB(255, 255, 255), L"돋움");
		//}
		std::wstring req_info;
		req_info.append(L"REQ LEV : ").append(std::to_wstring(item->GetItemInfo().GetReqLevel()));
		StringTools::CreateTextOut(hdc,
			static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx + 14 + 75,
			static_cast<int>(mouse->GetInfo().y) + 14 + mouse->GetInfo().cy + 20,
			req_info.c_str(), 9, RGB(255, 255, 255), L"돋움");
		req_info.clear();

		req_info.append(L"REQ STR : ").append(std::to_wstring(item->GetItemInfo().GetReqStr()));
		StringTools::CreateTextOut(hdc,
			static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx + 14 + 75,
			static_cast<int>(mouse->GetInfo().y) + 24 + mouse->GetInfo().cy + 20,
			req_info.c_str(), 9, RGB(255, 255, 255), L"돋움");
		req_info.clear();

		req_info.append(L"REQ DEX : ").append(std::to_wstring(item->GetItemInfo().GetReqDex()));
		StringTools::CreateTextOut(hdc,
			static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx + 14 + 75,
			static_cast<int>(mouse->GetInfo().y) + 34 + mouse->GetInfo().cy + 20,
			req_info.c_str(), 9, RGB(255, 255, 255), L"돋움");
		req_info.clear();

		req_info.append(L"REQ INT : ").append(std::to_wstring(item->GetItemInfo().GetReqInt()));
		StringTools::CreateTextOut(hdc,
			static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx + 14 + 75,
			static_cast<int>(mouse->GetInfo().y) + 44 + mouse->GetInfo().cy + 20,
			req_info.c_str(), 9, RGB(255, 255, 255), L"돋움");
		req_info.clear();

		req_info.append(L"REQ LUK : ").append(std::to_wstring(item->GetItemInfo().GetReqLuk()));
		StringTools::CreateTextOut(hdc,
			static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx + 14 + 75,
			static_cast<int>(mouse->GetInfo().y) + 54 + mouse->GetInfo().cy + 20,
			req_info.c_str(), 9, RGB(255, 255, 255), L"돋움");
		req_info.clear();

		req_info.append(L"REQ POP : 0");
		StringTools::CreateTextOut(hdc,
			static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx + 14 + 75,
			static_cast<int>(mouse->GetInfo().y) + 64 + mouse->GetInfo().cy + 20,
			req_info.c_str(), 9, RGB(255, 255, 255), L"돋움");
		req_info.clear();

		req_info.append(L"ITEM LEV : 0");
		StringTools::CreateTextOut(hdc,
			static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx + 14 + 75,
			static_cast<int>(mouse->GetInfo().y) + 74 + mouse->GetInfo().cy + 20,
			req_info.c_str(), 9, RGB(255, 255, 255), L"돋움");
		req_info.clear();

		req_info.append(L"ITEM EXP : 0");
		StringTools::CreateTextOut(hdc,
			static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx + 14 + 75,
			static_cast<int>(mouse->GetInfo().y) + 84 + mouse->GetInfo().cy + 20,
			req_info.c_str(), 9, RGB(255, 255, 255), L"돋움");
		req_info.clear();





		StringTools::CreateTextOut(hdc,
			static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx + (232 >> 1),
			static_cast<int>(mouse->GetInfo().y + 110) + mouse->GetInfo().cy + 20,
			L"초보자  전사  마법사  궁수  도적  해적", 12, RGB(255, 255, 255), L"돋움", TA_CENTER);

		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		HPEN hOldPen = static_cast<HPEN>(SelectObject(hdc, hPen));
		MoveToEx(hdc, static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx + 5,
			static_cast<int>(mouse->GetInfo().y + 150) + mouse->GetInfo().cy, nullptr);
		LineTo(hdc, static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx + 5 + 220,
			static_cast<int>(mouse->GetInfo().y + 150) + mouse->GetInfo().cy);
		SelectObject(hdc, hOldPen);
		DeleteObject(hPen);
	}
}

void ItemInfoToolTip::AlphaBlueScreenEquipInfo(HDC hdc, const std::shared_ptr<SkinInfo> item) const
{
	const auto mouse = InGameScene::GetMouse();
	auto info_str_pair = SelectEquipInfo(item);

	auto myBrush = static_cast<HBRUSH>(CreateSolidBrush(RGB(21, 16, 56)));
	auto oldBrush = static_cast<HBRUSH>(SelectObject(_memDC, myBrush));

	int32_t height = 13 * info_str_pair.first;
	BLENDFUNCTION bf{ 0,0 , 175, 0 };
	Rectangle(_memDC,
		0,
		-30,
		232,
		height);
	SelectObject(_memDC, oldBrush);
	DeleteObject(myBrush);


	BitBlt(_memDC2, 0, 0,
		232,
		height,
		hdc,
		static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx,
		static_cast<int>(mouse->GetInfo().y) + mouse->GetInfo().cy + height, SRCCOPY);

	GdiTransparentBlt(_memDC2,
		0,
		0,
		232,
		height,
		_memDC,
		0,
		0,
		232,
		height,
		RGB(255, 0, 255));

	GdiAlphaBlend(hdc,
		static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx,
		static_cast<int>(mouse->GetInfo().y) + mouse->GetInfo().cy + 160,
		232,
		height,
		_memDC2,
		0,
		0,
		232,
		height, bf);
	RECT rc {
		static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx + (232 >> 1) - 1,
		static_cast<int>(mouse->GetInfo().y) + mouse->GetInfo().cy + 160,
		static_cast<int>(mouse->GetInfo().x) + mouse->GetInfo().cx + (232 >> 1) + 1,
		static_cast<int>(mouse->GetInfo().y) + mouse->GetInfo().cy + 160 + height };
    //Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
	StringTools::CreateDrawText(hdc, rc, info_str_pair.second, 11, RGB(255,255,255), L"돋움", true);
}

std::pair<int32_t, std::wstring> ItemInfoToolTip::SelectEquipInfo(const std::shared_ptr<SkinInfo> item) const
{
	std::wstring str;
	int32_t count = 0;
	if (item->GetItemInfo().GetIncStr() > 0)
	{
		str.append(L"STR +").append(std::to_wstring(item->GetItemInfo().GetIncStr())).append(L"\r\n");
		count++;
	}

	if (item->GetItemInfo().GetIncDex() > 0)
	{
		str.append(L"DEX +").append(std::to_wstring(item->GetItemInfo().GetIncDex())).append(L"\r\n");
		count++;
	}
	

	if (item->GetItemInfo().GetIncInt() > 0)
	{
		str.append(L"INT +").append(std::to_wstring(item->GetItemInfo().GetIncInt())).append(L"\r\n");
		count++;
	}
	

	if (item->GetItemInfo().GetIncLuk() > 0)
	{
		str.append(L"LUK +").append(std::to_wstring(item->GetItemInfo().GetIncLuk())).append(L"\r\n");
		count++;
	}
	

	if (item->GetItemInfo().GetIncPad() > 0)
	{
		str.append(L"공격력 +").append(std::to_wstring(item->GetItemInfo().GetIncPad())).append(L"\r\n");
		count++;
	}
	

	if (item->GetItemInfo().GetIncMad() > 0)
	{
		str.append(L"마법 공격력 +").append(std::to_wstring(item->GetItemInfo().GetIncMad())).append(L"\r\n");
		count++;
	}
	

	if (item->GetItemInfo().GetIncPdd() > 0)
	{
		str.append(L"물리 방어력 +").append(std::to_wstring(item->GetItemInfo().GetIncPdd())).append(L"\r\n");
		count++;
	}
	

	if (item->GetItemInfo().GetIncMdd() > 0)
	{
		str.append(L"마법 방어력 +").append(std::to_wstring(item->GetItemInfo().GetIncMdd()));
		count++;
	}

	str.append(L"업그레이드 남은 횟수 : ").append(std::to_wstring(item->GetItemInfo().GetTuc()));
	count++;
	//str.append(L"\r\n보스 몬스터 공격 시 데미지 : + 40%\r\n");
	//str.append(L"보스 몬스터 공격 시 데미지 : + 40%\r\n");
	//str.append(L"보스 몬스터 공격 시 데미지 : + 40%\r\n");
	//count += 4;

	return std::make_pair(count, str);
}
