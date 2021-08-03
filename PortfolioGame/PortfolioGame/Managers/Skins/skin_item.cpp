#include "../../pch.h"
#include "skin_item.h"
#include "skin_frame.h"
#include "skin_info.h"
#include "skin_manager.h"

SkinItem::SkinItem()
{
}

SkinItem::~SkinItem()
{
}

void SkinItem::InsertFrame(SkinFrame* frame)
{
	_skinFrames.push_back(frame);
}

SkinFrame* SkinItem::FindFrame(std::string index) const
{
	if (_skinFrames.empty())
	{
		return nullptr;
	}
	for (auto begin : _skinFrames)
	{
		if (!strcmp(begin->GetFrame().c_str(), index.c_str())) {
			return begin;
		}
	}
	return  nullptr;
}

std::string SkinItem::GetName() const
{
	return _name;
}

void SkinItem::SetName(const std::string name)
{
	_name = name;
}

size_t SkinItem::GetFrameSize() const
{
	return _skinFrames.size();
}

std::vector<SkinFrame*>* SkinItem::GetSkinFrames()
{
	return &_skinFrames;
}

void SkinItem::SetPartner(SkinInfo* partner)
{
	_partner = partner;
}

SkinInfo* SkinItem::GetPartner() const
{
	return _partner;
}
