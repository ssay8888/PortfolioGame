#include "../../../../pch.h"
#include "damage_info.h"
#include "../../../Base/game_object.h"
#include "../../../../Managers/EffectManager/effect_manager.h"
#include "../../../../Managers/ScrollManager/scroll_manager.h"
#include "../../../../../Common/Managers/BitmapManager/my_bitmap.h"

DamageInfo::DamageInfo(GameObject* target, uint32_t damage, uint32_t delay) :
	_targetObject(target),
	_damage(damage),
	_delay(delay),
	_tick(GetTickCount64()),
	_is_alive(true),
	_position({ static_cast<int>(target->GetInfo().x), static_cast<int>(target->GetInfo().y - (target->GetInfo().cy >> 1)) })
{
	ReadyDamage();
}

DamageInfo::~DamageInfo()
{
}

void DamageInfo::ReadyDamage()
{
	auto instance = EffectManager::GetInstance();
	std::list<MyBitmap*> list;
	if (_damage == 0)
	{
		_damage_image.emplace_back(instance->GetAttackDamageNumber(10));
		return;
	}

	auto temp_damage = _damage;
	while (temp_damage != 0)
	{
		_damage_image.emplace_front(instance->GetAttackDamageNumber(temp_damage % 10));
		temp_damage /= 10;
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
		int totalWidth = 0;
		int width = 0;
		for (auto& damage : _damage_image)
		{
			totalWidth += (*damage)->GetWidth();
		}
		totalWidth >>= 1;
		for (auto& damage : _damage_image)
		{
			(*damage)->RenderBitmapImage(hdc,
				static_cast<int>(_position.x - totalWidth + width + ScrollManager::GetScrollX()),
				static_cast<int>(_position.y - (_targetObject->GetInfo().cy >> 1) + ScrollManager::GetScrollY()),
				(*damage)->GetWidth(),
				(*damage)->GetHeight());
			width += (*damage)->GetWidth();
		}
		_position.y -= 1;
	}
}

bool DamageInfo::IsAlive()
{
	return _is_alive;
}
