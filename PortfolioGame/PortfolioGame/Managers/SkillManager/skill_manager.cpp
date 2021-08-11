#include "../../pch.h"
#include "skill_manager.h"
#include "Skill/skill.h"
#include "../../Utility/xml_reader.h"

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
