#include "../../pch.h"
#include "monster_manager.h"
#include "../../Utility/xml_reader.h"

void MonsterManager::LoadMonster()
{
	XmlReader::GetInstance().LoadMonsters();
}

void MonsterManager::InsertMonster(std::string key, std::shared_ptr<Monster*> data)
{
	_list_monster.insert(std::make_pair(key, data));
}

std::shared_ptr<Monster*> MonsterManager::FindMonster(std::string key)
{
	auto data = _list_monster.find(key);
	if (data != _list_monster.end())
	{
		return data->second;
	}
	return nullptr;
}
