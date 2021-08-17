#pragma once
class Npc;
class Item;
class GameObject;
class Monster;
class FootHold;
class MyBitmap;
class Portal;
class Player;
class MapInstance
{
public:
	MapInstance();
	~MapInstance();

	void AddGameObject(GameObject* object);
	void AddMonsterObject(Monster* object);
	void AddFootHold(FootHold* foothold);
	void AddRopeLadder(FootHold* foothold);
	void AddPortal(Portal* portal);
	void AddDropItem(std::pair<POINT, std::shared_ptr<Item>>);

	std::list<GameObject*>& GetGameObjectList(int32_t layer);
	std::list<Monster*>* InMapMonsterObjectList(int32_t layer);
	std::list<Npc*>* InMapNpcObjectList();
	std::list<Monster*>& GetSpawnPointMonsterList(int32_t layer);
	std::list<FootHold*>& GetFootHoldList();
	std::list<FootHold*>& GetRopeLadderList();
	std::list<Portal*>& GetPortalList();
	std::list<std::pair<POINT, std::shared_ptr<Item>>>& GetListDropItem();

	void RenderDropItems(HDC hdc);

	void SetMapSize(ObjectPos size);
	ObjectPos GetMapSize() const;
	MyBitmap* GetBackGroundImage();
	Player* GetPlayer();
	void SetPlayer(Player* player);
private:
	MyBitmap* _back_ground_image;
	std::list<GameObject*> _list_game_object[MaxLayer];
	std::list<Monster*> _spawn_point[MaxLayer];
	std::list<FootHold*> _list_foot_hold;
	std::list<FootHold*> _list_rope_ladder;
	std::list<std::pair<POINT, std::shared_ptr<Item>>> _list_drop_item;
	ObjectPos _map_size;
	std::list<Portal*> _list_portal;
	Player* _player;
};

