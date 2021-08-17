#include "../../../../pch.h"
#include "quest_check.h"

QuestCheck::QuestCheck(): _item_id(0),
                          _price(0)
{
}

QuestCheck::~QuestCheck()
{
}

void QuestCheck::SetItemId(const int32_t id)
{
	_item_id = id;
}

void QuestCheck::SetPrice(const int32_t price)
{
	_price = price;
}

int32_t QuestCheck::GetItemId() const
{
	return _item_id;
}

int32_t QuestCheck::GetPrice() const
{
	return _price;
}
