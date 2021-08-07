#pragma once
class Monster;
class MonsterManager
{
private:
	MonsterManager() = default;
	~MonsterManager() = default;
public:
	static MonsterManager* GetInstance()
	{
		static MonsterManager instance;
		return &instance;
	}
	void LoadMonster();

	void InsertMonster(std::string key, std::shared_ptr<Monster*> data);
	std::shared_ptr<Monster*> FindMonster(std::string key);
private:
	std::map<std::string, std::shared_ptr<Monster*>> _list_monster;
};

