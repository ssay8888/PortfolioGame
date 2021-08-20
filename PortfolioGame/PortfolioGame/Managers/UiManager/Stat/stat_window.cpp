#include "../../../pch.h"
#include "stat_window.h"
#include "../ui_button.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../../Common/Utility/string_tools.h"
#include "../../../Components/game_mouse.h"
#include "../../../Components/MapObject/Player/player.h"
#include "../../KeyManaer/key_manager.h"
#include "../../MapManager/map_manager.h"
#include "../../ScenManager/InGameScene/in_game_scene.h"
#include "../../UiManager/ui_manager.h"

StatWindow::StatWindow(): _show_background2(false),
                          _key_manager2(nullptr)
{
}

StatWindow::~StatWindow()
{

	if (_key_manager2 != nullptr)
	{
		delete _key_manager2;
	}

}

void StatWindow::StrUp()
{
	const auto player = MapManager::GetInstance()->GetPlayer();
	if (player->GetAp() > 0)
	{
		player->GainAp(-1);
		player->GainStr(1);
		player->RecalcEqpStat();
	}
}

void StatWindow::DexUp()
{
	const auto player = MapManager::GetInstance()->GetPlayer();
	if (player->GetAp() > 0)
	{
		player->GainAp(-1);
		player->GainDex(1);
		player->RecalcEqpStat();
	}
}

void StatWindow::IntUp()
{
	const auto player = MapManager::GetInstance()->GetPlayer();
	if (player->GetAp() > 0)
	{
		player->GainAp(-1);
		player->GainInt(1);
		player->RecalcEqpStat();
	}
}

void StatWindow::LukUp()
{
	const auto player = MapManager::GetInstance()->GetPlayer();
	if (player->GetAp() > 0)
	{
		player->GainAp(-1);
		player->GainLuk(1);
		player->RecalcEqpStat();
	}
}

void StatWindow::DetailBackGroundSlide()
{
	const auto stat_window = UiManager::GetInstance()->GetStatWindow();
	stat_window->SetShowBackGround2(!stat_window->IsShowBackGround2());
}

void StatWindow::SetShowBackGround2(const bool show)
{
	_show_background2 = show;
}

bool StatWindow::IsShowBackGround2() const
{
	return _show_background2;
}

void StatWindow::ReadyWindow()
{
	_background = std::make_shared<MyBitmap>(MyBitmap());
	_background->Insert_Bitmap(_hWnd, L"Client\\Ui\\Stat.img\\Stat.backgrnd.bmp");
	_info.cx = _background->GetWidth();
	_info.cy = _background->GetHeight();
	_background2 = std::make_shared<MyBitmap>(MyBitmap());
	_background2->Insert_Bitmap(_hWnd, L"Client\\Ui\\Stat.img\\Stat.backgrnd2.bmp");

	_key_manager = new KeyManager();
	_key_manager2 = new KeyManager();

	for (int i = 0; i < 4; ++i)
	{
		auto button = std::make_shared<UiButton>(UiButton(_key_manager2));
		button->SetObjectPos({ 152, float(237 + (18 * i)) });
		switch (i)
		{
		case 0:
			button->SetCallBack(StrUp);
			break;
		case 1:
			button->SetCallBack(DexUp);
			break;
		case 2:
			button->SetCallBack(IntUp);
			break;
		case 3:
			button->SetCallBack(LukUp);
			break;
		}
		
		button->ReadyButton(L"Stat.img\\Stat.BtApUp");
		_ap_up_button[i] = button;
	}
	auto button = std::make_shared<UiButton>(UiButton(_key_manager2));
	button->SetObjectPos({ 98, 187 });
	button->ReadyButton(L"Stat.img\\Stat.BtAuto");
	_ap_up_auto_button = button;

	button = std::make_shared<UiButton>(UiButton(_key_manager2));
	button->SetObjectPos({ 123, 315 });
	button->ReadyButton(L"Stat.img\\Stat.BtDetail");
	button->SetCallBack(DetailBackGroundSlide);
	_detail_button = button;

	

	
}

void StatWindow::UpdateWindow()
{
	_key_manager->KeyUpdate();
	if(_key_manager->KeyDown(KEY_S))
	{
		SetShow(!IsShow());
	}
	auto player = MapManager::GetInstance()->GetPlayer();
	for (int i = 0; i < 4; ++i)
	{
		if (player->GetAp() > 0)
		{
			_ap_up_button[i]->SetState(UiButton::ButtonState::kNormal);
		}
		else
		{
			_ap_up_button[i]->SetState(UiButton::ButtonState::kDisable);
		}
	}
	if (InMouserWindow())
	{
		const POINT mouse = InGameScene::GetMouse()->GetPoint();
		if (_key_manager->KeyDown(KEY_LBUTTON))
		{
			TitleBarClick(mouse);
			SelectTab(mouse);
		}
		if (_key_manager->KeyPressing(KEY_LBUTTON))
		{
			if (_move_window)
			{
				_click_pos.x = _click_pos.x - mouse.x;
				_click_pos.y = _click_pos.y - mouse.y;
				_info.x -= static_cast<float>(_click_pos.x);
				_info.y -= static_cast<float>(_click_pos.y);
				for (int i = 0; i < 4; ++i)
				{
					_ap_up_button[i]->GainObjectPosXY(static_cast<float>(-_click_pos.x), static_cast<float>(-_click_pos.y));
				}
				_ap_up_auto_button->GainObjectPosXY(static_cast<float>(-_click_pos.x), static_cast<float>(-_click_pos.y));
				_detail_button->GainObjectPosXY(static_cast<float>(-_click_pos.x), static_cast<float>(-_click_pos.y));
				_click_pos = mouse;
			}
		}
		if (_key_manager->KeyUp(KEY_LBUTTON))
		{
			TitleBarUp(mouse);
		}

		for (int i = 0; i < 4; ++i)
		{
			_ap_up_button[i]->UpdateButton();
		}
		_ap_up_auto_button->UpdateButton();
		_detail_button->UpdateButton();
	}
}

void StatWindow::RenderWinodw(HDC hdc)
{
	if (IsShow())
	{
		BaseStatWindowRender(hdc);
		BaseStatInfoRender(hdc);
		auto player = MapManager::GetInstance()->GetPlayer();
		std::wstring str;

		if (_show_background2)
		{
			int min_power = 0;

			int max_power = 0;
			str.append(std::to_wstring(player->GetMinPower())).append(L" ~ ").append(std::to_wstring(player->GetMaxPower()));
			StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 246, static_cast<int>(_info.y) + 144, str, 11, RGB(0, 0, 0), L"µ¸¿ò");
			str.clear();
			str.append(std::to_wstring(player->GetEqpPdd()));
			StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 246, static_cast<int>(_info.y) + 162, str, 11, RGB(0, 0, 0), L"µ¸¿ò");
			str.clear();
			str.append(std::to_wstring(player->GetTotalMad()));
			StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 246, static_cast<int>(_info.y) + 180, str, 11, RGB(0, 0, 0), L"µ¸¿ò");
			str.clear();
			str.append(std::to_wstring(player->GetEqpMdd()));
			StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 246, static_cast<int>(_info.y) + 198, str, 11, RGB(0, 0, 0), L"µ¸¿ò");
			str.clear();
			str.append(std::to_wstring(player->GetTotalDex()));
			StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 246, static_cast<int>(_info.y) + 216, str, 11, RGB(0, 0, 0), L"µ¸¿ò");
			str.clear();
			str.append(std::to_wstring((player->GetTotalLuk() > 100 ? 100 : player->GetTotalLuk())));
			StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 246, static_cast<int>(_info.y) + 234, str, 11, RGB(0, 0, 0), L"µ¸¿ò");
			str.clear();
			str.append(L"0");
			StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 246, static_cast<int>(_info.y) + 252, str, 11, RGB(0, 0, 0), L"µ¸¿ò");
			str.clear();
			str.append(L"100%");
			StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 246, static_cast<int>(_info.y) + 270, str, 11, RGB(0, 0, 0), L"µ¸¿ò");
			str.clear();
			str.append(L"100%");
			StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 246, static_cast<int>(_info.y) + 288, str, 11, RGB(0, 0, 0), L"µ¸¿ò");
		}
	}
}

void StatWindow::BaseStatWindowRender(HDC hdc)
{
	if (_show_background2)
	{
		_background2->RenderBitmapImage(hdc,
			static_cast<int>(_info.x) + _background->GetWidth() - 5,
			static_cast<int>(_info.y) + _background->GetHeight() - _background2->GetHeight(),
			_background2->GetWidth(),
			_background2->GetHeight());
	}

	_background->RenderBitmapImage(hdc,
		static_cast<int>(_info.x),
		static_cast<int>(_info.y),
		_background->GetWidth(),
		_background->GetHeight());

	for (int i = 0; i < 4; ++i)
	{
		_ap_up_button[i]->RenderButtonUi(hdc);
	}
	_ap_up_auto_button->RenderButtonUi(hdc);
	_detail_button->RenderButtonUi(hdc);
}

void StatWindow::BaseStatInfoRender(HDC hdc)
{
	auto player = MapManager::GetInstance()->GetPlayer();
	std::wstring str;
	str.append(StringTools::StringToWString(player->GetName().c_str()));
	StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 61, static_cast<int>(_info.y) + 36, str, 11, RGB(0, 0, 0), L"µ¸¿ò");
	str.clear();
	str.append(L"¸¶¹ý»ç");
	StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 61, static_cast<int>(_info.y) + 53, str, 11, RGB(0, 0, 0), L"µ¸¿ò");
	str.clear();
	str.append(std::to_wstring(player->GetAp()));
	StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 61, static_cast<int>(_info.y) + 71, str, 11, RGB(0, 0, 0), L"µ¸¿ò");
	str.clear();
	str.append(L"-");
	StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 61, static_cast<int>(_info.y) + 88, str, 11, RGB(0, 0, 0), L"µ¸¿ò");
	str.clear();
	str.append(std::to_wstring(player->GetHp())).append(L" / ").append(std::to_wstring(player->GetMaxHp()));
	StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 61, static_cast<int>(_info.y) + 107, str, 11, RGB(0, 0, 0), L"µ¸¿ò");
	str.clear();
	str.append(std::to_wstring(player->GetMp())).append(L" / ").append(std::to_wstring(player->GetMaxMp()));
	StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 61, static_cast<int>(_info.y) + 125, str, 11, RGB(0, 0, 0), L"µ¸¿ò");
	str.clear();
	const float expPercent =
		static_cast<float>(player->GetPlayerInfo()->exp) / 10000 * 100.f;
	str.append(std::to_wstring(player->GetExp())).append(L" (").append(std::to_wstring(static_cast<int>(expPercent))).append(L"%)");
	StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 61, static_cast<int>(_info.y) + 143, str, 11, RGB(0, 0, 0), L"µ¸¿ò");
	str.clear();
	str.append(L"0");
	StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 61, static_cast<int>(_info.y) + 161, str, 11, RGB(0, 0, 0), L"µ¸¿ò");
	str.clear();
	str.append(std::to_wstring(player->GetAp()));
	StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 67, static_cast<int>(_info.y) + 208, str, 11, RGB(0, 0, 0), L"µ¸¿ò");
	str.clear();
	str.append(std::to_wstring(player->GetStr()));
	if (player->GetEqpStr() > 0)
	{
		str.append(L" (+").append(std::to_wstring(player->GetEqpStr()).append(L")"));
	}
	StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 60, static_cast<int>(_info.y) + 237, str, 11, RGB(0, 0, 0), L"µ¸¿ò");
	str.clear();
	str.append(std::to_wstring(player->GetDex()));
	if (player->GetEqpDex() > 0)
	{
		str.append(L" (+").append(std::to_wstring(player->GetEqpDex()).append(L")"));
	}
	StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 60, static_cast<int>(_info.y) + 255, str, 11, RGB(0, 0, 0), L"µ¸¿ò");
	str.clear();
	str.append(std::to_wstring(player->GetInt()));
	if (player->GetEqpInt() > 0)
	{
		str.append(L" (+").append(std::to_wstring(player->GetEqpInt()).append(L")"));
	}
	StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 60, static_cast<int>(_info.y) + 273, str, 11, RGB(0, 0, 0), L"µ¸¿ò");
	str.clear();
	str.append(std::to_wstring(player->GetLuk()));
	if (player->GetEqpLuk() > 0)
	{
		str.append(L" (+").append(std::to_wstring(player->GetEqpLuk()).append(L")"));
	}
	StringTools::CreateTextOut(hdc, static_cast<int>(_info.x) + 60, static_cast<int>(_info.y) + 291, str, 11, RGB(0, 0, 0), L"µ¸¿ò");

}
