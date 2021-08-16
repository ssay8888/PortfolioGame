#include "../../pch.h"
#include "drop_data_info.h"

DropDataInfo::DropDataInfo(): _item_id(0),
                              _rate(0),
                              _min_price(0),
                              _max_price(0)
{
}

DropDataInfo::~DropDataInfo()
{
}

void DropDataInfo::SetItemId(int32_t item_id)
{
	_item_id = item_id;
}

void DropDataInfo::SetRate(int32_t rate)
{
	_rate = rate;
}

void DropDataInfo::SetMinPrice(int32_t min_price)
{
	_min_price = min_price;
}

void DropDataInfo::SetMaxPrice(int32_t max_price)
{
	_max_price = max_price;
}

int32_t DropDataInfo::GetItemId() const
{
	return _item_id;
}

int32_t DropDataInfo::GetRate() const
{
	return _rate;
}

int16_t DropDataInfo::GetMinPrice() const
{
	return _min_price;
}

int16_t DropDataInfo::GetMaxPrice() const
{
	return _max_price;
}
