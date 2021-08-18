#include "../../pch.h"
#include "shop_manager.h"
#include "../../Utility/xml_reader.h"

ShopManager::ShopManager()
{
}

ShopManager::~ShopManager()
{
}

void ShopManager::InsertShop(int32_t shop_id, std::vector<std::shared_ptr<ShopItem>> items)
{
	_shop.insert(std::make_pair(shop_id, items));
}

std::vector<std::shared_ptr<ShopItem>> ShopManager::FindShop(int32_t shop_id)
{
	auto data = _shop.find(shop_id);
	if (data != _shop.end())
	{
		return data->second;
	}

	return std::vector<std::shared_ptr<ShopItem>>();
}

void ShopManager::LoadShopData()
{
	XmlReader::GetInstance().LoadShop();
}
