#include "../../pch.h"
#include "ui_button.h"
#include "../../Components/game_mouse.h"
#include "../KeyManaer/key_manager.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../ScenManager/InGameScene/in_game_scene.h"

UiButton::UiButton() :
	_normalImage(nullptr),
	_overImage(nullptr),
	_pressedImage(nullptr)
{
}

UiButton::~UiButton()
{
}

/// <summary>
/// 
/// </summary>
/// <param name="buttonPath"> StatusBar\\BtMenu 를의 형식을 넣어줘야함. </param>
void UiButton::ReadyButton(std::wstring buttonPath)
{
	std::wstring path;
	path.append(L"Client\\Ui\\").append(buttonPath).append(L".normal.0.bmp");
	if (!_access(StringTools::WStringToString(path.c_str()).c_str(), 0))
	{
		_normalImage = std::make_shared<MyBitmap>(MyBitmap());
		_normalImage->Insert_Bitmap(_hWnd, path.c_str());
		_info.cx = _normalImage->GetWidth();
		_info.cy = _normalImage->GetHeight();
		_rect.left = static_cast<int>(_info.x);
		_rect.top = static_cast<int>(_info.y);
		_rect.right = static_cast<int>(_info.x) + static_cast<int>(_info.cx);
		_rect.bottom = static_cast<int>(_info.y) + static_cast<int>(_info.cy);
	}

	path.clear();
	path.append(L"Client\\Ui\\").append(buttonPath).append(L".mouseOver.0.bmp");
	if (!_access(StringTools::WStringToString(path.c_str()).c_str(), 0))
	{
		_overImage = std::make_shared<MyBitmap>(MyBitmap());
		_overImage->Insert_Bitmap(_hWnd, path.c_str());
	}

	path.clear();
	path.append(L"Client\\Ui\\").append(buttonPath).append(L".pressed.0.bmp");
	if (!_access(StringTools::WStringToString(path.c_str()).c_str(), 0))
	{
		_pressedImage = std::make_shared<MyBitmap>(MyBitmap());
		_pressedImage->Insert_Bitmap(_hWnd, path.c_str());
	}


}

void UiButton::UpdateButton()
{
	GameMouse* gameMouse = InGameScene::GetMouse();

	if (PtInRect(&_rect, gameMouse->GetPoint()))
	{
		_state = ButtonState::kMouseOver;
		if (KeyManager::GetInstance()->KeyPressing(KEY_LBUTTON))
		{
			_state = ButtonState::kPressed;
		}
	}
	else
	{
		_state = ButtonState::kNormal;
	}
}

void UiButton::RenderButtonUi(HDC hdc)
{

	switch (_state)
	{
	case UiButton::ButtonState::kNormal:
		_normalImage->RenderBitmapImage(hdc, 
			static_cast<int>(_info.x), 
			static_cast<int>(_info.y), 
			static_cast<int>(_normalImage->GetWidth()), 
			static_cast<int>(_normalImage->GetHeight()));
		break;
	case UiButton::ButtonState::kMouseOver:
		_overImage->RenderBitmapImage(hdc, 
			static_cast<int>(_info.x), 
			static_cast<int>(_info.y),
			static_cast<int>(_normalImage->GetWidth()),
			static_cast<int>(_normalImage->GetHeight()));
		break;
	case UiButton::ButtonState::kPressed:
		_pressedImage->RenderBitmapImage(hdc, 
			static_cast<int>(_info.x), 
			static_cast<int>(_info.y),
			static_cast<int>(_normalImage->GetWidth()),
			static_cast<int>(_normalImage->GetHeight()));
		break;
	default:
		break;
	}
}

void UiButton::SetObjectPos(Info pos)
{
	_info = pos;
}

void UiButton::SetObjectPosX(float x)
{
	_info.x = x;
}

void UiButton::SetObjectPosY(float y)
{
	_info.y = y;
}
