#include "../../pch.h"
#include "monster_manager.h"

#include "monster_movement.h"
#include "../../Managers/MonsterMnager/monster_parts.h"
#include "../../Components/MapObject/Monster/monster.h"
#include "../../Components/MapObject/Monster/AttackInfo/attack_info.h"
#include "../../Utility/xml_reader.h"

void MonsterManager::LoadMonster()
{
	XmlReader::GetInstance().LoadMonsters();
	UolFindSetting();
}

void MonsterManager::InsertMonster(std::string key, std::shared_ptr<Monster*> data)
{
	_list_monster.insert(std::make_pair(key, data));
}

std::shared_ptr<Monster*> MonsterManager::FindMonster(std::string key)
{
	auto data = _list_monster.find(key);
	if (data != _list_monster.end())
	{
		return data->second;
	}
	return nullptr;
}

void MonsterManager::UolFindSetting()
{
	for (auto& pair_data : _list_monster)
	{
		const auto& monster = (*pair_data.second);

		const auto movemnet = (*monster->GetMovement());
		for(const auto& move_frame : movemnet->GetMovemnet())
		{
			for (const auto& parts : move_frame.second)
			{
				const auto part = (*parts);

				if (!part->GetUol().empty())
				{
					MonsterPartsUolSetting(part, move_frame.second);
				}
			}
		}
		for (auto info : monster->GetAttackInfo())
		{
			auto effect_vector = info.second->GetEffect();
			for (const auto& effect_frame_shared : effect_vector)
			{
				const auto& effect_frame = (*effect_frame_shared);

				if (!effect_frame->GetUol().empty())
				{
					uint16_t overlapCount = StringTools::FindOverlapCount(effect_frame->GetUol(), "../");
					auto splitString = StringTools::SplitString(effect_frame->GetUol(), '/');

					if (overlapCount == 0)
					{
						auto copy_target = (*effect_vector[std::stoi(splitString[0])]);

						effect_frame->SetDelay(copy_target->GetDelay());
						effect_frame->SetHeadPos(copy_target->GetHeadPos());
						effect_frame->SetImage(copy_target->GetImage());
						effect_frame->SetOriginPos(copy_target->GetOriginPos());
						effect_frame->SetRect(copy_target->GetRect());
					}
				}
			}

			auto area_warning_vector = info.second->GetAreaWarning();
			for (const auto& effect_frame_shared : area_warning_vector)
			{
				const auto& effect_frame = (*effect_frame_shared);

				if (!effect_frame->GetUol().empty())
				{
					uint16_t overlapCount = StringTools::FindOverlapCount(effect_frame->GetUol(), "../");
					auto splitString = StringTools::SplitString(effect_frame->GetUol(), '/');

					if (overlapCount == 0)
					{
						auto copy_target = (*area_warning_vector[std::stoi(splitString[0])]);

						effect_frame->SetDelay(copy_target->GetDelay());
						effect_frame->SetHeadPos(copy_target->GetHeadPos());
						effect_frame->SetImage(copy_target->GetImage());
						effect_frame->SetOriginPos(copy_target->GetOriginPos());
						effect_frame->SetRect(copy_target->GetRect());
					}
				}
			}

			auto hit_effect = info.second->GetHitEffect();
			for (const auto& effect_frame_shared : hit_effect)
			{
				const auto& effect_frame = (*effect_frame_shared);

				if (!effect_frame->GetUol().empty())
				{
					uint16_t overlapCount = StringTools::FindOverlapCount(effect_frame->GetUol(), "../");
					auto splitString = StringTools::SplitString(effect_frame->GetUol(), '/');

					if (overlapCount == 0)
					{
						auto copy_target = (*hit_effect[std::stoi(splitString[0])]);

						effect_frame->SetDelay(copy_target->GetDelay());
						effect_frame->SetHeadPos(copy_target->GetHeadPos());
						effect_frame->SetImage(copy_target->GetImage());
						effect_frame->SetOriginPos(copy_target->GetOriginPos());
						effect_frame->SetRect(copy_target->GetRect());
					}
				}
			}
		}
	}
}

void MonsterManager::MonsterPartsUolSetting(MonsterParts* part, const std::vector<std::shared_ptr<MonsterParts*>> move_frame) const
{
	uint16_t overlapCount = StringTools::FindOverlapCount(part->GetUol(), "../");
	auto splitString = StringTools::SplitString(part->GetUol(), '/');
	if (overlapCount == 0)
	{
		auto frame = (*move_frame[std::stoi(splitString[0])]);
		part->SetDelay(frame->GetDelay());
		part->SetHeadPos(frame->GetHeadPos());
		part->SetImage(frame->GetImage());
		part->SetOriginPos(frame->GetOriginPos());
		part->SetRect(frame->GetRect());
	}
	else if (overlapCount == 1)
	{
		auto move_frame = (*part->GetPartner())->FindMovement(splitString[1]);
		auto frame = (*move_frame[std::stoi(splitString[2])]);
		part->SetDelay(frame->GetDelay());
		part->SetHeadPos(frame->GetHeadPos());
		part->SetImage(frame->GetImage());
		part->SetOriginPos(frame->GetOriginPos());
		part->SetRect(frame->GetRect());
	}
	else
	{
		std::cout << overlapCount << "¿À¹ö·¾ 2°³  " << std::endl;
	}
}
