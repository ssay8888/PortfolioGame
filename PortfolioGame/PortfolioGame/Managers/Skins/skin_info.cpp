#include "../../pch.h"
#include "skin_info.h"
#include "skin_item.h"
#include "skin_frame.h"
#include "skin_parts.h"

SkinInfo::SkinInfo() :
	_name("")
{
}

SkinInfo::SkinInfo(std::string name) :
	_name(name)
{

}

void SkinInfo::InsertBodySkinItem(SkinItem* skinItem)
{
	_bodySkinItem.insert(std::make_pair(skinItem->GetName(), skinItem));
}

void SkinInfo::InsertHeadSkinItem(SkinItem* skinItem)
{
	_headSkinItem.insert(std::make_pair(skinItem->GetName(), skinItem));
}

SkinItem* SkinInfo::FindBodySkinItem(std::string key)
{
	auto item = _bodySkinItem.find(key);
	if (_bodySkinItem.end() != item)
	{
		return item->second;
	}
	return  nullptr;
}

SkinItem* SkinInfo::FindHeadSkinItem(std::string key)
{
	auto item = _headSkinItem.find(key);
	if (_headSkinItem.end() != item)
	{
		return item->second;
	}
	return nullptr;
}

std::map<std::string, SkinItem*>* SkinInfo::GetBodySkinItem()
{
	return &_bodySkinItem;
}

std::map<std::string, SkinItem*>* SkinInfo::GetHeadSkinItem()
{
	return &_headSkinItem;
}

void SkinInfo::SetName(const std::string name)
{
	if(_name == "")
	_name = name;
}

std::string SkinInfo::GetName() const
{
	return _name;
}


void SkinInfo::SetIcon(std::shared_ptr<MyBitmap> icon)
{
	_icon = icon;
}

void SkinInfo::SetIconRaw(std::shared_ptr<MyBitmap> icon)
{
	_icon_raw = icon;
}

std::shared_ptr<MyBitmap> SkinInfo::GetIcon() const
{
	return _icon;
}

std::shared_ptr<MyBitmap> SkinInfo::GetIconRaw() const
{
	return _icon_raw;
}