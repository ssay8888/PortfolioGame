#include "../../pch.h"
#include "skill_manager.h"
#include "Skill/skill.h"
#include "../../Utility/xml_reader.h"
#include "BuffUi/buff_ui.h"
#include "../../Components/MapObject/Player/player.h"
#include "../../Components/MapObject/Player/Buff/buffstat.h"
#include "../MapManager/map_manager.h"
#include "../../Utility/game_constants.h"

SkillManager::SkillManager() = default;

SkillManager::~SkillManager()
{
	for (auto& data : _skills)
	{
		delete data.second;
	}
	_skills.clear();
}

SkillManager* SkillManager::GetInstance()
{
	static SkillManager instance;  // NOLINT(clang-diagnostic-exit-time-destructors)
	return &instance;
}

Skill* SkillManager::FindSkill(uint32_t skill_id)
{
	const auto data = _skills.find(skill_id);
	if (data != _skills.end())
	{
		return data->second;
	}

	return nullptr;
}

std::list<Skill*> SkillManager::FindSkillList(const uint32_t job_id)
{
	std::list<Skill*> list;

	for (auto& data : _skills)
	{
		if (data.first / 10000 == job_id)
		{
			list.emplace_back(data.second);
		}
	}
	return list;
}

void SkillManager::LoadSkillData()
{
	XmlReader::GetInstance().SkillLoad();
}

void SkillManager::InsertSkill(int32_t skill_id, Skill* skill)
{
	_skills.insert(std::make_pair(skill_id, skill));
}

void SkillManager::InsertBuffSkill(int32_t skill_id)
{
	const auto skill = FindSkill(skill_id);

	if (skill  != nullptr)
	{
		std::shared_ptr<BuffUi> buff = std::make_shared<BuffUi>(BuffUi(skill));

		for (auto begin = _buff_list.begin(); begin != _buff_list.end();)
		{
			if ((*begin)->GetSkill()->GetSkillId() == skill_id)
			{
				begin = _buff_list.erase(begin);
			}
			else
			{
				++begin;
			}
		}

		_buff_list.emplace_back(buff);
	}
}

std::shared_ptr<BuffUi> SkillManager::FindBuffSkill(int32_t skill_id)
{
	for (auto& data : _buff_list)
	{
		if (data->GetSkill()->GetSkillId() == skill_id)
		{
			return data;
		}
	}
	return nullptr;
}

void SkillManager::UpdateBuff()
{
	for (auto begin = _buff_list.begin(); begin != _buff_list.end(); )
	{
		if ((*begin)->UpdateBuffUi())
		{
			const auto player = MapManager::GetInstance()->GetPlayer();
			player->GetBuffStat()->RemoveFlag(GameConstants::SkillToBuffStat((*begin)->GetSkill()->GetSkillId()));
			player->RecalcEqpStat();
			begin = _buff_list.erase(begin);
		}
		else
		{
			++begin;
		}
	}
}

void SkillManager::RenderBuff(const HDC hdc)
{
	int32_t x = static_cast<int32_t>(800 - (32 * _buff_list.size()));
	for (const auto& buff : _buff_list)
	{
		buff->RenderBuffUi(hdc, { x, 0});
		x += 32;
	}
}
