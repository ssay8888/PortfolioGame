#include "../pch.h"
#include "game_mouse.h"
#include "../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../Managers/KeyManaer/key_manager.h"

int GameMouse::ReadyGameObject()
{
	std::shared_ptr<MyBitmap*> basic = std::make_shared<MyBitmap*>(new MyBitmap());
	(*basic)->Insert_Bitmap(_hWnd, L"Client\\Ui\\Basic.img\\Cursor.0.0.bmp");
	_mouseImage.insert({ MouseState::kBasic, { basic } });


	std::shared_ptr<MyBitmap*> click = std::make_shared<MyBitmap*>(new MyBitmap());
	(*click)->Insert_Bitmap(_hWnd, L"Client\\Ui\\Basic.img\\Cursor.1.0.bmp");
	_mouseImage.insert({ MouseState::kClick, { click } });


	std::shared_ptr<MyBitmap*> drag1 = std::make_shared<MyBitmap*>(new MyBitmap());
	std::shared_ptr<MyBitmap*> drag2 = std::make_shared<MyBitmap*>(new MyBitmap());
	std::shared_ptr<MyBitmap*> drag3 = std::make_shared<MyBitmap*>(new MyBitmap());
	(*drag1)->Insert_Bitmap(_hWnd, L"Client\\Ui\\Basic.img\\Cursor.2.0.bmp");
	(*drag2)->Insert_Bitmap(_hWnd, L"Client\\Ui\\Basic.img\\Cursor.2.1.bmp");
	(*drag3)->Insert_Bitmap(_hWnd, L"Client\\Ui\\Basic.img\\Cursor.2.2.bmp");
	_mouseImage.insert({ MouseState::kDrag, { drag1, drag2, drag3 } });

	std::shared_ptr<MyBitmap*> isButton1 = std::make_shared<MyBitmap*>(new MyBitmap());
	std::shared_ptr<MyBitmap*> isButton2 = std::make_shared<MyBitmap*>(new MyBitmap());
	(*isButton1)->Insert_Bitmap(_hWnd, L"Client\\Ui\\Basic.img\\Cursor.3.0.bmp");
	(*isButton2)->Insert_Bitmap(_hWnd, L"Client\\Ui\\Basic.img\\Cursor.3.1.bmp");
	_mouseImage.insert({ MouseState::kIsButton, { isButton1, isButton2 } });

	_key_manager = new KeyManager();
	return 0;
}

void GameMouse::UpdateGameObject(const float deltaTime)
{
	_key_manager->KeyUpdate();
	GetCursorPos(&_cursorPos);
	ScreenToClient(_hWnd, &_cursorPos);
	this->SetInfo({ static_cast<float>(_cursorPos.x), static_cast<float>(_cursorPos.y) });

	if (_key_manager->KeyDown(KEY_LBUTTON))
	{
		_state = MouseState::kClick;
	}
	if (_key_manager->KeyUp(KEY_LBUTTON))
	{
		_state = MouseState::kBasic;
	}

}

void GameMouse::RenderGameObject(HDC hdc)
{
	auto image = _mouseImage.find(_state)->second[_frameCount];
	(*image)->RenderBitmapImage(hdc,
		static_cast<int>(this->GetInfo().x),
		static_cast<int>(this->GetInfo().y),
		(*image)->GetWidth(), (*image)->GetHeight());
}

void GameMouse::LateUpdateGameObject()
{
}

GameMouse::GameMouse() :
	GameObject(8),
	_frameCount(0),
	_state(MouseState::kBasic),
	_cursorPos({})
{
}
GameMouse::~GameMouse()
{
	delete _key_manager;
}


POINT GameMouse::GetPoint() const
{
	return _cursorPos;
}
