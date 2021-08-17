#include "../../../../pch.h"
#include "quest_reward.h"

QuestReward::QuestReward(): _item_id(0),
                            _value(0)
{
}

QuestReward::~QuestReward()
{
}

void QuestReward::SetRewardType(const std::string type)
{
	_reward_type = type;
}

std::string QuestReward::GetRewardType() const
{
	return _reward_type;
}

void QuestReward::SetItemId(const int32_t id)
{
	_item_id = id;
}

int32_t QuestReward::GetItemId() const
{
	return _item_id;
}

void QuestReward::SetValue(const int32_t value)
{
	_value = value;
}

int32_t QuestReward::GetValue() const
{
	return _value;
}
