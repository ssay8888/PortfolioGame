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

	bool FootholdYCollision(GameObject* object, float* outY, FootHold** outHold);
	bool FootholdAndRectCollision(GameObject* object);
	bool LadderRopeCollsition(GameObject* object, float* outX, FootHold** outHold);

	void TileImageLoad(std::string folderName);
	void MapObjectImageLoad();


	std::list<FootHold*>* GetListRopeLadder();
	std::list<FootHold*> GetMapFootHold();
	ObjectPos GetMapSize() const;
private:
	std::list<GameObject*> _listGameObject[MaxLayer];
	std::list<FootHold*> _listFootHold;
	std::list<FootHold*> _listRopeLadder;

	std::map<std::string, std::map<std::string, std::vector<MyBitmap*>>> _listBitmap;


	std::map<std::string, MyBitmap*> _listObjBitmap;
	ObjectPos _mapSize;

};

