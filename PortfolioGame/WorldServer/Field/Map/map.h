#pragma once
class Character;

class Map
{
public:
	Map();
	~Map();
	void SetMapId(int32_t id);
	int32_t GetMapId() const;
	void InsertCharacter(std::shared_ptr<Character> character);
	void RemoveCharacter(std::shared_ptr<Character> character);
private:
	int32_t _map_id;
	std::map<int32_t, std::shared_ptr<Character>> _characters;
};

