#include "../../pch.h"
#include "map_factory.h"
#include "map.h"
MapFactory::MapFactory() {}

MapFactory::~MapFactory() {}

MapFactory* MapFactory::GetInstance()
{
	static MapFactory instance;
	return &instance;
}

std::shared_ptr<Map> MapFactory::FindMap(int32_t map_id)
{
	auto map = _list_map.find(map_id);
  if (map != _list_map.end())
  {
	  return map->second;
  }

  auto new_map = std::make_shared<Map>();
  new_map->SetMapId(map_id);
  _list_map.insert(std::make_pair(new_map->GetMapId(), new_map));
  return new_map;
}
