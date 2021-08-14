#include "../../pch.h"
#include "effect_manager.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../Common/Utility/file_manager.h"
#include "../../Utility/xml_reader.h"

void EffectManager::LoadDamageNumber()
{
	auto files = FileManager::GetInstance()->GetDirFileName(L"Client\\Effect\\Damage\\");

	for (auto wpath : files)
	{
		auto path = StringTools::WStringToString(wpath.c_str());
		if (!_access(path.c_str(), 0))
		{
			std::shared_ptr<MyBitmap> image = std::make_shared<MyBitmap>(MyBitmap());
			image->Insert_Bitmap(_hWnd, wpath.c_str());
			if (wpath.find(L"NoRed") != std::wstring::npos)
			{
				_attack_damage_unmber.push_back(image);
			}
			else
			{
				_take_damage_unmber.push_back(image);
			}
		}
	}
}

std::shared_ptr<MyBitmap> EffectManager::GetAttackDamageNumber(uint32_t number)
{
	if (number >= _attack_damage_unmber.size())
	{
		return _take_damage_unmber[_take_damage_unmber.size() - 1];
	}
	return _attack_damage_unmber[number];
}

std::shared_ptr<MyBitmap> EffectManager::GetTakeDamageNumber(uint32_t number)
{
	if (number >= _take_damage_unmber.size())
	{
		return _take_damage_unmber[_take_damage_unmber.size() - 1];
	}
	return _take_damage_unmber[number];
}
