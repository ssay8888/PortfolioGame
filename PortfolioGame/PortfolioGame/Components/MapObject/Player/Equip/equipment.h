#pragma once
class SkinInfo;

class Equipment
{

public:

	void InsertEquipItem(const std::shared_ptr<SkinInfo>& skin_info);
	std::map<ObjectType::EquipPosition, std::shared_ptr<SkinInfo>>& GetEquipItems();
	std::shared_ptr<SkinInfo> FindItem(ObjectType::EquipPosition pos);
	void RemoveItem(ObjectType::EquipPosition pos);
private:
	ObjectType::EquipPosition FindItemPosition(int32_t item_id);
private:
	std::map<ObjectType::EquipPosition, std::shared_ptr<SkinInfo>> _equip_item;
};

