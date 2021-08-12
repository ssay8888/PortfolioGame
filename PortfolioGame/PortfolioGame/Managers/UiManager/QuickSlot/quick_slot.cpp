#include "../../../pch.h"
#include "quick_slot.h"
#include "../../../Managers/SkillManager/Skill/skill.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"

QuickSlot::QuickSlot() :
	_background(nullptr)
{
}

QuickSlot::~QuickSlot()
{
}

void QuickSlot::ReadyQuickSlot()
{
	_background = std::make_shared<MyBitmap*>(new MyBitmap());
	(*_background)->Insert_Bitmap(_hWnd, L"Client\\Ui\\StatusBar\\quickSlot.bmp");
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
}

void QuickSlot::UpdateQuickSlot()
{
}

void QuickSlot::RenderQuickSlot(HDC hdc)
{
	(*_background)->RenderBitmapImage(hdc, 647, 453, (*_background)->GetWidth(), (*_background)->GetHeight());

	constexpr int max_count = static_cast<int>(KeyBoard::kEnd);
	for (int i = 0; i < max_count; ++i)
	{
		if (_skills[i] != nullptr)
		{
			switch (static_cast<KeyBoard>(i))
			{
			case KeyBoard::kShift:
				_skills[i]->GetIconImage()->RenderBitmapImage(hdc, 655, 462, _skills[i]->GetIconImage()->GetWidth(), _skills[i]->GetIconImage()->GetHeight());
				break;
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

}

Skill* QuickSlot::GetSkill(const KeyBoard key) const
{
	return _skills[static_cast<int>(key)];
}
