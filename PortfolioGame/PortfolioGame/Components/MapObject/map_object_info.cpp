#include "../../pch.h"
#include "map_object_info.h"

MapObjectInfo::MapObjectInfo(uint8_t layer) :
	GameObject(layer)
{
}

MapObjectInfo::~MapObjectInfo()
{
}

int MapObjectInfo::ReadyGameObject()
{
	return 0;
}

void MapObjectInfo::UpdateGameObject(const float deltaTime)
{
}

void MapObjectInfo::RenderGameObject(HDC hdc)
{
}

void MapObjectInfo::LateUpdateGameObject()
{
}
