#include "../../../pch.h"
#include "quick_slot.h"
#include "../../../Managers/SkillManager/Skill/skill.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../Components/MapObject/Item/item.h"
#include "../../../../Common/Utility/string_tools.h"

QuickSlot::QuickSlot() :
	_background(nullptr)
{
}

QuickSlot::~QuickSlot()
{
}

void QuickSlot::ReadyQuickSlot()
{
	_background = std::make_shared<MyBitmap>(MyBitmap());
	_background->Insert_Bitmap(_hWnd, L"Client\\Ui\\StatusBar\\quickSlot.bmp");
}

void QuickSlot::ChangeSlotSkill(KeyBoard key, Skill* skill)
{
	for (int i = 0; i < 8; ++i)
	{
		if(_skills[i] == skill)
		{
			_skills[i] = nullptr;
		}
	}
	_skills[static_cast<int>(key)] = skill;
	_items[static_cast<int>(key)] = nullptr;
}

void QuickSlot::ChangeSlotItem(KeyBoard key, std::shared_ptr<Item> item)
{
	for (int i = 0; i < 8; ++i)
	{
		if (_items[i] == item)
		{
			_items[i] = nullptr;
		}
	}
	_skills[static_cast<int>(key)] = nullptr;
	_items[static_cast<int>(key)] = item;
}

void QuickSlot::UpdateQuickSlot()
{
}

void QuickSlot::RenderQuickSlot(HDC hdc)
{
	_background->RenderBitmapImage(hdc, 647, 453, _background->GetWidth(), _background->GetHeight());

	constexpr int max_count = static_cast<int>(KeyBoard::kEnd);
	for (int i = 0; i < max_count; ++i)
	{
		if (_skills[i] != nullptr)
		{
			switch (static_cast<KeyBoard>(i))
			{
			case KeyBoard::kShift:
			case KeyBoard::kInsert:
			case KeyBoard::kHome:
			case KeyBoard::kPageUp:
				_skills[i]->GetIconImage()->RenderBitmapImage(hdc, 655 + (34 * i), 462, _skills[i]->GetIconImage()->GetWidth(), _skills[i]->GetIconImage()->GetHeight());
				break;
			case KeyBoard::kControl:
			case KeyBoard::kDelete:
			case KeyBoard::kEndKey:
			case KeyBoard::kPageDown:
				_skills[i]->GetIconImage()->RenderBitmapImage(hdc, 655 + (34 * (i % 4)), 462 + 31, _skills[i]->GetIconImage()->GetWidth(), _skills[i]->GetIconImage()->GetHeight());
				break;
			case KeyBoard::kEnd:
			default:
				break;
			}
		}
	}

	for (int i = 0; i < max_count; ++i)
	{
		if (_items[i] != nullptr)
		{
			switch (static_cast<KeyBoard>(i))
			{
			case KeyBoard::kShift:
			case KeyBoard::kInsert:
			case KeyBoard::kHome:
			case KeyBoard::kPageUp:
			{
				_items[i]->GetIconRaw()->RenderBitmapImage(hdc,
					655 + (34 * i),
					462,
					_items[i]->GetIconRaw()->GetWidth(),
					_items[i]->GetIconRaw()->GetHeight());
				std::wstring str;
				str.append(std::to_wstring(_items[i]->GetQuantity()));
				StringTools::CreateTextOut(hdc,
					655 + (34 * (i % 4)) + 17,
					462, str.c_str(), 10, RGB(0, 0 ,0), L"±¼¸²Ã¼", TA_TOP, true);
				break;
			}
			case KeyBoard::kControl:
			case KeyBoard::kDelete:
			case KeyBoard::kEndKey:
			case KeyBoard::kPageDown:
			{
				_items[i]->GetIconRaw()->RenderBitmapImage(hdc,
					655 + (34 * (i % 4)),
					462 + 31,
					_items[i]->GetIconRaw()->GetWidth(),
					_items[i]->GetIconRaw()->GetHeight());
				std::wstring str;
				str.append(std::to_wstring(_items[i]->GetQuantity()));
				StringTools::CreateTextOut(hdc,
					655 + (34 * (i % 4)) + 17,
					462 + 31, str.c_str(), 10, RGB(0, 0, 0), L"±¼¸²Ã¼", TA_TOP, true);
			}
				break;
			case KeyBoard::kEnd:
			default:
				break;
			}
		}
	}

}

Skill* QuickSlot::GetSkill(const KeyBoard key) const
{
	return _skills[static_cast<int>(key)];
}

std::shared_ptr<Item> QuickSlot::GetItem(KeyBoard key) const
{
	return _items[static_cast<int>(key)];
}
