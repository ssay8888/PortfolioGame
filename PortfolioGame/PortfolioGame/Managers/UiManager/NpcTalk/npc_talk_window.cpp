#include "../../../pch.h"
#include "npc_talk_window.h"

#include "../ui_button.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../UiManager/ui_manager.h"
#include "../../../Components/MapObject/Npc/npc.h"
#include "../../KeyManaer/key_manager.h"

NpcTalkWindow::NpcTalkWindow(): _talk_type()
{
}

NpcTalkWindow::~NpcTalkWindow()
{
}

void NpcTalkWindow::CancelNpcTalk()
{
	UiManager::GetInstance()->GetNpcTalkWindow()->SetShow(false);
}

void NpcTalkWindow::ReadyWindow()
{
	_show = true;

	_background = std::make_shared<MyBitmap>(MyBitmap());
	_background->Insert_Bitmap(_hWnd, L"Client\\Ui\\UtilDlgEx.img\\background.bmp");
	_info.x = WindowCX / 2 - _background->GetWidth() / 2.f;
	_info.y = WindowCY / 2 - _background->GetHeight() / 2.f;
	_info.cx = _background->GetWidth();
	_info.cy = _background->GetHeight();
	_key_manager = new KeyManager();

	auto button = std::make_shared<UiButton>(UiButton(_key_manager));
	button->SetObjectPos({ _info.x + 8, _info.y + 179});
	button->ReadyButton(L"UtilDlgEx.img\\UtilDlgEx.BtClose");
	button->SetCallBack(CancelNpcTalk);
	_list_button.insert(std::make_pair(NpcTalkButton::kClose, button));

	button = std::make_shared<UiButton>(UiButton(_key_manager));
	button->SetObjectPos({ _info.x + 449, _info.y + 127 });
	button->ReadyButton(L"UtilDlgEx.img\\UtilDlgEx.BtNext");
	_list_button.insert(std::make_pair(NpcTalkButton::kNext, button));

	button = std::make_shared<UiButton>(UiButton(_key_manager));
	button->SetObjectPos({ _info.x + 461, _info.y + 177 });
	button->ReadyButton(L"UtilDlgEx.img\\UtilDlgEx.BtNo");
	_list_button.insert(std::make_pair(NpcTalkButton::kNo, button));

	button = std::make_shared<UiButton>(UiButton(_key_manager));
	button->SetObjectPos({ _info.x + 472, _info.y + 178 });
	button->ReadyButton(L"UtilDlgEx.img\\UtilDlgEx.BtOK");
	_list_button.insert(std::make_pair(NpcTalkButton::kOk, button));

	button = std::make_shared<UiButton>(UiButton(_key_manager));
	button->SetObjectPos({ _info.x + 461, _info.y + 177 });
	button->ReadyButton(L"UtilDlgEx.img\\UtilDlgEx.BtQNo");
	_list_button.insert(std::make_pair(NpcTalkButton::kQNo, button));

	button = std::make_shared<UiButton>(UiButton(_key_manager));
	button->SetObjectPos({ _info.x + 396, _info.y + 177 });
	button->ReadyButton(L"UtilDlgEx.img\\UtilDlgEx.BtYes");
	_list_button.insert(std::make_pair(NpcTalkButton::kYes, button));

	button = std::make_shared<UiButton>(UiButton(_key_manager));
	button->SetObjectPos({ _info.x + 396, _info.y + 177 });
	button->ReadyButton(L"UtilDlgEx.img\\UtilDlgEx.BtQYes");
	_list_button.insert(std::make_pair(NpcTalkButton::kQYes, button));


}

void NpcTalkWindow::UpdateWindow()
{
	_key_manager->KeyUpdate();
	for (auto button : _list_button)
	{
		button.second->UpdateButton();
	}
	_talk_type = ObjectType::NpcTalkType::kOk;
}

void NpcTalkWindow::RenderWinodw(HDC hdc)
{
	if (!IsShow())
	{
		return;
	}

	_background->RenderBitmapImage(hdc,
		static_cast<int>(_info.x),
		static_cast<int>(_info.y),
		_background->GetWidth(),
		_background->GetHeight());
	if (_this_npc != nullptr)
	{
		auto images = _this_npc->GetStandImages();
		if (!images.empty())
		{
			images[0]->RenderBitmapImage(hdc,
				static_cast<int>(_info.x) + 68,
				static_cast<int>(_info.y) + 111,
				images[0]->GetWidth(),
				images[0]->GetHeight());
		}
	}
	auto close_button_pair = _list_button.find(NpcTalkButton::kClose);
	if (close_button_pair != _list_button.end())
	{
		auto close_button = close_button_pair->second;
		close_button->RenderButtonUi(hdc);
	}
	switch (_talk_type) {
	case ObjectType::NpcTalkType::kNext:
	{
		auto next_button_pair = _list_button.find(NpcTalkButton::kNext);
		if (next_button_pair != _list_button.end())
		{
			auto next_button = next_button_pair->second;
			next_button->RenderButtonUi(hdc);
		}
		break;
	}
	case ObjectType::NpcTalkType::kOk: 
	{
		auto ok_button_pair = _list_button.find(NpcTalkButton::kOk);
		if (ok_button_pair != _list_button.end())
		{
			auto ok_button = ok_button_pair->second;
			ok_button->RenderButtonUi(hdc);
		}
		break;
	}
	case ObjectType::NpcTalkType::kQYesNo: 
	{

		auto qyes_button_pair = _list_button.find(NpcTalkButton::kQYes);
		if (qyes_button_pair != _list_button.end())
		{
			auto qyes_button = qyes_button_pair->second;
			qyes_button->RenderButtonUi(hdc);
		}
		auto qno_button_pair = _list_button.find(NpcTalkButton::kQNo);
		if (qno_button_pair != _list_button.end())
		{
			auto qno_button = qno_button_pair->second;
			qno_button->RenderButtonUi(hdc);
		}
		break;
	}
	default:;
	}
}
