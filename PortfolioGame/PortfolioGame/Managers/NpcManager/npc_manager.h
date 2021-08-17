#pragma once
class Npc;

class NpcManager
{
	NpcManager() = default;
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

private:
	std::map<std::string, std::shared_ptr<Npc>> _npcs;
};

