#include "../../pch.h"
#include "shop_item.h"

ShopItem::ShopItem()
{
}

ShopItem::~ShopItem()
{
}

void ShopItem::SetItemId(const int32_t item_id)
{
	_item_id = item_id;
}

void ShopItem::SetPrice(const int32_t price)
{
	_price = price;
}

void ShopItem::SetMeso(const int32_t meso)
{
	_meso = meso;
}

int32_t ShopItem::GetItemId() const
{
	return _item_id;
}

int32_t ShopItem::GetPrice() const
{
	return _price;
}

int32_t ShopItem::GetMeso() const
{
	return _meso;
}
