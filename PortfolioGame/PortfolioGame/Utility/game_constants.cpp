#include "../pch.h"
#include "game_constants.h"

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
