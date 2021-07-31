#include "../../pch.h"
#include "skin_frame.h"
#include "skin_info.h"
#include "skin_item.h"
#include "skin_manager.h"


SkinFrame::SkinFrame() :
	_origin({ 0, 0 }),
	_width(0),
	_height(0),
	_position(0),
	_image(nullptr),
	_uolFrame(nullptr)
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

void SkinFrame::SetMap(std::map<std::string, ObjectPos> map)
{
	_map = map;
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

const std::string SkinFrame::GetPath()
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

SkinFrame* SkinFrame::FindUolFrame()
{
	if (GetUol().empty()) {
		return nullptr;
	}
	auto list = StringTools::SplitString(GetPath(), '\\');
	std::string str;
	for (auto data : list)
	{
		if (data.find(".img") != std::string::npos)
		{
			str.append(data).append("/");
			break;
		}
	}
	str.append(GetUol());
	auto data = SkinManager::GetInstance()->GetSkinInfo(str);
	auto frameData = StringTools::SplitString(GetUol(), '/');
	if (data != nullptr) 
	{
		_uolFrame = data->GetSkinItem()->GetFindFrame(frameData[frameData.size() - 1]);
		this->SetHeight(_uolFrame->GetHeight());
		this->SetWidth(_uolFrame->GetWidth());
		this->SetImage(_uolFrame->GetImage());
		this->SetOrigin(_uolFrame->GetOrigin());
		this->SetPath(_uolFrame->GetPath());
		this->SetPosition(_uolFrame->GetPosition());
		this->SetZ(_uolFrame->GetZ());
		this->SetMap(_uolFrame->GetMap());
	}
	return _uolFrame;
}

void SkinFrame::SetUolFrame(SkinFrame* uol)
{
	_uolFrame = uol;
}

SkinFrame* SkinFrame::GetUolFrame()
{
	return _uolFrame;
}

void SkinFrame::SetZ(const std::string z)
{
	SetPosition(SkinManager::GetInstance()->FindPosition(z));
	_z = z;
}

std::string SkinFrame::GetZ()
{
	return _z;
}

void SkinFrame::SetPosition(const uint16_t z)
{
	_position = z;
}

uint16_t SkinFrame::GetPosition()
{
	return _position;
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

ObjectPos SkinFrame::GetOrigin() 
{
	return _origin;
}

void SkinFrame::SetWidth(const uint32_t width)
{
	_width = width;
}

uint32_t SkinFrame::GetWidth()
{
	return _width;
}

void SkinFrame::SetHeight(const uint32_t height)
{
	_height = height;
}

uint32_t SkinFrame::GetHeight()
{
	return _height;
}

void SkinFrame::SetImage(Gdiplus::Image* image)
{
	_image = image;
}

Gdiplus::Image* SkinFrame::GetImage()
{
	return _image;
}