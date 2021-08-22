#include "../pch.h"
#include "game_constants.h"
#include "../Components/MapObject/Player/Buff/buffstat.h"

uint32_t GameConstants::ShowSkillTabSize(const int job)
{
	if (job == 0)
		return 0;

	if (job % 100 == 0)
	{
		return 1;
	}
	return 2 + (job % 10);
}

ObjectType::BuffFlag GameConstants::SkillToBuffStat(int skill_id)
{
	switch (skill_id)
	{
	case 2001002:
	{
		return ObjectType::BuffFlag::kMagicGuard;
	}
	case 2201001:
	{
		return ObjectType::BuffFlag::kMeditation;
	}
	case 2221004:
	{
		return ObjectType::BuffFlag::kinfinity;
	}
	}
	return ObjectType::BuffFlag::kEnd;
}
