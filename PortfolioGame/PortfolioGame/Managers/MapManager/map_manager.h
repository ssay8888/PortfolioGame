#pragma once
class Item;
class GameObject;
class FootHold;
class MyBitmap;
class Player;
class Monster;
class MapInstance;
class AniMapObject;
class Portal;
class Npc;
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

	void LoadMapData(uint32_t mapid);
public:
	void ReadyMapManager();

	void ReleaseAllData();

	void UpdateGameObjectManager(const float deltaTime);
	void RenderGameObjectManager(HDC hDC);
	void RenderFootHoldManager(HDC hDC);
	void LateUpdateGameObjectManager();
	void ReleaseGameObjectManager();

	bool FootholdYCollision(GameObject* object, float* outY, FootHold** outHold);
	bool FootholdYCollisionMonster(GameObject* object, float* outY, FootHold** outHold);
	bool FootholdAndRectCollision(GameObject* object);
	bool MonsterHitBoxCollision(RECT rect);
	bool LadderRopeCollsition(GameObject* object, float* outX, FootHold** outHold);
	bool PortalCollsition(GameObject* object, Portal** outPortal);

	void TileImageLoad(std::string folderName);
	void InsertObjectImage(std::string path, AniMapObject* object);
	AniMapObject* FindAniObjectCopy(std::string key);
	
	void MapObjectImageLoad();
	void AniObjectImageLoad();
	std::list<Monster*> MonsterCollision(RECT rect, uint32_t count);
	std::list<Npc*> NpcCollision(RECT rect);
	std::list<Npc*> NpcCollision(POINT pos);
	std::shared_ptr<MapInstance> GetNowMap();
	
	void SetPlayer(Player* player);
	Player* GetPlayer();

	void ChangeMap(int32_t next_map, ObjectPos pos);

private:
	Player* _map_player;

	std::shared_ptr<MapInstance> _now_map;

	std::map<std::string, std::map<std::string, std::vector<MyBitmap*>>> _listBitmap;
	std::map<std::string, MyBitmap*> _listObjBitmap;
	std::map<std::string, AniMapObject*> _listAniObject;
	std::map<uint32_t, std::shared_ptr<MapInstance>> _listMap;


};

