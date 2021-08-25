#pragma once
class Map;

class MapFactory
{
	MapFactory();
	~MapFactory();
public:
	static MapFactory* GetInstance();
	std::shared_ptr<Map> FindMap(int32_t map_id);

private:
	std::map<int32_t, std::shared_ptr<Map>> _list_map;
};