#pragma once
class GameObject;
class FootHold;
class MyBitmap;
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

	void LoadMapData();
public:

	void ReadyMapManager();

	void AddGameObject(GameObject* object);
	void AddFootHold(FootHold* foothold);

	void ReleaseAllData();

	void UpdateGameObjectManager(const float deltaTime);
	void RenderGameObjectManager(HDC hDC);
	void RenderFootHoldManager(HDC hDC);
	void LateUpdateGameObjectManager();
	void ReleaseGameObjectManager();

	bool FootholdYCollision(GameObject* object, float* outY);
	bool FootholdAndRectCollision(GameObject* object);

	POINT GetMapSize() const;
private:
	std::list<GameObject*> _listGameObject[MaxLayer];
	std::list<FootHold*> _listFootHold;
	std::map<std::string, std::map<std::string, std::vector<MyBitmap*>>> _listBitmap;
	POINT _mapSize;

};

