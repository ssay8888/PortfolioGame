#include "../../../../pch.h"
#include "inventory.h"

Inventory::Inventory()
= default;

Inventory::~Inventory()
{
}

void Inventory::AddItem(int32_t slot, std::shared_ptr<Item> item)
{
	if (slot >= inventory_slot_max)
	{
		return;
	}

	_item[slot] = item;

}

std::shared_ptr<Item> Inventory::GetItem(int32_t slot)
{
	return _item[slot];
}

std::shared_ptr<Item>* Inventory::GetItem()
{
	return _item;
}

void Inventory::MoveItem(const int32_t src, const int32_t dst)
{
	std::shared_ptr<Item> dst_item = nullptr;
	if (_item[dst] != nullptr)
	{
		dst_item = _item[dst];
	}

	_item[dst] = _item[src];
	_item[src] = dst_item;

}

int32_t Inventory::FindFreeSlot() const
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
