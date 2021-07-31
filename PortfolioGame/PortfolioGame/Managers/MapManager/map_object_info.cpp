#include "../../pch.h"
#include "map_object_info.h"

MapObjectInfo::MapObjectInfo() :
	_x(0),
	_y(0),
	_layer(0)
{
}

MapObjectInfo::~MapObjectInfo()
{
}

void MapObjectInfo::SetName(const std::string name)
{
	_name = name;
}

std::string MapObjectInfo::GetName() const
{
	return _name;
}

void MapObjectInfo::SetX(const uint32_t x)
{
	_x = x;
}

void MapObjectInfo::SetY(const uint32_t y)
{
	_y = y;
}

void MapObjectInfo::SetLayer(const uint32_t layer)
{
	_layer = layer;
}

uint32_t MapObjectInfo::GetX() const
{
	return _x;
}

uint32_t MapObjectInfo::GetY() const
{
	return _y;
}

uint8_t MapObjectInfo::GetLayer() const
{
	return _layer;
}
