#include "../../../../pch.h"
#include "damage_handler.h"
#include "../player.h"
#include "damage_info.h"
#include "../../../Base/game_object.h"
#include "../../../../Managers/EffectManager/effect_manager.h"
#include "../../../../../Common/Managers/BitmapManager/my_bitmap.h"


void DamageHandler::InsertAttackDamageEffect(GameObject* target, uint32_t damage, uint32_t delay)
{
	DamageInfo* info = new DamageInfo(target, damage, delay);

	_show_damage_list.push_back(info);
}

void DamageHandler::InsertTakeDamageEffect(GameObject* target, uint32_t damage, uint32_t delay)
{
	DamageInfo* info = new DamageInfo(target, damage, delay, false);

	_show_damage_list.push_back(info);
}

void DamageHandler::ShowDamages(HDC hdc)
{
	for (auto& data: _show_damage_list)
	{
		data->RenderDamage(hdc);
	}
}

void DamageHandler::UpdateDamages()
{
	for (auto begin = _show_damage_list.begin(); begin != _show_damage_list.end();)
	{
		(*begin)->UpdateDamages();
		if (!(*begin)->IsAlive()) {
			delete *begin;
			begin = _show_damage_list.erase(begin);
		}
		else
		{
			++begin;
		}
	}
}
