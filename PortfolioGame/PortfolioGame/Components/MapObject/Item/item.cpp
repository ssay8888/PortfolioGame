#include "../../../pch.h"
#include "item.h"

Item::Item(): _price(0),
              _slot_max(0),
              _quantity(0)
{
}

Item::~Item()
{
}

void Item::SetIcon(const std::shared_ptr<MyBitmap> icon)
{
	_icon = icon;
}

void Item::SetIconRaw(const std::shared_ptr<MyBitmap> icon_raw)
{
	_icon_raw = icon_raw;
}

std::shared_ptr<MyBitmap> Item::GetIcon() const
{
	return _icon;
}

std::shared_ptr<MyBitmap> Item::GetIconRaw() const
{
	return _icon_raw;
}

void Item::SetPrice(const int32_t price)
{
	_price = price;
}

int32_t Item::GetPrice() const
{
	return _price;
}

void Item::SetSlotMax(const int32_t slot_max)
{
	_slot_max = slot_max;
}

int32_t Item::GetSlotMax() const
{
	return _slot_max;
}

void Item::SetQuantity(int32_t quantity)
{
	_quantity = quantity;
}

void Item::GainQuantity(int32_t quantity)
{
	_quantity += quantity;
}

void Item::SetItemId(int32_t item_id)
{
	_item_id = item_id;
}

int32_t Item::GetItemId() const
{
	return _item_id;
}

int32_t Item::Getquantity() const
{
	return _quantity;
}

void Item::InsertSpec(const std::string& name, int32_t value)
{
	_spec.insert(std::make_pair(name, value));
}

std::map<std::string, int32_t>& Item::GetSpec()
{
	return _spec;
}
