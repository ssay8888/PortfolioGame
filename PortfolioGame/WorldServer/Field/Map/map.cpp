#include "../../pch.h"
#include "map.h"
#include "../Character/character.h"
Map::Map(): _map_id(0) {}
Map::~Map() {}
void Map::SetMapId(int32_t id)
{
	_map_id = id;
}
int32_t Map::GetMapId() const
{
	return _map_id;
}
void Map::InsertCharacter(std::shared_ptr<Character> character)
{
	_characters.insert(std::make_pair(character->GetId(), character));
}
void Map::RemoveCharacter(std::shared_ptr<Character> character)
{
	auto data = _characters.find(character->GetId());
  if (data != _characters.end())
  {
	  _characters.erase(data);
  }
}
