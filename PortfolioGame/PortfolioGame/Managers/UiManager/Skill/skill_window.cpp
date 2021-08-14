#include "../../../pch.h"
#include "skill_window.h"
#include "../ui_tab_button.h"
#include "../UiScroll/ui_scroll.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../../Common/Utility/string_tools.h"
#include "../../../Components/game_mouse.h"
#include "../../../Components/MapObject/Player/player.h"
#include "../../../Managers/MapManager/map_manager.h"
#include "../../KeyManaer/key_manager.h"
#include "../../SkillManager/skill_manager.h"
#include "../../SkillManager/Skill/skill.h"
#include "../../SkillManager/Skill/skill_effect_image.h"
#include "../../SkillManager/Skill/skill_info.h"
#include "../../ScenManager/InGameScene/in_game_scene.h"
#include "../../UiManager/ui_manager.h"
#include "../../../Utility/game_constants.h"
#include "../QuickSlot/quick_slot.h"

SkillWindow::SkillWindow() :
                           _info({ 0,0, 175, 289 }),
                           _skill_window_image(nullptr),
                           _select_tab(SkillTab::kBasic),
                           _skill_ui_distance(40),
                           _mouse_over_skillid(0),
                           _select_skill(nullptr),
                           _key_manager(nullptr),
                           _scroll(nullptr)
{
	UpdateRect();
}

SkillWindow::~SkillWindow()
{
	if (_skill_window_image != nullptr)
	{
		delete _skill_window_image;
		_skill_window_image = nullptr;
	}
	for (const auto& data : _skill_window_buttons)
	{
		delete data;
	}
	_skill_window_buttons.clear();
}

void SkillWindow::ReadyWindow()
{
	_skill_window_image = new MyBitmap();
	_skill_window_image->Insert_Bitmap(_hWnd, L"Client\\Ui\\Skill.img\\backgrnd.bmp");
	float x = 2;
	for (int i = 0; i < 5; ++i)
	{
		wchar_t path[150];
		swprintf_s(path, 150, L"Skill.img\\%d", i);
		auto button = new UiTabButton();
		button->ReadyButton(path);
		button->SetInfoPosX(x);
		button->SetInfoPosY(23);
		x += 34;
		if (i == 0)
		{
			button->SetState(UiTabButton::ButtonState::kEnable);
		}
		_skill_window_buttons.emplace_back(button);
	}
	int job = MapManager::GetInstance()->GetPlayer()->GetJob();

	_key_manager = new KeyManager();
	_scroll = new UiScroll();
}

void SkillWindow::UpdateWindow()
{
	_key_manager->KeyUpdate();
	if (_key_manager->KeyDown(KEY_K))
	{
		_show_window = !_show_window;
	}
	if (_show_window)
	{
		WindowEvent();

	}
}

void SkillWindow::RenderWindow(const HDC hdc)
{
	if (_show_window)
	{
		UpdateRect();
		RenderBackground(hdc);
		RenderSp(hdc);
		RenderSkillList(hdc);
		RenderTab(hdc);
		RenderQuickSlot(hdc);
	}
}

void SkillWindow::RenderBackground(const HDC hdc) const
{
	_skill_window_image->RenderBitmapImage(hdc,
		static_cast<int>(_info.x),
		static_cast<int>(_info.y),
		_skill_window_image->GetWidth(), _skill_window_image->GetHeight(),
		_skill_window_image->GetWidth(), _skill_window_image->GetHeight());

}

void SkillWindow::RenderSp(const HDC hdc) const
{
	std::wstring strew;
	strew.append(std::to_wstring(MapManager::GetInstance()->GetPlayer()->GetSp()));
	StringTools::CreateTextOut(hdc,
		static_cast<int>(_info.x + 85),
		static_cast<int>(_info.y) + 268, strew.c_str(), 11, RGB(0, 0, 0));

}

void SkillWindow::RenderSkillList(const HDC hdc)
{
	auto paddingsize = 99;

	for (const auto& skill_icon : _show_skill_list)
	{
		MyBitmap* icon = nullptr;
		if (_mouse_over_skillid == skill_icon->GetSkillId())
		{
			icon = skill_icon->GetIconMouseOverImage();
		}
		else
		{
			icon = skill_icon->GetIconImage();
		}
		if (static_cast<int>(_info.y) + paddingsize + _scroll->GetScrollY() >= static_cast<int>(_info.y) + 99 &&
			static_cast<int>(_info.y) + paddingsize + _scroll->GetScrollY() <= static_cast<int>(_info.y) + 219)
		{
			icon->RenderBitmapImage(hdc,
				static_cast<int>(_info.x) + 6,
				static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()),
				icon->GetWidth(), icon->GetHeight());
		}

		paddingsize += _skill_ui_distance;
	}
}

void SkillWindow::RenderTab(const HDC hdc)
{
	const int job = MapManager::GetInstance()->GetPlayer()->GetJob();
	uint32_t i = 0;
	const auto size = GameConstants::ShowSkillTabSize(job);
	for (const auto tab_button : _skill_window_buttons)
	{
		if (i <= size)
		{
			tab_button->RenderButtonUi(hdc);
		}
		++i;
	}

}

void SkillWindow::RenderQuickSlot(HDC hdc)
{
	if (_select_skill != nullptr)
	{
		const POINT mouse = InGameScene::GetMouse()->GetPoint();
		_select_skill->GetIconImage()->RenderBitmapImage(hdc,
			mouse.x - (_select_skill->GetIconImage()->GetWidth() >> 1),
			mouse.y - (_select_skill->GetIconImage()->GetHeight() >> 1),
			_select_skill->GetIconImage()->GetWidth(), _select_skill->GetIconImage()->GetHeight());
	}
}

void SkillWindow::WindowEvent()
{
	const POINT mouse = InGameScene::GetMouse()->GetPoint();
	FindMouseOverSkillId(mouse);
	if (_key_manager->KeyDown(KEY_LBUTTON))
	{
		if (InMouserSkillWindow())
		{
			TitleBarClick(mouse);
			SelectTab(mouse);
			SelectSkillIcon(mouse);
			ScrollBarUpClick(mouse);
			ScrollBarDownClick(mouse);
		}
	}
	

	if (_key_manager->KeyPressing(KEY_LBUTTON))
	{
		TitleBarMove(mouse);
	}
	

	if (_key_manager->KeyUp(KEY_LBUTTON))
	{
		TitleBarUp(mouse);
		DropSkillIcon(mouse);
	}
}

void SkillWindow::FindMouseOverSkillId(const POINT mouse)
{
	auto paddingsize = 99;
	_mouse_over_skillid = 0;
	for (const auto& skill_icon : _show_skill_list)
	{
		const auto icon = skill_icon->GetIconImage();
		RECT icon_rect{ static_cast<int>(_info.x + 6),
			static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()),
					static_cast<int>(_info.x + 6) + skill_icon->GetIconImage()->GetWidth(),
			static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()) + skill_icon->GetIconImage()->GetHeight() };

		if (PtInRect(&icon_rect, mouse))
		{
			_mouse_over_skillid = skill_icon->GetSkillId();
		}

		paddingsize += _skill_ui_distance;
	}
}

void SkillWindow::TitleBarClick(POINT mouse)
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

void SkillWindow::TitleBarMove(POINT mouse)
{
	if (_move_window)
	{
		_click_pos.x = _click_pos.x - mouse.x;
		_click_pos.y = _click_pos.y - mouse.y;
		_info.x -= static_cast<float>(_click_pos.x);
		_info.y -= static_cast<float>(_click_pos.y);
		for (const auto tab_button : _skill_window_buttons)
		{
			tab_button->GainInfoPosXY(static_cast<float>(-_click_pos.x), static_cast<float>(-_click_pos.y));
		}
		_click_pos = mouse;
	}
}

void SkillWindow::TitleBarUp(POINT mouse)
{
	if (_move_window)
	{
		_move_window = false;
	}
}

void SkillWindow::SelectTab(const POINT mouse)
{
	bool check = false;
	uint32_t i = 0;
	UiTabButton* select_button(nullptr);
	int job = MapManager::GetInstance()->GetPlayer()->GetJob();
	auto size = GameConstants::ShowSkillTabSize(job);
	for (const auto& button : _skill_window_buttons)
	{
		RECT tab_bar = button->GetRect();
		if (i <= size)
		{
			if (PtInRect(&tab_bar, mouse))
			{
				_select_tab = static_cast<SkillTab>(i);
				button->SetState(UiTabButton::ButtonState::kEnable);
				if (i == 0)
				{
					_show_skill_list = SkillManager::GetInstance()->FindSkillList(0);
				}
				else if (i == 1)
				{
					_show_skill_list = SkillManager::GetInstance()->FindSkillList(job / 100 * 100);
				}
				else if (i == 2)
				{
					_show_skill_list = SkillManager::GetInstance()->FindSkillList(job - 2);
				}
				else if (i == 3)
				{
					_show_skill_list = SkillManager::GetInstance()->FindSkillList(job - 1);
				}
				else if (i == 4)
				{
					_show_skill_list = SkillManager::GetInstance()->FindSkillList(job);
				}
				select_button = button;
				_scroll->SetScrollY(0);
				break;
			}
		}
		++i;
	}
	for (const auto& button : _skill_window_buttons)
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

void SkillWindow::SelectSkillIcon(const POINT mouse)
{
	auto paddingsize = 99;

	for (const auto& skill_icon : _show_skill_list)
	{
		const auto icon = skill_icon->GetIconImage();
		RECT icon_rect{ static_cast<int>(_info.x + 6),
			static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()),
					static_cast<int>(_info.x + 6) + skill_icon->GetIconImage()->GetWidth(),
			static_cast<int>(_info.y + paddingsize + _scroll->GetScrollY()) + skill_icon->GetIconImage()->GetHeight() };

		if (PtInRect(&icon_rect, mouse))
		{
			_select_skill = skill_icon;
		}

		paddingsize += _skill_ui_distance;
	}

}

void SkillWindow::DropSkillIcon(POINT mouse)
{
	RECT quick_slot_rect{ 655, 462, 655 + 134, 462 + 63 };
	if (PtInRect(&quick_slot_rect, mouse))
	{
		int totalNum = 0;
		for (int y =0; y <= 2; ++y)
		{
			for (int x = 0; x < 4; ++x)
			{
				//x + 34
				//y + 31
				RECT slot{
					655 + (34 * x),
					462 + (y * 31),
					655 + (34 * x) + 34,
					462 + (31* y) + 31 };

				if (PtInRect(&slot, mouse))
				{
					auto quick_slot = 	UiManager::GetInstance()->GetQuickSlot();
					quick_slot->ChangeSlotSkill(static_cast<QuickSlot::KeyBoard>(totalNum), _select_skill);
				}
				++totalNum;
			}
		}
	}
	_select_skill = nullptr;
}

void SkillWindow::ScrollBarUpClick(const POINT mouse) const
{
	const RECT up_button{
		static_cast<int>(_info.x + 156),
		static_cast<int>(_info.y + 100),
		static_cast<int>(_info.x + 164),
		static_cast<int>(_info.y) + 108 };
	if (PtInRect(&up_button, mouse))
	{
		if (_scroll->GetScrollY() != 0)
		{
			_scroll->GainScrollY(static_cast<float>(_skill_ui_distance));
		}
	}

}

void SkillWindow::ScrollBarDownClick(POINT mouse) const
{
	const RECT up_button{
		static_cast<int>(_info.x + 156),
		static_cast<int>(_info.y + 242),
		static_cast<int>(_info.x + 164),
		static_cast<int>(_info.y) + 250 };
	if (PtInRect(&up_button, mouse))
	{
		const auto max_distance = static_cast<int>(_show_skill_list.size() - 4) * _skill_ui_distance;
		if (-max_distance != _scroll->GetScrollY())
		{
			_scroll->GainScrollY(static_cast<float>(-_skill_ui_distance));
		}
	}
}

bool SkillWindow::InMouserSkillWindow() const
{
	const POINT mouse = InGameScene::GetMouse()->GetPoint();
	const RECT rc = GetRect();
	if (PtInRect(&rc, mouse))
	{
		return true;
	}
	return false;
}

void SkillWindow::SetInfo(const Info info)
{
	_info = info;
}

RECT SkillWindow::GetRect() const
{
	return _rect;
}

Info SkillWindow::GetInfo() const
{
	return _info;
}

void SkillWindow::UpdateRect()
{
	_rect.left = static_cast<LONG>(_info.x);
	_rect.top = static_cast<LONG>(_info.y);
	_rect.right = static_cast<LONG>(_info.x + _info.cx);
	_rect.bottom = static_cast<LONG>(_info.y + _info.cy);
}
