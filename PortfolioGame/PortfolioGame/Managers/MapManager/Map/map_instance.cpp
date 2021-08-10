#include "../../../pch.h"
#include "map_instance.h"
#include "../../../Components/Base/game_object.h"
#include "../../../Components/MapObject/Player/player.h"
#include "../../../Components/MapObject/foot_hold.h"
#include "../../../Components/MapObject/portal.h"
#include "../../../Components/MapObject/Monster/monster.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"

MapInstance::MapInstance() :
	_map_size({})
{
	_back_ground_image = new MyBitmap();
	_back_ground_image->Insert_Bitmap(_hWnd, L"Client\\Map\\Back\\back.bmp");
}

MapInstance::~MapInstance()
{
	for (auto& data : _list_portal)
	{
		delete data;
	}
	_list_portal.clear();
}


void MapInstance::AddGameObject(GameObject* object)
{
	_list_game_object[object->GetLayer()].emplace_back(object);
}

void MapInstance::AddMonsterObject(Monster* object)
{
	_spawn_point[object->GetLayer()].emplace_back(object);
}

void MapInstance::AddFootHold(FootHold* foothold)
{
	_list_foot_hold.push_back(foothold);
}

void MapInstance::AddRopeLadder(FootHold* foothold)
{
	_list_rope_ladder.push_back(foothold);
}

void MapInstance::AddPortal(Portal* portal)
{
	_list_portal.push_back(portal);
}

std::list<GameObject*>& MapInstance::GetGameObjectList(int32_t layer)
{
	return _list_game_object[layer];
}

std::list<Monster*>* MapInstance::InMapMonsterObjectList(int32_t layer)
{
	std::list<Monster*>* list = new std::list<Monster*>();

	for (auto& data : _list_game_object[layer])
	{
		Monster* monster = dynamic_cast<Monster*>(data);
		if (monster != nullptr)
		{
			list->push_back(monster);
		}
	}
	return list;
}

std::list<Monster*>& MapInstance::GetSpawnPointMonsterList(int32_t layer)
{
	return _spawn_point[layer];
}

std::list<FootHold*>& MapInstance::GetFootHoldList()
{
	return _list_foot_hold;
}

std::list<FootHold*>& MapInstance::GetRopeLadderList()
{
	return _list_rope_ladder;
}

std::list<Portal*>& MapInstance::GetPortalList()
{
	return _list_portal;
}


void MapInstance::SetMapSize(ObjectPos size)
{
	_map_size = size;
}

ObjectPos MapInstance::GetMapSize() const
{
	return _map_size;
}

MyBitmap* MapInstance::GetBackGroundImage()
{
	return _back_ground_image;
}

Player* MapInstance::GetPlayer()
{
	return _player;
}

void MapInstance::SetPlayer(Player* player)
{
	if (_player == nullptr)
	{
		AddGameObject(player);
	}
	_player = player;
}
