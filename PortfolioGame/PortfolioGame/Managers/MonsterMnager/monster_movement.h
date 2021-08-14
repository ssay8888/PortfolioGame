#pragma once
class MonsterParts;
class MonsterMovement
{
public:
	MonsterMovement();
	~MonsterMovement();
	void InsertMovement(std::string key, std::vector<std::shared_ptr<MonsterParts>> data);
	std::vector<std::shared_ptr<MonsterParts>> FindMovement(std::string key);
	std::map<std::string, std::vector<std::shared_ptr<MonsterParts>>>& GetMovemnet();
private:
	std::map<std::string, std::vector<std::shared_ptr<MonsterParts>>> _movement;
};

