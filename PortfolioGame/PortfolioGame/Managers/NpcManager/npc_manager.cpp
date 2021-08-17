#include "../../pch.h"
#include "npc_manager.h"
#include "../../Utility/xml_reader.h"

void NpcManager::InsertNpc(std::string npc_path, std::shared_ptr<Npc> npc)
{
	_npcs.insert(std::make_pair(npc_path, npc));
}

std::shared_ptr<Npc> NpcManager::FindNpc(std::string npc_path)
{
	auto data = _npcs.find(npc_path);
	if (data != _npcs.end())
	{
		return data->second;
	}
	return nullptr;
}

void NpcManager::LoadNpc()
{
	XmlReader::GetInstance().LoadNpc();
}
