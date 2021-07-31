#pragma once
class Map;
class MapManager
{
	MapManager() = default;
	~MapManager() = default;
public:
	static MapManager* Get_Instance()
	{
		static MapManager instance;
		return &instance;
	}
};

