#include "../../pch.h"
#include "skin_parts.h"
#include "skin_frame.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"

std::string SkinParts::GetName() const
{
    return _name;
}

void SkinParts::SetName(std::string name)
{
    _name = name;
}

void SkinParts::SetMap(std::map<std::string, POINT> map)
{
    _map = map;
}

std::map<std::string, POINT> SkinParts::GetMap() const
{
    return _map;
}

void SkinParts::InsertMap(std::string key, POINT pos)
{
    _map.insert(std::make_pair(key, pos));
}

POINT SkinParts::FindMap(std::string key)
{
    auto item = _map.find(key);
    if (_map.end() != item)
    {
        return item->second;
    }
    return POINT();
}

std::map<std::string, POINT>* SkinParts::GetMaps()
{
    return &_map;
}

POINT SkinParts::GetOrigin() const
{
    return _origin;
}

void SkinParts::SetOrigin(POINT pos)
{
    _origin = pos;
}

void SkinParts::SetOriginX(LONG x)
{
    _origin.x = x;
}

void SkinParts::SetOriginY(LONG y)
{
    _origin.y = y;
}

std::string SkinParts::GetZ() const
{
    return _z;
}

void SkinParts::SetZ(std::string z)
{
    _z = z;
}

uint16_t SkinParts::GetDelay() const
{
    return _delay;
}

void SkinParts::SetDelay(uint16_t delay)
{
    _delay = delay;
}

std::string SkinParts::GetUol() const
{
    return _uol;
}

void SkinParts::SetUol(std::string uol)
{
    _uol = uol;
}

MyBitmap* SkinParts::GetBitmap() const
{
    return _image;
}

void SkinParts::SetBitmap(MyBitmap* bitmap)
{
    _image = bitmap;
}

uint16_t SkinParts::GetPosition() const
{
    return _position;
}

void SkinParts::SetPosition(uint16_t pos)
{
    _position = pos;
}

void SkinParts::SetPartner(SkinFrame* partner)
{
    _partner = partner;
}

SkinFrame* SkinParts::GetPartner() const
{
    return _partner;
}
