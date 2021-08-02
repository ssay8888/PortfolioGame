#pragma once
class GameObject;
class FootHold;
class MapManager
{
	MapManager() = default;
	~MapManager() = default;
public:
	static MapManager* GetInstance()
	{
		static MapManager instance;
		return &instance;
	}

	void AddGameObject(GameObject* object);
	void AddFootHold(FootHold* foothold);

	void LoadMapData();

	void ReleaseAllData();

	void UpdateGameObjectManager(const float deltaTime);
	void RenderGameObjectManager(HDC hDC);
	void ReleaseGameObjectManager();
private:
	std::list<GameObject*> _listGameObject[MaxLayer];
	std::list<FootHold*> _listFootHold;

};

