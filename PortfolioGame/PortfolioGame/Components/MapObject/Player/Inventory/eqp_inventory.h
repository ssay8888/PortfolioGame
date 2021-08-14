#pragma once
class SkinInfo;

class EqpInventory
{
public:
	EqpInventory();
	~EqpInventory();

	void AddItem(int32_t slot, std::shared_ptr<SkinInfo> item);
	std::shared_ptr<SkinInfo> GetItem(int32_t slot);

	std::shared_ptr<SkinInfo>* GetItem();

	void MoveItem(int32_t src, int32_t dst);
private:

	std::shared_ptr<SkinInfo> _item[inventory_slot_max]{ nullptr };
};

