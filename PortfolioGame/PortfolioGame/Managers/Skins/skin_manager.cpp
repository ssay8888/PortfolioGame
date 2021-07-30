
#include "../../pch.h"
#include "skin_manager.h"
#include "skin_info.h"

SkinManager::SkinManager()
{
	std::cout << "생성자 호출" << std::endl;
}

void SkinManager::LoadSkin()
{
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