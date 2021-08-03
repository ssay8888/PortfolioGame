#include "../../pch.h"
#include "skin_frame.h"
#include "skin_info.h"
#include "skin_item.h"
#include "skin_manager.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"

SkinFrame::SkinFrame() :
	_actionFrame(0),
	_delay(0)
{
}

SkinFrame::~SkinFrame()
{
}

SkinParts* SkinFrame::FindParts(std::string key)
{
	auto item = _parts.find(key);
	if (item != _parts.end())
	{
		return item->second;
	}
	return nullptr;
}

void SkinFrame::InsertParts(std::string key, SkinParts* item)
{
	_parts.insert(std::make_pair(key, item));
}

std::map<std::string, SkinParts*>* SkinFrame::GetParts()
{
	return &_parts;
}

uint32_t SkinFrame::GetDelay() const
{
	return _delay;
}

void SkinFrame::SetDelay(uint32_t delay)
{
	_delay = delay;
}

std::string SkinFrame::GetFrame() const
{
	return _frame;
}

void SkinFrame::SetFrame(std::string frame)
{
	_frame = frame;
}

uint32_t SkinFrame::GetActionFrame() const
{
	return _actionFrame;
}

void SkinFrame::SetActionFrame(uint32_t nummber)
{
	_actionFrame = nummber;
}

std::string SkinFrame::GetAction() const
{
	return _action;
}

void SkinFrame::SetAction(std::string action)
{
	_action = action;
}

void SkinFrame::SetPartner(SkinItem* partner)
{
	_partner = partner;
}

SkinItem* SkinFrame::GetPartner() const
{
	return _partner;
}
