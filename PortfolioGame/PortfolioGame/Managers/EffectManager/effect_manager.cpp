#include "../../pch.h"
#include "effect_manager.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../Common/Utility/file_manager.h"
#include "../../Utility/xml_reader.h"
#include "effect_info.h"
#include "effect_parts.h"

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

void EffectManager::LoadEffectParts()
{
	XmlReader::GetInstance().LoadEffectParts();
}

void EffectManager::UpdateEffect()
{
	if(!_show_effect_list.empty())
	{
		for (auto& effect : _show_effect_list)
		{
			effect->UpdateFrame();
		}
	}
}

void EffectManager::RenderEffect(HDC hdc)
{
	if (!_show_effect_list.empty())
	{
		for (auto& effect : _show_effect_list)
		{
			effect->RenderFrame(hdc);
		}
	}
}

void EffectManager::LateUpdateEffect()
{
	if (!_show_effect_list.empty())
	{
		for (auto begin = _show_effect_list.begin(); begin != _show_effect_list.end();)
		{
			auto remove = (*begin)->LateUpdateFrame();
			if (!remove)
			{
				begin = _show_effect_list.erase(begin);
			}
			else
			{
				++begin;
			}

		}
	}
}

void EffectManager::InsertEffect(std::shared_ptr<EffectInfo> effect)
{
	_list_effect.insert(std::make_pair(effect->GetName(), effect));
}

void EffectManager::ShowEffect(std::string key)
{
	auto data = _list_effect.find(key);
	if (data != _list_effect.end())
	{
		_show_effect_list.emplace_back(std::make_shared<EffectInfo>(EffectInfo(*data->second)));
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
