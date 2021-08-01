#include "../../pch.h"
#include "map_object.h"

MapObject::MapObject() :
	_image(nullptr),
	_info({}),
	_rect({}),
	_layer(5)
{
}

MapObject::~MapObject()
{
	if (_image != nullptr)
		delete _image;
}

void MapObject::SetImage(Gdiplus::Image* image)
{
	_image = image;
}

Gdiplus::Image* MapObject::GetImage() const
{
	return _image;
}

void MapObject::UpdateRect()
{
	if (_image != nullptr)
	{
		_info.cx = _image->GetWidth();
		_info.cy = _image->GetHeight();
	}
	_rect.left = LONG(_info.x - (_info.cx >> 1));
	_rect.top = static_cast<LONG>(_info.y - (_info.cy * 0.5f));
	_rect.right = LONG(_info.x + (_info.cx >> 1));
	_rect.bottom = static_cast<LONG>(_info.y + (_info.cy * 0.5f));
}

void MapObject::SetPos(float fX, float fY)
{
	_info.x = fX;
	_info.y = fY;
}

void MapObject::SetCX(int x)
{
	_info.cx = x;
}

void MapObject::SetLayer(uint32_t layer)
{
	_layer = layer;
}

void MapObject::SetCY(int y)
{
	_info.cy = y;
}

void MapObject::SetInfo(Info info)
{
	_info = info;
}

Info MapObject::GetInfo() const
{
	return _info;
}

uint32_t MapObject::GetLayer() const
{
	return _layer;
}
