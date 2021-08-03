
#include "../../pch.h"
#include "skin_manager.h"
#include "skin_info.h"
#include "skin_item.h"
#include "skin_frame.h"
#include "skin_parts.h"
#include "../../Utility/xml_reader.h"

SkinManager::SkinManager()
{
}

void SkinManager::LoadSkin()
{
	XmlReader::GetInstance().LoadCharecterSkin(2000);
	XmlReader::GetInstance().LoadCharecterSkin(12000);
	LoadBase();

	for (auto skin : _bodySkins)
	{
		std::map<std::string, SkinItem*>* frames = nullptr;
		if (std::stoi(skin.first) > 10000 )
		{
			frames = skin.second->GetHeadSkinItem();
		}
		else
		{
			frames = skin.second->GetBodySkinItem();
		}

		for (auto begin = frames->begin(); begin != frames->end(); ++begin) {
			for (auto frames = begin->second->GetSkinFrames()->begin(); frames != begin->second->GetSkinFrames()->end(); ++frames)
			{
				for (auto parts = (*frames)->GetParts()->begin(); parts != (*frames)->GetParts()->end(); ++parts)
				{
					parts->second->SetPosition(SkinManager::GetInstance()->FindPosition(parts->second->GetZ()));
					SkinManager::GetInstance()->UolSetting(std::stoi(skin.first), _bodySkins, begin->second);
				}
			}
			//begin->second->GetSkinFrames()->front()->GetParts()->begin()->second->SetPosition();
			//begin->second->FindUolFrame();
			//begin->second->SetPosition(SkinManager::GetInstance()->FindPosition(begin->second->GetZ()));
		}
	}
}

void SkinManager::LoadBase()
{
	_smap = XmlReader::GetInstance().LoadSmap();
	_zmap = XmlReader::GetInstance().LoadZmap();
	//for (auto skinInfos : _bodySkins)
	//{
	//	for (auto skinItem = skinInfos.second->GetBodySkinItem()->begin(); skinItem != skinInfos.second->GetBodySkinItem()->end(); ++skinItem)
	//	{
	//		UolSetting(_bodySkins, skinItem->second);
	//	}
	//}
}

void SkinManager::InsertBodySkin(SkinInfo* skin)
{
	if (_bodySkins.find(skin->GetName()) == _bodySkins.end())
	{
		_bodySkins.insert(std::make_pair(skin->GetName(), skin));
	}
}

void SkinManager::InsertHeadSkin(SkinInfo* skin)
{
	if (_headSkins.find(skin->GetName()) == _headSkins.end())
	{
		_headSkins.insert(std::make_pair(skin->GetName(), skin));
	}
}

SkinInfo* SkinManager::GetBodySkinInfo(std::string key) const
{
	auto pair = _bodySkins.find(key);
	if (pair != _bodySkins.end())
	{
		return pair->second;
	}

	return nullptr;
}

SkinInfo* SkinManager::GetHeadSkinInfo(std::string key) const
{
	auto pair = _bodySkins.find(key);
	if (pair != _bodySkins.end())
	{
		return pair->second;
	}
	return nullptr;
}

size_t SkinManager::GetSize()
{
	return _bodySkins.size();
}

uint32_t SkinManager::FindPosition(std::string z)
{
	auto z_Index = z;
	int zPosition = 0;

	try
	{
		zPosition = std::stoi(z_Index);

	}
	catch (std::exception&)
	{
		int i = 0;
		std::for_each(_zmap.begin(), _zmap.end(),
			[&i, &z, &zPosition](std::string& data) {
				i++;
				if (data == z)
				{
					zPosition = i;
					return;
				}
			});
		return zPosition;
	}
	return zPosition;
}

void SkinManager::UolSetting(uint16_t skinId, std::map<std::string, SkinInfo*> info, SkinItem* skinitem)
{
	for (auto frames = skinitem->GetSkinFrames()->begin(); frames != skinitem->GetSkinFrames()->end(); ++frames)
	{
		for (auto parts = (*frames)->GetParts()->begin(); parts != (*frames)->GetParts()->end(); ++parts)
		{
			if (!parts->second->GetUol().empty())
			{
				std::string uol = parts->second->GetUol();
				uint16_t overlapCount = StringTools::FindOverlapCount(uol, "../");
				StringTools::ReplaceAll(uol, "../");
				auto splitString = StringTools::SplitString(uol, '/');
				SkinParts * part = nullptr;
				if (!strcmp(skinitem->GetName().c_str(), "proneStab"))
				{
					int asd = 123;
				}
				if (overlapCount == 1)
				{
					if (splitString.size() == 2)
					{
						auto frame = parts->second->GetPartner()->GetPartner()->FindFrame(splitString[0]);
						part = frame->FindParts(splitString[1]);
					}
					else if (splitString.size() == 3)
					{
						auto skinInfo = parts->second->GetPartner()->GetPartner()->GetPartner()->FindBodySkinItem(splitString[0]);
						auto frame = skinInfo->FindFrame(splitString[1]);
						part = frame->FindParts(splitString[2]);
					}
				}
				else if (overlapCount == 2)
				{
					if (splitString.size() == 2)
					{
						SkinItem* skinItem = nullptr;
						if (skinId > 10000) 
						{
							skinItem = parts->second->GetPartner()->GetPartner()->GetPartner()->FindHeadSkinItem(splitString[0]);
						}
						else
						{
							skinItem = parts->second->GetPartner()->GetPartner()->GetPartner()->FindBodySkinItem(splitString[0]);
						}
						auto skinFrame = skinItem->FindFrame(splitString[1]);
						if (skinFrame != nullptr && !strcmp(splitString[1].c_str(), "head"))
						{
							part = skinFrame->GetParts()->begin()->second;
						}
					}
					else if (splitString.size() == 3)
					{

						SkinItem* skinItem = nullptr;
						if (skinId > 10000)
						{
							skinItem = parts->second->GetPartner()->GetPartner()->GetPartner()->FindHeadSkinItem(splitString[0]);
							auto frame = skinItem->FindFrame(splitString[1]);
							if (frame->GetParts()->size() == 1)
							{
								part = frame->GetParts()->begin()->second;
							}
							else
							{
								part = frame->FindParts(splitString[2]);
							}
						}
						else
						{
							skinItem = parts->second->GetPartner()->GetPartner()->GetPartner()->FindBodySkinItem(splitString[0]);
							auto frame = skinItem->FindFrame(splitString[1]);
							part = frame->FindParts(splitString[2]);
						}
					}
				}
				//auto frame = skinItem->FindFrame(std::stoi(splitString[1]));
				//auto part = frame->FindParts(splitString[2]);
				std::cout << skinId << " : "  << parts->second->GetUol() << std::endl;
				parts->second->SetMap(part->GetMap());
				parts->second->SetBitmap(part->GetBitmap());
				parts->second->SetDelay(part->GetDelay());
				parts->second->SetName(part->GetName());
				parts->second->SetOrigin(part->GetOrigin());
				parts->second->SetPosition(part->GetPosition());
				parts->second->SetUol(part->GetUol());
				parts->second->SetZ(part->GetZ());
			}
		}
	}
}