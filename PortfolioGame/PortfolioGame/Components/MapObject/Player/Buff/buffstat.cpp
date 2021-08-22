#include "../../../../pch.h"
#include "buffstat.h"

BuffStat::BuffStat(): _flag(0)
{
}

BuffStat::~BuffStat()
{
}

void BuffStat::AddFlag(ObjectType::BuffFlag flag)
{
	_flag |= static_cast<int>(flag);
}

void BuffStat::RemoveFlag(ObjectType::BuffFlag flag)
{
	_flag ^= static_cast<int>(flag);
}

bool BuffStat::CheckFlag(ObjectType::BuffFlag flag) const
{
	return _flag & static_cast<int>(flag);
}
