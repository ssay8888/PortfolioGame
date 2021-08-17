#pragma once
class QuestInfo;

class QuestManager
{
	QuestManager();
	~QuestManager();
public:
	static QuestManager* GetInstance()
	{
		static QuestManager instance;
		return &instance;
	}

	void InsertQuestInfo(std::shared_ptr<QuestInfo> quest_info);
	std::shared_ptr<QuestInfo> FindQuestInfo(int32_t npc_id);
	void LoadQuest();
private:
	std::map<int32_t, std::shared_ptr<QuestInfo>> _list_quest_info;
};

