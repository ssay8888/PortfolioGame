#pragma once
class GameObject;
class Monster;
class FootHold;
class MapInstance
{
public:
	MapInstance();
	~MapInstance();
	void AddGameObject(GameObject* object);
	void AddFootHold(FootHold* foothold);

private:
	std::list<GameObject*> _listGameObject[MaxLayer];
	std::list<Monster*> _listMonsterObject[MaxLayer];
	std::list<FootHold*> _listFootHold;
	std::list<FootHold*> _listRopeLadder;
	ObjectPos _mapSize;
};

