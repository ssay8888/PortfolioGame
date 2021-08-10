#include "../../pch.h"
#include "portal.h"

#include "../../Managers/MapManager/map_manager.h"
#include "../../Components/MapObject/ani_map_object.h"

Portal::Portal(ObjectPos pos, int32_t next, Info info) :
	_next_map_spawn_pos(pos),
	_next_map(next),
	_ani_image(MapManager::GetInstance()->FindAniObjectCopy("portal")),
	_info(info),
	_rect({})
{
	_rect.left = LONG(_info.x - (_info.cx >> 1));
	_rect.top = static_cast<LONG>(_info.y - (_info.cy * 0.5f));
	_rect.right = LONG(_info.x + (_info.cx >> 1));
	_rect.bottom = static_cast<LONG>(_info.y + (_info.cy * 0.5f));
	_ani_image->SetInfo(_info);
	_ani_image->SetRect(_rect);
}

Portal::~Portal()
{
	delete _ani_image;
}

void Portal::RenderPortal(HDC hdc)
{
	_ani_image->DoRenderGameObject(hdc);
}

void Portal::SetNextMap(const int32_t next)
{
	_next_map = next;
}

int32_t Portal::GetNextMap() const
{
	return _next_map;
}

void Portal::SetNextMapSpawnPos(const ObjectPos pos)
{
	_next_map_spawn_pos = pos;
}

ObjectPos Portal::GetNextMapSpawnPos() const
{
	return _next_map_spawn_pos;
}

Info Portal::GetInfo() const
{
	return _info;
}

void Portal::SetInfo(Info info)
{
	_info = info;
}

float Portal::GetInfoX() const
{
	return _info.x;
}

float Portal::GetInfoY() const
{
	return _info.y;
}

int32_t Portal::GetInfoCX() const
{
	return _info.cx;
}

int32_t Portal::GetInfoCY() const
{
	return _info.cy;
}

void Portal::SetInfoX(float x)
{
	_info.x = x;
}

void Portal::SetInfoY(float y)
{
	_info.y = y;
}

void Portal::SetInfoCX(int cx)
{
	_info.cx = cx;
}

void Portal::SetInfoCY(int cy)
{
	_info.cy = cy;
}

RECT Portal::GetRect() const
{
	return _rect;
}
