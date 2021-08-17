#pragma once
class Item;
class Inventory
{
public:
	Inventory();
	~Inventory();
	void AddItem(int32_t slot, std::shared_ptr<Item> item);
	std::shared_ptr<Item> GetItem(int32_t slot);
	std::shared_ptr<Item>* GetItem();
	void MoveItem(int32_t src, int32_t dst);
	int32_t FindFreeSlot() const;
	int32_t TotalQuantity(int32_t item_id);
private:

	std::shared_ptr<Item> _item[inventory_slot_max]{nullptr};
};

