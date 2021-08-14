#include "../../pch.h"
#include "item_manager.h"
#include "../../Components/MapObject/Item/item.h"
#include "../../Utility/xml_reader.h"

ItemManager::ItemManager()
{
}

ItemManager::~ItemManager()
{
}

void ItemManager::LoadItem()
{
	XmlReader::GetInstance().LoadItem("Consume\\0200.img");
	XmlReader::GetInstance().LoadItem("Etc\\0400.img");
}

void ItemManager::InsertItem(int32_t item_id, std::shared_ptr<Item> item)
{
	_list_item.insert(std::make_pair(item_id, item));
}

std::shared_ptr<Item> ItemManager::FindItem(int32_t item_id)
{
	auto data = _list_item.find(item_id);
	if(data != _list_item.end())
	{
		return data->second;
	}
	return nullptr;
}

std::shared_ptr<Item> ItemManager::FindCopyItem(int32_t item_id)
{
	const auto data = _list_item.find(item_id);
	if (data != _list_item.end())
	{
		auto item = std::make_shared<Item>(Item((*data->second)));
		return item;
	}
	return nullptr;
}
