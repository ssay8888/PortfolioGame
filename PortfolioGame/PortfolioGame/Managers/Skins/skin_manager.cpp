
#include "../../pch.h"
#include "skin_manager.h"
#include "skin_info.h"
#include "skin_item.h"
#include "skin_frame.h"
#include "../../Utility/xml_reader.h"

SkinManager::SkinManager()
{
}

void SkinManager::LoadSkin()
{
	XmlReader::GetInstance().LoadCharecterSkin2(L"Client\\Character\\00002000.img.xml");
	XmlReader::GetInstance().LoadCharecterSkin2(L"Client\\Character\\00012000.img.xml");
	LoadBase();
	auto asd = _skins.begin();
	for (auto skin : _skins)
	{
		auto frames = skin.second->GetSkinItem()->GetFrames();

		for (auto begin = frames->begin(); begin != frames->end(); ++begin) {
			begin->second->FindUolFrame();
			begin->second->SetPosition(SkinManager::GetInstance()->FindPosition(begin->second->GetZ()));
		}
	}
}

void SkinManager::LoadBase()
{
	_smap = XmlReader::GetInstance().LoadSmap();
	_zmap = XmlReader::GetInstance().LoadZmap();
}

void SkinManager::AddSkin(SkinInfo* skin)
{
	if (_skins.find(skin->GetName()) == _skins.end())
	{
		_skins.insert(std::make_pair(skin->GetName(), skin));
	}
}

SkinInfo* SkinManager::GetSkinInfo(std::string key) const
{
	auto pair = _skins.find(key);
	if (pair != _skins.end())
	{
		return pair->second;
	}

	return nullptr;
}

size_t SkinManager::GetSize() 
{ 
	return _skins.size(); 
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
