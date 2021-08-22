#pragma once
class MyBitmap;
class Npc;

class NpcManager
{
	NpcManager();
	~NpcManager() = default;
public:
	static NpcManager* GetInstance()
	{
		static NpcManager instance;
		return &instance;
	}

	void InsertNpc(std::string npc_path, std::shared_ptr<Npc> npc);
	std::shared_ptr<Npc> FindNpc(std::string npc_path);
	void LoadNpc();
	std::vector<std::shared_ptr<MyBitmap>>& GetAcceptable();
	std::vector<std::shared_ptr<MyBitmap>>& GetProceeding();
	std::vector<std::shared_ptr<MyBitmap>>& GetCompleted();

	void SetQuestIconTick();
	uint64_t GetQuestIconTick() const;

	void GainQuestFrameNumber(uint32_t value);
	uint32_t GetQuestFrameNumber() const;
private:
	void QuestAlarmImage();
private:
	std::map<std::string, std::shared_ptr<Npc>> _npcs;
	std::vector<std::shared_ptr<MyBitmap>> _acceptable;
	std::vector<std::shared_ptr<MyBitmap>> _proceeding;
	std::vector<std::shared_ptr<MyBitmap>> _completed;

	uint64_t _quest_icon_tick;
	uint32_t _quest_frame_number;
};

