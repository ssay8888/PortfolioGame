#include "../../../pch.h"
#include "base_window.h"
#include "../../KeyManaer/key_manager.h"
#include "../../ScenManager/InGameScene/in_game_scene.h"
#include "../../../Components/game_mouse.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../ui_tab_button.h"
BaseWindow::BaseWindow(): _info(),
                          _key_manager(new KeyManager()),
                          _show(false),
                          _move_window(false),
                          _click_pos()
{
}

BaseWindow::~BaseWindow()
{
	delete _key_manager;
}

void BaseWindow::DoReadyWindow()
{
    ReadyWindow();
}

void BaseWindow::DoUpdateWindow()
{
    UpdateWindow();
}

void BaseWindow::DoRenderWindow(const HDC hdc)
{
    RenderWinodw(hdc);
}

void BaseWindow::BaseUiEvent()
{
	const POINT mouse = InGameScene::GetMouse()->GetPoint();
	if (_key_manager->KeyDown(KEY_LBUTTON))
	{
		TitleBarClick(mouse);
		SelectTab(mouse);
	}
	if (_key_manager->KeyPressing(KEY_LBUTTON))
	{
		TitleBarMove(mouse);
	}
	if (_key_manager->KeyUp(KEY_LBUTTON))
	{
		TitleBarUp(mouse);
	}
}

void BaseWindow::BaseUiRender(HDC hdc)
{
	if(_show)
	{
		BackgroundRender(hdc);
		TabRender(hdc);
	}
}


void BaseWindow::SetShow(const bool show)
{
	_show = show;
}

bool BaseWindow::IsShow() const
{
	return _show;
}

bool BaseWindow::InMouserSkillWindow() const
{
	if (!_show)
	{
		return false;
	}
	const POINT mouse = InGameScene::GetMouse()->GetPoint();
	const RECT rc{ static_cast<int>(_info.x), static_cast<int>(_info.y),
		static_cast<int>(_info.x) +_info.cx,
		static_cast<int>(_info.y) + _info.cy };
	if (PtInRect(&rc, mouse))
	{
		return true;
	}
	return false;
}

Info BaseWindow::GetInfo() const
{
	return _info;
}

void BaseWindow::BackgroundRender(HDC hdc) const
{
	_background->RenderBitmapImage(hdc,
		static_cast<int>(_info.x),
		static_cast<int>(_info.y),
		_background->GetWidth(),
		_background->GetHeight());
}

void BaseWindow::TabRender(const HDC hdc)
{
	for (auto& tab : _tab_buttons)
	{
		tab->RenderButtonUi(hdc);
	}
}

void BaseWindow::TitleBarClick(const POINT mouse)
{
	const RECT top_bar{
		static_cast<int>(_info.x),
		static_cast<int>(_info.y),
		static_cast<int>(_info.x + _info.cx),
		static_cast<int>(_info.y) + 20 };
	if (PtInRect(&top_bar, mouse))
	{
		_move_window = true;
		_click_pos = mouse;
	}
}

void BaseWindow::TitleBarMove(const POINT mouse)
{
	if (_move_window)
	{
		_click_pos.x = _click_pos.x - mouse.x;
		_click_pos.y = _click_pos.y - mouse.y;
		_info.x -= static_cast<float>(_click_pos.x);
		_info.y -= static_cast<float>(_click_pos.y);
		for (const auto tab_button : _tab_buttons)
		{
			tab_button->GainInfoPosXY(static_cast<float>(-_click_pos.x), static_cast<float>(-_click_pos.y));
		}
		_click_pos = mouse;
	}
}

void BaseWindow::TitleBarUp(POINT mouse)
{
	if (_move_window)
	{
		_move_window = false;
	}
}

void BaseWindow::SelectTab(const POINT mouse)
{
	bool check = false;
	uint32_t i = 0;
	std::shared_ptr<UiTabButton> select_button(nullptr);
	for (const auto& button : _tab_buttons)
	{
		RECT tab_bar = button->GetRect();
		if (PtInRect(&tab_bar, mouse))
		{
			button->SetState(UiTabButton::ButtonState::kEnable);
			select_button = button;
		}
		++i;
	}
	for (const auto& button : _tab_buttons)
	{
		if (select_button != nullptr)
		{
			if (button != select_button)
			{
				button->SetState(UiTabButton::ButtonState::kDisable);
			}
		}
	}
}
