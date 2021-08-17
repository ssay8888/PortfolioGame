#include "../../../pch.h"
#include "quest_info.h"

QuestInfo::QuestInfo(): _npc_id(0)
{
}

QuestInfo::~QuestInfo()
{
}

void QuestInfo::SetNpcId(int32_t id)
{
	_npc_id = id;
}

void QuestInfo::InsertSay(const std::shared_ptr<QuestSay> say)
{
	_say.emplace_back(say);
}

void QuestInfo::InsertSuccess(std::shared_ptr<QuestSay> success)
{
	_success.emplace_back(success);
}

void QuestInfo::InsertIngSay(std::shared_ptr<QuestSay> say)
{
	_ing_say.emplace_back(say);
}

std::vector<std::shared_ptr<QuestSay>>& QuestInfo::GetIngSay()
{
	return _ing_say;
}

std::vector<std::shared_ptr<QuestSay>>& QuestInfo::GetSuccess()
{
	return _success;
}

void QuestInfo::InsertReward(const std::shared_ptr<QuestReward> reward)
{
	_reward.emplace_back(reward);
}

void QuestInfo::InsertCheck(const std::shared_ptr<QuestCheck> check)
{
	_check.emplace_back(check);
}

int32_t QuestInfo::GetNpcId() const
{
	return _npc_id;
}

std::vector<std::shared_ptr<QuestSay>>& QuestInfo::GetSay()
{
	return _say;
}

std::vector<std::shared_ptr<QuestReward>>& QuestInfo::GetReward()
{
	return _reward;
}

std::vector<std::shared_ptr<QuestCheck>>& QuestInfo::GetCheck()
{
	return _check;
}
