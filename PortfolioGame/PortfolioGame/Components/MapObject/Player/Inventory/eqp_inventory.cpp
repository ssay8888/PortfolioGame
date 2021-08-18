#include "../../../../pch.h"
#include "eqp_inventory.h"
#include "../../../../Managers/Skins/skin_info.h"

EqpInventory::EqpInventory()
{
}

EqpInventory::~EqpInventory()
{
}

void EqpInventory::AddItem(int32_t slot, std::shared_ptr<SkinInfo> item)
{
	_item[slot] = item;
}

std::shared_ptr<SkinInfo> EqpInventory::GetItem(int32_t slot)
{
	return _item[slot];
}

std::shared_ptr<SkinInfo>* EqpInventory::GetItem()
{
	return _item;
}

void EqpInventory::MoveItem(int32_t src, int32_t dst)
{
	std::shared_ptr<SkinInfo> dst_item = nullptr;
	if (_item[dst] != nullptr)
	{
		dst_item = _item[dst];
	}

	_item[dst] = _item[src];
	_item[src] = dst_item;
}

void EqpInventory::ReMoveItem(int32_t src)
{
	_item[src] = nullptr;
}

void EqpInventory::ReMoveItem(std::shared_ptr<SkinInfo> src)
{
	for (int i = 0; i < inventory_slot_max; ++i)
	{
		if (_item[i] == src)
		{
			_item[i] = nullptr;
		}
	}
}

int32_t EqpInventory::FindFreeSlot() const
{
	for (int i = 0; i < inventory_slot_max; ++i)
	{
		if (_item[i] == nullptr)
		{
			return i;
		}
	}
	return -1;
}
