#include "../../pch.h"
#include "drop_data_manager.h"
#include "drop_data_info.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../Utility/xml_reader.h"
#include "../../Components/Base/game_object.h"
#include "../../Components/MapObject/Item/item.h"
#include "../MapManager/map_manager.h"
#include "../MapManager/Map/map_instance.h"
#include "../ItemManager/item_manager.h"


DropDataManager::DropDataManager()
{
}

DropDataManager::~DropDataManager()
{
}

void DropDataManager::InsertDropData(int32_t mob_id, std::list<std::shared_ptr<DropDataInfo>> info)
{
	_drop_info_list.insert(std::make_pair(mob_id, info));
}

std::list<std::shared_ptr<DropDataInfo>> DropDataManager::FindDropInfo(const int32_t mob_id)
{
	auto data = _drop_info_list.find(mob_id);
	if (data != _drop_info_list.end())
	{
		return data->second;
	}
	return std::list<std::shared_ptr<DropDataInfo>>();
}

void DropDataManager::DropFromMonster(int32_t monster_id, GameObject* object)
{
	auto data_list = FindDropInfo(monster_id);

	int plusX = 0;
	for (auto info : data_list)
	{
		if (rand() % 10000 < info->GetRate())
		{
			int price = rand() % info->GetMaxPrice() + info->GetMinPrice();
			std::shared_ptr<Item> item = nullptr;
			if (info->GetItemId() == 0)
			{
				if (price < 50)
				{
					item = ItemManager::GetInstance()->FindCopyItem(9000000);
				}
				else if (price < 100)
				{
					item = ItemManager::GetInstance()->FindCopyItem(9000001);
				}
				else if (price < 1000)
				{
					item = ItemManager::GetInstance()->FindCopyItem(9000002);
				}
				else
				{
					item = ItemManager::GetInstance()->FindCopyItem(9000003);
				}
			}
			else
			{
				item = ItemManager::GetInstance()->FindCopyItem(info->GetItemId());
			}
			if (item != nullptr)
			{
				item->SetQuantity(price);
				POINT pos{ static_cast<int>(object->GetInfo().x) + plusX, static_cast<int>(object->GetRect().bottom) };
				MapManager::GetInstance()->GetNowMap()->AddDropItem(std::make_pair(pos, item));
			}
			plusX += 30;
		}
	}
}

void DropDataManager::LoadDropData()
{
	XmlReader::GetInstance().LoadDropData();
}