#include "../../../../pch.h"
#include "magic_attack.h"

#include <utility>

#include "../player.h"
#include "../../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../../Managers/MonsterMnager/monster_parts.h"
#include "../../../../Managers/ScrollManager/scroll_manager.h"
#include "../../../../Managers/SkillManager/Skill/skill.h"
#include "../../../../Managers/SkillManager/Skill/skill_effect_image.h"
#include "../../Monster/monster.h"

MagicAttack::MagicAttack(Skill* skill, Player* player, std::list<Monster*> target) :
	_skill(skill),
	_targets(),
	_player(player),
	_ball_frame_number(0),
	_ball_frame_delay(0),
	_is_ball_frame_finished(false),
	_effect_frame_number(0),
	_effect_frame_delay(0),
	_is_effect_frame_finished(false),
	_hit_frame_number(0),
	_hit_frame_delay(0),
	_is_hit_frame_finished(false),
	_memDC(nullptr),
	_bitmap(nullptr),
	_old_bitmap(nullptr)
{
	for (auto& monster : target)
	{
		auto copy = new Monster(*monster);
		copy->DoReadyGame();
		_targets.emplace_back(copy);
	}
}

MagicAttack::~MagicAttack()
{
	SelectObject(_memDC, _old_bitmap);
	DeleteObject(_bitmap);
	DeleteDC(_memDC);
}

void MagicAttack::ReadyMagicAttack()
{
	const HDC hdc = GetDC(_hWnd);
	_memDC = CreateCompatibleDC(hdc);
	_bitmap = CreateCompatibleBitmap(hdc, 500, 500);
	_old_bitmap = static_cast<HBITMAP>(SelectObject(_memDC, _bitmap));
	ReleaseDC(_hWnd, hdc);
}

void MagicAttack::SkillUpdate() const
{
	if (_skill == nullptr)
	{
		return;
	}

}

void MagicAttack::SkillRender(const HDC hdc)
{
	RunEffectImage(hdc);
	RunHitImage(hdc);
	RunBallImage(hdc);
}

void MagicAttack::BallSkill()
{
}

void MagicAttack::MagicSkill()
{

}

void MagicAttack::RunEffectImage(HDC hdc)
{
	if (_is_effect_frame_finished)
		return;
	const auto effects = _skill->GetSkillEffectImage();
	if (effects.size() > _effect_frame_number)
	{
		const auto effect = effects[_effect_frame_number];
		const auto image = effect->GetImage();
		const HBRUSH brush = CreateSolidBrush(RGB(255, 0, 255));
		const HBRUSH brush_prev = static_cast<HBRUSH>(SelectObject(_memDC, brush));
		Rectangle(_memDC, -10, -10, 310, 310);
		SelectObject(_memDC, brush_prev);
		DeleteObject(brush);
		DeleteObject(brush_prev);
		image->RenderBitmapImage(_memDC,
			0,
			0,
			image->GetWidth(), image->GetHeight());

		if (_player->GetFacingDirection())
		{
			StretchBlt(_memDC, 0, 0, image->GetWidth(), image->GetHeight(), _memDC, image->GetWidth() - 1, 0, -image->GetWidth(), image->GetHeight(), SRCCOPY);

			GdiTransparentBlt(hdc,
			                  static_cast<int>(_player->GetInfo().x + (_player->GetInfo().cx / 2) - effect->GetOrigin().
				                  x + ScrollManager::GetScrollX()),
			                  static_cast<int>(_player->GetInfo().y + (_player->GetInfo().cy / 2) - effect->GetOrigin().
				                  y + ScrollManager::GetScrollY()),
			                  image->GetWidth(),
			                  image->GetHeight(),
			                  _memDC,
			                  0,
			                  0,
			                  image->GetWidth(),
			                  image->GetHeight(),
			                  RGB(255, 0, 255));

		}
		else
		{
			GdiTransparentBlt(hdc,
			                  static_cast<int>(_player->GetInfo().x - effect->GetOrigin().x +
				                  ScrollManager::GetScrollX()),
			                  static_cast<int>(_player->GetInfo().y + (_player->GetInfo().cy / 2) - effect->GetOrigin().
				                  y + ScrollManager::GetScrollY()),
			                  image->GetWidth(),
			                  image->GetHeight(),
			                  _memDC,
			                  0,
			                  0,
			                  image->GetWidth(),
			                  image->GetHeight(),
			                  RGB(255, 0, 255));


		}

		if (GetTickCount64() > _effect_frame_delay + effect->GetDelay())
		{
			++_effect_frame_number;
			_effect_frame_delay = GetTickCount64();
		}
	}
	else
	{
		_is_effect_frame_finished = true;
		if (_finishied_delay_tick == 0)
		{
			_finishied_delay_tick = GetTickCount64();
		}
	}
}

void MagicAttack::RunHitImage(HDC hdc)
{
	if (_is_hit_frame_finished)
		return;
	for (const auto& monster : _targets)
	{
		if (monster == nullptr)
		{
			continue;
		}
		const auto effects = _skill->GetHitImage();
		if (effects.size() > _hit_frame_number)
		{
			const auto effect = effects[_hit_frame_number];
			const auto image = effect->GetImage();

			HBRUSH brush = CreateSolidBrush(RGB(255, 0, 255));
			HBRUSH brushPrev = (HBRUSH)SelectObject(_memDC, brush);
			Rectangle(_memDC, -10, -10, 500, 500);
			SelectObject(_memDC, brushPrev);
			DeleteObject(brush);
			DeleteObject(brushPrev);
			image->RenderBitmapImage(_memDC,
				0,
				0,
				image->GetWidth(), image->GetHeight());
			GdiTransparentBlt(hdc,
				static_cast<int>((monster->GetInfo().x - (monster->GetThisFrame()->GetOriginPos().x / 2)) - effect->GetOrigin().x + ScrollManager::GetScrollX()),
				static_cast<int>((monster->GetRect().bottom) - effect->GetOrigin().y  + ScrollManager::GetScrollY()),
				image->GetWidth(),
				image->GetHeight(),
				_memDC,
				0,
				0,
				image->GetWidth(),
				image->GetHeight(),
				RGB(255, 0, 255));


			if (GetTickCount64() > _hit_frame_delay + effect->GetDelay())
			{
				++_hit_frame_number;
				_hit_frame_delay = GetTickCount64();
			}
		}
		else
		{
			_is_hit_frame_finished = true;
			break;
		}
	}
	if (_targets.empty())
	{
		_is_hit_frame_finished = true;
		if (_finishied_delay_tick == 0)
		{
			_finishied_delay_tick = GetTickCount64();
		}
	}
}

void MagicAttack::RunBallImage(HDC hdc)
{
	if (_is_ball_frame_finished)
		return;
	const auto effects = _skill->GetBallEffectImage();
	if (effects.size() > _ball_frame_number)
	{
		const auto effect = effects[_ball_frame_number];
		const auto image = effect->GetImage();
		const HBRUSH brush = CreateSolidBrush(RGB(255, 0, 255));
		const HBRUSH brush_prev = static_cast<HBRUSH>(SelectObject(_memDC, brush));
		Rectangle(_memDC, -10, -10, 310, 310);
		SelectObject(_memDC, brush_prev);
		DeleteObject(brush);
		DeleteObject(brush_prev);
		image->RenderBitmapImage(_memDC,
			0,
			0,
			image->GetWidth(), image->GetHeight());

		if (_ball_frame_number == 0)
		{
			_ball_pos = {
				static_cast<int>(_player->GetInfo().x - effect->GetOrigin().x + ScrollManager::GetScrollX()),
				static_cast<int>(_player->GetInfo().y - effect->GetOrigin().y + ScrollManager::GetScrollY()),
			};
		}
		else
		{
			if (_player->GetFacingDirection())
			{
				_ball_pos.x += 15;
			}
			else
			{
				_ball_pos.x -= 15;
			}
		}
		if (_player->GetFacingDirection())
		{
			StretchBlt(_memDC, 0, 0, image->GetWidth(), image->GetHeight(), _memDC, image->GetWidth() - 1, 0, -image->GetWidth(), image->GetHeight(), SRCCOPY);

			GdiTransparentBlt(hdc,
				static_cast<int>(_ball_pos.x),
				static_cast<int>(_ball_pos.y),
				image->GetWidth(),
				image->GetHeight(),
				_memDC,
				0,
				0,
				image->GetWidth(),
				image->GetHeight(),
				RGB(255, 0, 255));
		}
		else
		{
			GdiTransparentBlt(hdc,
				static_cast<int>(_ball_pos.x),
				static_cast<int>(_ball_pos.y),
				image->GetWidth(),
				image->GetHeight(),
				_memDC,
				0,
				0,
				image->GetWidth(),
				image->GetHeight(),
				RGB(255, 0, 255));

		}

		if (GetTickCount64() > _ball_frame_delay + effect->GetDelay())
		{
			++_ball_frame_number;
			_ball_frame_delay = GetTickCount64();
		}
	}
	else
	{
		_is_ball_frame_finished = true;
		if (_finishied_delay_tick == 0)
		{
			_finishied_delay_tick = GetTickCount64();
		}
	}
}

void MagicAttack::ResetSkill(Skill* skill, Player* player, std::list<Monster*> target)
{
	_skill = skill;
	for (auto& monster : _targets)
	{
		delete monster;
		monster = nullptr;
	}
	_targets.clear();
	for (auto& monster : target)
	{
		auto copy = new Monster(*monster);
		copy->DoReadyGame();
		_targets.emplace_back(copy);
	}
	_player = player;
	_is_ball_frame_finished = false;
	_ball_frame_number = 0;
	_ball_frame_delay = 0;
	_is_effect_frame_finished = false;
	_effect_frame_number = 0;
	_effect_frame_delay = 0;
	_is_hit_frame_finished = false;
	_hit_frame_number = 0;
	_hit_frame_delay = 0;
	_finishied_delay_tick = 0;
}

bool MagicAttack::IsFinishedFrame()
{
	if (_is_ball_frame_finished && _is_effect_frame_finished && _is_hit_frame_finished)
	{
		if (GetTickCount64() > _finishied_delay_tick + 850)
		{
			return true;
		}
	}
	return false;
}
