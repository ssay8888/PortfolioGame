#include "../../pch.h"
#include "ui_tab_button.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../Components/game_mouse.h"
#include "../ScenManager/InGameScene/in_game_scene.h"
#include "../KeyManaer/key_manager.h"

UiTabButton::UiTabButton() :
	_info(),
	_rect(),
	_state(ButtonState::kDisable),
	_show(true)
{
}

UiTabButton::~UiTabButton()
{
}

void UiTabButton::ReadyButton(std::wstring button_path)
{
	std::wstring path;
	path.append(L"Client\\Ui\\").append(button_path).append(L".enable.bmp");
	if (!_access(StringTools::WStringToString(path.c_str()).c_str(), 0))
	{
		_enable = std::make_shared<MyBitmap>(MyBitmap());
		_enable->Insert_Bitmap(_hWnd, path.c_str());
		_info.cx = _enable->GetWidth();
		_info.cy = _enable->GetHeight();
		UpdateRect();
	}

	path.clear();
	path.append(L"Client\\Ui\\").append(button_path).append(L".disable.bmp");
	if (!_access(StringTools::WStringToString(path.c_str()).c_str(), 0))
	{
		_disable = std::make_shared<MyBitmap>(MyBitmap());
		_disable->Insert_Bitmap(_hWnd, path.c_str());
	}

}

void UiTabButton::UpdateButton()
{
	//GameMouse* gameMouse = InGameScene::GetMouse();

	//if (KeyManager::GetInstance()->KeyDown(KEY_LBUTTON))
	//{
	//	if (PtInRect(&_rect, gameMouse->GetPoint()))
	//	{
	//		_state = ButtonState::kEnable;
	//	}
	//}
}

void UiTabButton::RenderButtonUi(const HDC hdc)
{
	if (_show)
	UpdateRect();

	switch (_state)
	{
	case UiTabButton::ButtonState::kEnable:
		_enable->RenderBitmapImage(hdc,
			static_cast<int>(_info.x),
			static_cast<int>(_info.y),
			static_cast<int>(_enable->GetWidth()),
			static_cast<int>(_enable->GetHeight()));
		break;
	case UiTabButton::ButtonState::kDisable:
		_disable->RenderBitmapImage(hdc,
			static_cast<int>(_info.x),
			static_cast<int>(_info.y),
			static_cast<int>(_disable->GetWidth()),
			static_cast<int>(_disable->GetHeight()));
		break;
	default:
		break;
	}
}

void UiTabButton::UpdateRect()
{
	_rect.left = static_cast<int>(_info.x);
	_rect.top = static_cast<int>(_info.y);
	_rect.right = static_cast<int>(_info.x) + static_cast<int>(_info.cx);
	_rect.bottom = static_cast<int>(_info.y) + static_cast<int>(_info.cy);
}

UiTabButton::ButtonState UiTabButton::GetState() const
{
	return _state;
}

void UiTabButton::SetState(const ButtonState state)
{
	_state = state;
}

void UiTabButton::SetInfoPos(const Info pos)
{
	_info = pos;
}

void UiTabButton::SetInfoPosX(const float x)
{
	_info.x = x;
}

void UiTabButton::SetInfoPosY(const float y)
{
	_info.y = y;
}

void UiTabButton::SetShow(const bool show)
{
	_show = show;
}

bool UiTabButton::IsShow() const
{
	return _show;
}

RECT UiTabButton::GetRect() const
{
	return _rect;
}

void UiTabButton::GainInfoPosX(const float x)
{
	_info.x += x;
}

void UiTabButton::GainInfoPosY(const float y)
{
	_info.y += y;
}

void UiTabButton::GainInfoPosXY(const float x, const float y)
{
	_info.x += x;
	_info.y += y;
}
