#include "../../pch.h"
#include "string_manager.h"
#include "../../Utility/xml_reader.h"
#include "string_Info.h"
#include "skill_string_info.h"

StringManager::StringManager() = default;

StringManager::~StringManager() = default;

void StringManager::InsertStringInfo(StringInfo info)
{
	_list.insert(std::make_pair(info.GetItemId(), info));
}

void StringManager::InsertSkillStringInfo(SkillStringInfo info)
{
	_skill_list.insert(std::make_pair(info.GetSkillId(), info));
}

void StringManager::LoadStringInfo()
{
	XmlReader::GetInstance().LoadItemString("Consume.img.xml", "imgdir/imgdir");
	XmlReader::GetInstance().LoadItemString("Etc.img.xml", "imgdir/imgdir/imgdir");
	XmlReader::GetInstance().LoadItemString("Eqp.img.xml", "imgdir/imgdir/imgdir/imgdir");
	XmlReader::GetInstance().LoadSkillString();
}

StringInfo StringManager::FindItemStringInfo(int32_t item_id)
{
	auto data = _list.find(item_id);
	if (data != _list.end())
	{
		return data->second;
	}
	return StringInfo();
}

SkillStringInfo StringManager::FindSkillStringInfo(int32_t item_id)
{
	auto data = _skill_list.find(item_id);
	if (data != _skill_list.end())
	{
		return data->second;
	}
	return SkillStringInfo();
}
