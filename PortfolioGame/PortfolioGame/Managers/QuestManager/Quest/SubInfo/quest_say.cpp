#include "../../../../pch.h"
#include "quest_say.h"

QuestSay::QuestSay(): _id(0),
                      _type()
{
}

QuestSay::~QuestSay()
{
}

void QuestSay::SetId(int32_t id)
{
	_id = id;
}

void QuestSay::SetSay(std::string say)
{
	_say = say;
}

void QuestSay::SetType(ObjectType::NpcTalkType type)
{
	_type = type;
}

int32_t QuestSay::GetId() const
{
	return _id;
}

std::string QuestSay::GetSay() const
{
	return _say;
}

ObjectType::NpcTalkType QuestSay::GetType() const
{
	return _type;
}
