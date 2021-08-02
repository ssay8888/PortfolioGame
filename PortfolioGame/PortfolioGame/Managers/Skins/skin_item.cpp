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

void SkinItem::AddFrame(SkinFrame* frame)
{
	_skinFrames.insert(std::make_pair(frame->GetName(), frame));
}

std::map<std::string, SkinFrame*>* SkinItem::GetFrames()
{
	return &_skinFrames;
}

SkinFrame* SkinItem::GetFindFrame(std::string key) const
{
	auto item = _skinFrames.find(key);
	if (item != _skinFrames.end())
	{
		if (!item->second->GetUol().empty())
		{
			auto list = StringTools::SplitString(item->second->GetPath(), '\\');
			std::string str;
			for (auto data : list)
			{
				if (data.find(".img") != std::string::npos)
				{
					str.append(data).append("/");
					break;
				}
			}
			str.append(item->second->GetUol());
			auto data = SkinManager::GetInstance()->GetSkinInfo(str);
			auto frameData = StringTools::SplitString(item->second->GetUol(), '/');
			if (frameData.size() > 1)
			{
				return data->GetSkinItem()->GetFindFrame(frameData[frameData.size() - 1]);
			}
			return  data->GetSkinItem()->GetFindFrame("head");
		}
		return item->second;
	}
	return nullptr;
}

size_t SkinItem::GetFrameMaxSize()
{
	return _skinFrames.size();
}

std::string SkinItem::GetName() const
{
	return _name;
}

void SkinItem::SetName(const std::string name)
{
	if (_name == "")
		_name = name;
}
