#pragma once
class GameObject;
class DropDataInfo;

class DropDataManager
{
	DropDataManager();
	~DropDataManager();
public:
	static DropDataManager* GetInstance()
	{
		static DropDataManager instance;
		return &instance;
	}

	void InsertDropData(int32_t mob_id, std::list<std::shared_ptr<DropDataInfo>> info);
	std::list<std::shared_ptr<DropDataInfo>> FindDropInfo(int32_t mob_id);
	void DropFromMonster(int32_t monster_id, GameObject* object);

	static void LoadDropData();
private:
	std::map<int32_t, std::list<std::shared_ptr<DropDataInfo>>> _drop_info_list;
};

