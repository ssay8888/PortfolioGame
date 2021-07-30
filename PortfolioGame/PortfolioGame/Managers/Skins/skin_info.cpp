#include "../../pch.h"
#include "skin_info.h"
#include "skin_item.h"

SkinInfo::SkinInfo(std::string name) :
	_name(name),
	_delay(0),
	_skin_item(nullptr)
{

}

void SkinInfo::SetSkinItem(SkinItem* item)
{
	_skin_item = item;
}

SkinItem* SkinInfo::GetSkinItem()
{
	return _skin_item;
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

void SkinInfo::SetDelay(uint32_t delay)
{
	_delay = delay;
}

uint32_t SkinInfo::GetDelay() const
{
	return _delay;
}
