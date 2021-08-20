#include "../../pch.h"
#include "effect_info.h"
#include "effect_parts.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../Components/MapObject/Player/player.h"
#include "../MapManager/map_manager.h"
#include "../ScrollManager/scroll_manager.h"

EffectInfo::EffectInfo(): _frame_tick(0),
                          _frame_number(0)
{
}

EffectInfo::~EffectInfo()
{
}

void EffectInfo::InsertEffectParts(std::shared_ptr<EffectParts> parts)
{
	_list_image.emplace_back(parts);
}

std::vector<std::shared_ptr<EffectParts>>& EffectInfo::GetListImage()
{
	return _list_image;
}

bool EffectInfo::UpdateFrame()
{
	return true;
}

void EffectInfo::RenderFrame(HDC hdc)
{
	const auto player = MapManager::GetInstance()->GetPlayer();
	if (_frame_number >= _list_image.size())
	{
		return;
	}
	const auto parts = _list_image[_frame_number];

	if (parts != nullptr)
	{
		parts->GetImage()->RenderBitmapImage(hdc,
			static_cast<int>(player->GetInfo().x - parts->GetOrigin().x + ScrollManager::GetScrollX()),
			static_cast<int>(player->GetInfo().y + (player->GetInfo().cy >> 1) - parts->GetOrigin().y + ScrollManager::GetScrollY()),
			parts->GetImage()->GetWidth(),
			parts->GetImage()->GetHeight());
	}
}

bool EffectInfo::LateUpdateFrame()
{
	if (_frame_number >= _list_image.size())
	{
		return false;
	}
	const auto image = _list_image[_frame_number];
	if (image != nullptr)
	{
		if (GetTickCount64() > _frame_tick + image->GetDelay())
		{
			++_frame_number;
			_frame_tick = GetTickCount64();
		}
		return true;
	}
	return false;
}

void EffectInfo::SetFrameTick(const uint64_t tick)
{
	_frame_tick = tick;
}

void EffectInfo::SetFrameNumber(const int32_t number)
{
	_frame_number = number;
}

void EffectInfo::SetName(const std::string name)
{
	_name = name;
}

uint64_t EffectInfo::GetFrameTick() const
{
	return _frame_tick;
}

int32_t EffectInfo::GetFrameNumber() const
{
	return _frame_number;
}

std::string EffectInfo::GetName() const
{
	return _name;
}
