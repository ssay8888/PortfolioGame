#pragma once
class Item;
class SkinInfo;

class Equipment
{

public:

	void InsertEquipItem(const std::shared_ptr<SkinInfo>& skin_info);
	std::map<ObjectType::EquipPosition, std::shared_ptr<SkinInfo>>& GetEquipItems();
	std::shared_ptr<SkinInfo> FindItem(ObjectType::EquipPosition pos);
	void RemoveItem(ObjectType::EquipPosition pos);
	void UseScrollItem(std::shared_ptr<SkinInfo> eqpi_tem, std::shared_ptr<Item> scroll_item);
private:
	ObjectType::EquipPosition FindItemPosition(int32_t item_id);
private:
	std::map<ObjectType::EquipPosition, std::shared_ptr<SkinInfo>> _equip_item;
};

