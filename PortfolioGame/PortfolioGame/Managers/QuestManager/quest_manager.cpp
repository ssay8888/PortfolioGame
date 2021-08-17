#include "../../pch.h"
#include "quest_manager.h"
#include "../../Utility/xml_reader.h"
#include "Quest/quest_info.h"

QuestManager::QuestManager()
{
}

QuestManager::~QuestManager()
{
}

void QuestManager::InsertQuestInfo(std::shared_ptr<QuestInfo> quest_info)
{
	_list_quest_info.insert(std::make_pair(quest_info->GetNpcId(), quest_info));
}

std::shared_ptr<QuestInfo> QuestManager::FindQuestInfo(int32_t npc_id)
{
	auto data = _list_quest_info.find(npc_id);
	if (data != _list_quest_info.end())
	{
		return data->second;
	}
	return nullptr;
}

void QuestManager::LoadQuest()
{
	XmlReader::GetInstance().LoadQuest();
}
