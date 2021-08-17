#include "../../../pch.h"
#include "map_instance.h"
#include "../../../Components/Base/game_object.h"
#include "../../../Components/MapObject/Player/player.h"
#include "../../../Components/MapObject/foot_hold.h"
#include "../../../Components/MapObject/portal.h"
#include "../../../Components/MapObject/Monster/monster.h"
#include "../../../Components/MapObject/Item/item.h"
#include "../../../Components/MapObject/Npc/npc.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../ScrollManager/scroll_manager.h"

MapInstance::MapInstance() :
	_map_size({}),
	_player(nullptr)
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

void MapInstance::AddDropItem(std::pair<POINT, std::shared_ptr<Item>> item)
{
	_list_drop_item.emplace_back(item);
}

std::list<std::pair<POINT, std::shared_ptr<Item>>>& MapInstance::GetListDropItem()
{
	return _list_drop_item;
}

void MapInstance::RenderDropItems(HDC hdc)
{
	for (auto& data : this->GetListDropItem())
	{
		auto item = data.second;
		if (item->IsNextFrameDelay())
		{
			item->AddFrameNumber();
			item->SetDelayTick();
		}
		auto icons = item->GetIconRaws();
		auto icon = icons[item->GetFrameNumber() % item->GetIconRaws().size()];

		icon->RenderBitmapImage(hdc,
			data.first.x + static_cast<int>(ScrollManager::GetScrollX()),
			data.first.y - icon->GetHeight() + static_cast<int>(ScrollManager::GetScrollY()),
			icon->GetWidth(),
			icon->GetHeight());
	}
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

std::list<Npc*>* MapInstance::InMapNpcObjectList()
{
	std::list<Npc*>* list = new std::list<Npc*>();
	for (int i = 0; i < MaxLayer; ++i)
	{
		for (auto& data : _list_game_object[i])
		{
			Npc* npc = dynamic_cast<Npc*>(data);
			if (npc != nullptr)
			{
				list->push_back(npc);
			}
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
