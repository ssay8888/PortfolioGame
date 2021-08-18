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
	void ReMoveItem(int32_t src);
	void ReMoveItem(std::shared_ptr<SkinInfo> src);

	int32_t FindFreeSlot() const;
private:

	std::shared_ptr<SkinInfo> _item[inventory_slot_max]{ nullptr };
};

