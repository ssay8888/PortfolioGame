#pragma once
class GameObject;
class FootHold;
class MyBitmap;
class Player;
class Monster;
class MapManager
{
	MapManager();
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
	std::list<Monster*> HitBoxMonsterCollision(RECT rect, uint32_t count);

	std::list<FootHold*>* GetListRopeLadder();
	std::list<FootHold*> GetMapFootHold();
	ObjectPos GetMapSize() const;

	void SetPlayer(Player* player);
	Player* GetPlayer();
private:
	MyBitmap* _backGroundImage;
	Player* _map_player;
	std::list<GameObject*> _listGameObject[MaxLayer];
	std::list<Monster*> _listMonsterObject[MaxLayer];
	std::list<FootHold*> _listFootHold;
	std::list<FootHold*> _listRopeLadder;

	std::map<std::string, std::map<std::string, std::vector<MyBitmap*>>> _listBitmap;


	std::map<std::string, MyBitmap*> _listObjBitmap;
	ObjectPos _mapSize;

};

