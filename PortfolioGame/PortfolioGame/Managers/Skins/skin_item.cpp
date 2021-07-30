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
	_frames.insert(std::make_pair(frame->GetName(), frame));
}

SkinFrame* SkinItem::GetFrame(std::string key) const
{
	auto item = _frames.find(key);
	if (item != _frames.end())
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
				return data->GetSkinItem()->GetFrame(frameData[frameData.size() - 1]);
			}
			return  data->GetSkinItem()->GetFrame("head");
		}
		return item->second;
	}
	return nullptr;
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
