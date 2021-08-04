#include "../../pch.h"
#include "map_object.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"

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

void MapObject::SetFileName(std::string path)
{
	_fileName = path;
}

void MapObject::SetPath(std::string path)
{
	_path = path;
}

void MapObject::SetImage(MyBitmap* image)
{
	_image = image;
}

void MapObject::SetImageNumber(uint32_t number)
{
	_imageNumber = number;
}

MyBitmap* MapObject::GetImage() const
{
	return _image;
}

uint32_t MapObject::GetImageNumber() const
{
	return _imageNumber;
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

void MapObject::SetRect(RECT rect)
{
	_rect = rect;
}

Info MapObject::GetInfo() const
{
	return _info;
}

RECT MapObject::GetRect() const
{
	return _rect;
}

uint32_t MapObject::GetLayer() const
{
	return _layer;
}

std::string MapObject::GetFileName() const
{
	return _fileName;
}

std::string MapObject::GetPath() const
{
	return _path;
}
