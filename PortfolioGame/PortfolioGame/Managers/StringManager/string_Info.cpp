#include "../../pch.h"
#include "string_Info.h"

StringInfo::StringInfo(): _item_id(0)
{
}


void StringInfo::SetItemId(const int32_t item_id)
{
	_item_id = item_id;
}

void StringInfo::SetName(const std::wstring name)
{
	_name = name;
}

void StringInfo::SetDesc(const std::wstring desc)
{
	_desc = desc;
}

int32_t StringInfo::GetItemId() const
{
	return _item_id;
}

std::wstring StringInfo::GetName() const
{
	return _name;
}

std::wstring StringInfo::GetDesc() const
{
	return _desc;
}
