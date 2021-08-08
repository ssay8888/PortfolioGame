#include "../../pch.h"
#include "skin_parts.h"
#include "skin_frame.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"

SkinParts::SkinParts() :
    _delay(0),
    _image(nullptr),
    _origin({}),
    _partner(nullptr),
    _position(0)
{
}

std::string SkinParts::GetName() const
{
    return _name;
}

void SkinParts::SetName(std::string name)
{
    _name = name;
}

void SkinParts::SetMap(std::unordered_map<std::string, ObjectPos> map)
{
    _map = map;
}

std::unordered_map<std::string, ObjectPos> SkinParts::GetMap() const
{
    return _map;
}

void SkinParts::InsertMap(std::string key, ObjectPos pos)
{
    _map.insert(std::make_pair(key, pos));
}

ObjectPos SkinParts::FindMap(std::string key)
{
    auto item = _map.find(key);
    if (_map.end() != item)
    {
        return item->second;
    }
    return ObjectPos();
}

std::unordered_map<std::string, ObjectPos>* SkinParts::GetMaps()
{
    return &_map;
}

ObjectPos SkinParts::GetOrigin() const
{
    return _origin;
}

void SkinParts::SetOrigin(ObjectPos pos)
{
    _origin = pos;
}

void SkinParts::SetOriginX(float x)
{
    _origin.x = x;
}

void SkinParts::SetOriginY(float y)
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

void SkinParts::SetPartner(SkinFrame* partner)
{
    _partner = partner;
}

SkinFrame* SkinParts::GetPartner() const
{
    return _partner;
}

void SkinParts::SetPosition(uint16_t position)
{
    _position = position;
}

uint16_t SkinParts::GetPosition() const
{
    return _position;
}
