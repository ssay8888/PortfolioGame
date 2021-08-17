#pragma once
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

private:

};

