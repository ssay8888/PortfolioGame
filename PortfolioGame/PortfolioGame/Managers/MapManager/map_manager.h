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
	void ReadyMapManager();

	void AddGameObject(GameObject* object);
	void AddFootHold(FootHold* foothold);

	void LoadMapData();

	void ReleaseAllData();

	void UpdateGameObjectManager(const float deltaTime);
	void RenderGameObjectManager(HDC hDC);
	void ReleaseGameObjectManager();

	bool FootholdCollision(Info& info, float * outY);
private:
	std::list<GameObject*> _listGameObject[MaxLayer];
	std::list<FootHold*> _listFootHold;
	std::map<std::string, std::map<std::string, MyBitmap*>> _listBitmap;

};

