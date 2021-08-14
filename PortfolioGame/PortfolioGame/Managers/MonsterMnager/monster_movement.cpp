#include "../../pch.h"
#include "monster_movement.h"
#include "monster_parts.h"

MonsterMovement::MonsterMovement()
{
}

MonsterMovement::~MonsterMovement()
{
}

void MonsterMovement::InsertMovement(std::string key, std::vector<std::shared_ptr<MonsterParts>> data)
{
	_movement.insert(std::make_pair(key, data));
}

std::vector<std::shared_ptr<MonsterParts>> MonsterMovement::FindMovement(std::string key)
{
	auto data = _movement.find(key);
	if (data != _movement.end())
	{
		return data->second;
	}
	return std::vector<std::shared_ptr<MonsterParts>>();
}

std::map<std::string, std::vector<std::shared_ptr<MonsterParts>>>& MonsterMovement::GetMovemnet()
{
	return _movement;
}
