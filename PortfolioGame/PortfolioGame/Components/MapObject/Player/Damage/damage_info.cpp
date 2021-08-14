#include "../../../../pch.h"
#include "damage_info.h"

#include <utility>
#include "../../../Base/game_object.h"
#include "../../../../Managers/EffectManager/effect_manager.h"
#include "../../../../Managers/ScrollManager/scroll_manager.h"
#include "../../../../../Common/Managers/BitmapManager/my_bitmap.h"

DamageInfo::DamageInfo(GameObject* target, uint32_t damage, const uint32_t delay, const bool is_attack) :
	_targetObject(target),
	_damages(),
	_delay(delay),
	_tick(GetTickCount64()),
	_is_alive(true)
{
	_damages.emplace_back(damage);
	ReadyDamage(is_attack);
}

DamageInfo::DamageInfo(GameObject* target, std::list<uint32_t> damage, const uint32_t delay, bool is_attack) :
	_targetObject(target),
	_damages(std::move(damage)),
	_delay(delay),
	_tick(GetTickCount64()),
	_is_alive(true)
{
	ReadyDamage(is_attack);
}

DamageInfo::~DamageInfo()
{
}

void DamageInfo::ReadyDamage(bool is_attack)
{
	const auto instance = EffectManager::GetInstance();

	const int upY = 33;
	int i = 0;
	for (const auto damage : _damages)
	{
		POINT point =
		{ static_cast<int>(_targetObject->GetInfo().x),
		static_cast<int>(_targetObject->GetInfo().y - upY + (upY * i) )
		};
		auto temp_damage = damage;
		std::list<std::shared_ptr<MyBitmap>> dam;

		if (damage == 0)
		{
			if (is_attack)
			{

				dam.emplace_back(instance->GetAttackDamageNumber(10));
			}
			else
			{
				dam.emplace_back(instance->GetTakeDamageNumber(10));
			}
		}
		else
		{
			while (temp_damage != 0)
			{

				if (is_attack)
				{
					dam.emplace_front(instance->GetAttackDamageNumber(temp_damage % 10));
				}
				else
				{
					dam.emplace_front(instance->GetTakeDamageNumber(temp_damage % 10));
				}
				temp_damage /= 10;
			}
		}
		_damage_image.emplace_back(std::make_pair(point, dam));
		++i;
	}
}

void DamageInfo::UpdateDamages()
{
	if (GetTickCount64() > _tick + _delay)
	{
		//½Ã°£ÀÌÁö³µÀ¸¹Ç·Î ÀÌ°´Ã¼´Â Á×À½
		_is_alive = false;
		return;
	}
}

void DamageInfo::RenderDamage(HDC hdc)
{
	if (_is_alive)
	{
		for (auto damage_image : _damage_image)
		{
			int totalWidth = 0;
			int width = 0;
			for (auto& damage : damage_image.second)
			{
				totalWidth += damage->GetWidth();
			}
			totalWidth >>= 1;
			for (auto& damage : damage_image.second)
			{
				damage->RenderBitmapImage(hdc,
					static_cast<int>(damage_image.first.x - totalWidth + width + ScrollManager::GetScrollX()),
					static_cast<int>(damage_image.first.y - _targetObject->GetInfo().cy + ScrollManager::GetScrollY()),
					damage->GetWidth(),
					damage->GetHeight());
				width += damage->GetWidth();
			}
			damage_image.first.y -= 1;
		}
	}
}

bool DamageInfo::IsAlive()
{
	return _is_alive;
}
