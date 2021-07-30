#include "../../pch.h"
#include "skin_frame.h"
#include "skin_manager.h"


SkinFrame::SkinFrame() :
	_origin({ 0, 0 }),
	_width(0),
	_height(0),
	_image(nullptr)
{
}

SkinFrame::~SkinFrame()
{
	if (_image != nullptr)
		delete _image;
}

void SkinFrame::AddMap(const std::string name, const ObjectPos pos)
{
	_map.insert(std::make_pair(name, pos));
}

void SkinFrame::AddMap(std::pair<std::string, ObjectPos> map)
{
	_map.insert(map);
}

size_t SkinFrame::GetMapSize() const
{
	return _map.size();
}

std::map<std::string, ObjectPos>& SkinFrame::GetMap()
{
	return _map;
}

const ObjectPos& SkinFrame::GetMapItem(const std::string name)
{
	auto item = _map.find(name);
	return item->second;
}

void SkinFrame::SetName(const std::string name)
{
	if (!strcmp(_name.c_str(), ""))
		_name = name;
}

const std::string SkinFrame::GetName() const
{
	return _name;
}

void SkinFrame::SetPath(const std::string path)
{
	_path = path;
}

const std::string SkinFrame::GetPath() const
{
	return _path;
}

void SkinFrame::SetUol(const std::string uol)
{
	_uol = uol;
}

std::string SkinFrame::GetUol() const
{
	return _uol;
}

void SkinFrame::SetOrigin(const ObjectPos pos)
{
	_origin = pos;
}

void SkinFrame::SetOriginX(float x)
{
	_origin.x = x;
}

void SkinFrame::SetOriginY(float y)
{
	_origin.y = y;
}

ObjectPos SkinFrame::GetOrigin() const
{
	return _origin;
}

void SkinFrame::SetWidth(const uint32_t width)
{
	_width = width;
}

uint32_t SkinFrame::GetWidth() const
{
	return _width;
}

void SkinFrame::SetHeight(const uint32_t height)
{
	_height = height;
}

uint32_t SkinFrame::GetHeight() const
{
	return _height;
}

void SkinFrame::SetImage(Gdiplus::Image* image)
{
	_image = image;
}

Gdiplus::Image* SkinFrame::GetImage() const
{
	SkinManager::GetInstance()->GetSkinInfo("");
	return _image;
}