#include "../../../pch.h"
#include "dead_message.h"
#include "../ui_button.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../Components/game_mouse.h"
#include "../../../Components/MapObject/Player/player.h"
#include "../../KeyManaer/key_manager.h"
#include "../../ScenManager/InGameScene/in_game_scene.h"
#include "../../MapManager/map_manager.h"
#include "../ui_manager.h"

DeadMessage::DeadMessage()
{
}

DeadMessage::~DeadMessage()
{

}

void DeadMessage::ResetCharacter()
{
	auto manager = UiManager::GetInstance()->GetDeadMessage();
	const auto player = MapManager::GetInstance()->GetPlayer();

	player->SetHp(player->GetMaxHp() / 2);
	player->SetMp(player->GetMaxMp() / 2);
	player->SetDead(false);
	manager->SetShow(false);
}

void DeadMessage::ReadyWindow()
{
	_background = std::make_shared<MyBitmap>(MyBitmap());
	_background->Insert_Bitmap(_hWnd, L"Client\\Ui\\Dead.img\\0.bmp");
	_info.cx = _background->GetWidth();
	_info.cy = _background->GetHeight();

	_info.x = static_cast<float>(400 - (_background->GetWidth() >> 1));
	_info.y = static_cast<float>(300 - (_background->GetHeight() >> 1));

	_key_manager = new KeyManager();
	_ok_button = std::make_shared<UiButton>(UiButton(_key_manager));
	_ok_button->SetObjectPos({ _info.x + 117, _info.y + 113 });
	_ok_button->SetCallBack(ResetCharacter);
	_ok_button->ReadyButton(L"Dead.img\\UtilDlgEx.BtOK");
}

void DeadMessage::UpdateWindow()
{
	if (IsShow())
	{
		_key_manager->KeyUpdate();
		_ok_button->UpdateButton();

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
				_ok_button->GainObjectPosXY(static_cast<float>(-_click_pos.x), static_cast<float>(-_click_pos.y));
				_click_pos = mouse;
			}
		}
		if (_key_manager->KeyUp(KEY_LBUTTON))
		{
			TitleBarUp(mouse);
		}
	}
}

void DeadMessage::RenderWinodw(HDC hdc)
{
	if (IsShow())
	{
		BaseUiRender(hdc);
		_ok_button->RenderButtonUi(hdc);
	}
}
