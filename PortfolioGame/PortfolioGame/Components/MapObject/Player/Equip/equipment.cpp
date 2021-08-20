#include "../../../../pch.h"
#include "equipment.h"

#include "../player.h"
#include "../../../../Managers/Skins/skin_info.h"
#include "../../../../Managers/MapManager/map_manager.h"
#include "../../Item/item.h"
#include "../Inventory/eqp_inventory.h"
#include "../../../../Managers/EffectManager/effect_manager.h"

void Equipment::InsertEquipItem(const std::shared_ptr<SkinInfo>& skin_info)
{
	auto player = MapManager::GetInstance()->GetPlayer();
	auto data = _equip_item.find(FindItemPosition(skin_info->GetItemId()));
	if (data != _equip_item.end())
	{
		auto freeslot = player->GetEqpInventory()->FindFreeSlot();
		if (freeslot == -1)
		{
			return;
		}
		player->GetEqpInventory()->AddItem(freeslot, data->second);
		_equip_item.erase(data);
	}
	_equip_item.insert(std::make_pair(FindItemPosition(skin_info->GetItemId()), skin_info));
}

std::map<ObjectType::EquipPosition, std::shared_ptr<SkinInfo>>& Equipment::GetEquipItems()
{
	return _equip_item;
}

std::shared_ptr<SkinInfo> Equipment::FindItem(ObjectType::EquipPosition pos)
{
	auto data = _equip_item.find(pos);
	if (data != _equip_item.end())
	{
		return data->second;
	}
	return nullptr;
}

void Equipment::RemoveItem(ObjectType::EquipPosition pos)
{
	_equip_item.erase(pos);
}

void Equipment::UseScrollItem(std::shared_ptr<SkinInfo> eqpi_tem, std::shared_ptr<Item> scroll_item)
{
	if (scroll_item != nullptr && eqpi_tem != nullptr)
	{
		if (eqpi_tem->GetItemInfo().GetTuc() > 0)
		{
			if (scroll_item->GetSuccess() >= rand() % 100 + 1)
			{
				eqpi_tem->GetItemInfo().SetIncInt(eqpi_tem->GetItemInfo().GetIncInt() + scroll_item->GetIncInt());
				eqpi_tem->GetItemInfo().SetIncMad(eqpi_tem->GetItemInfo().GetIncMad() + scroll_item->GetIncMad());
				eqpi_tem->GetItemInfo().SetSucessTuc(eqpi_tem->GetItemInfo().GetSucessTuc() + 1);
				EffectManager::GetInstance()->ShowEffect("Success");
			}
			else
			{
				EffectManager::GetInstance()->ShowEffect("Failure");
			}
			eqpi_tem->GetItemInfo().SetTuc(eqpi_tem->GetItemInfo().GetTuc() - 1);
		}
	}
}

ObjectType::EquipPosition Equipment::FindItemPosition(const int32_t item_id)
{
	if (item_id >= 1000000 && item_id <= 1009999)
	{
		return ObjectType::EquipPosition::kCap;
	}
	else if (item_id >= 1040000 && item_id <= 1049999)
	{
		return ObjectType::EquipPosition::kCoat;
	}
	else if (item_id >= 1060000 && item_id <= 1069999)
	{
		return ObjectType::EquipPosition::kPants;
	}
	else if (item_id >= 1070000 && item_id <= 1079999)
	{
		return ObjectType::EquipPosition::kShoes;
	}
	else if (item_id >= 1300000 && item_id <= 1709999)
	{
		return ObjectType::EquipPosition::kWeapon;
	}
	return ObjectType::EquipPosition::kEquipEnd;
}
