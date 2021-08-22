#include "../../../pch.h"
#include "buff_ui.h"

#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../Skill/skill.h"
#include "../Skill/skill_info.h"

BuffUi::BuffUi(Skill* skill): _start_tick(GetTickCount64()),
                              _skill(skill),
                              _is_flicker(false),
                              _is_dead(false)
{
}

BuffUi::~BuffUi()
{
}

bool BuffUi::UpdateBuffUi()
{
    if (_is_dead)
    {
        return true;
    }
    auto skill_info = _skill->GetSkillInfo()[_skill->GetSkillInfo().size() - 1];
    auto time_tick = (_start_tick + (skill_info->GetTime() * 1000)) - 5000;
    if (time_tick < 0)
    {
        time_tick = 0;
    }
    if (!_is_flicker &&
        GetTickCount64() > static_cast<uint64_t>(time_tick))
    {
        _is_flicker = true;
    }
    if (GetTickCount64() > static_cast<uint64_t>(_start_tick + (skill_info->GetTime() * 1000)))
    {
        _is_dead = true;
    }
    return _is_dead;
}

void BuffUi::RenderBuffUi(HDC hdc, POINT pos)
{
    if (_is_flicker)
    {
        if (rand() % 2)
            return;
    }
    auto icon = _skill->GetIconImage();
    icon->RenderBitmapImage(hdc, pos.x, pos.y, icon->GetWidth(), icon->GetHeight());
    wchar_t str[20];
    auto skill_info = _skill->GetSkillInfo()[_skill->GetSkillInfo().size() - 1];
    uint64_t end_tick = static_cast<uint64_t>(_start_tick + (skill_info->GetTime() * 1000));
    uint64_t remainTime = end_tick - GetTickCount64();

    swprintf_s(str, 20, L"%d", static_cast<int>(remainTime / 1000));
	StringTools::CreateTextOut(hdc, pos.x + 32, pos.y + 32, str, 11, RGB(0, 0, 0), L"µ¸¿ò", TA_RIGHT);
}

Skill* BuffUi::GetSkill() const
{
    return _skill;
}
