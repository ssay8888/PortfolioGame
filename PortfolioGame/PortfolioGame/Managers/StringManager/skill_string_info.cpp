#include "../../pch.h"
#include "skill_string_info.h"

SkillStringInfo::SkillStringInfo(): _skill_id(0)
{
}

SkillStringInfo::~SkillStringInfo()
{
}

void SkillStringInfo::SetSkillId(int32_t skill_id)
{
	_skill_id = skill_id;
}

int32_t SkillStringInfo::GetSkillId() const
{
	return _skill_id;
}

void SkillStringInfo::SetName(std::wstring name)
{
	_name = name;
}

std::wstring SkillStringInfo::GetName() const
{
	return _name;
}

void SkillStringInfo::SetDesc(std::wstring desc)
{
	_desc = desc;
}

std::wstring SkillStringInfo::GetDesc() const
{
	return _desc;
}

void SkillStringInfo::InsertInfoString(int32_t key, std::wstring value)
{
	_info.insert(std::make_pair(key, value));
}

std::wstring SkillStringInfo::FindInfoString(int32_t key) const
{
	auto data = _info.find(key);
	if (data != _info.end())
	{
		return data->second;
	}
	return L"";
}
