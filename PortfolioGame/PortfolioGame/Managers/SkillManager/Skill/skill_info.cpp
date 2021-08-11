#include "../../../pch.h"
#include "skill_info.h"

SkillInfo::SkillInfo() :
	_attack_count(0),
	_mad(0),
	_mp_con(0),
	_rect()
{
}

SkillInfo::~SkillInfo()
{
}

int32_t SkillInfo::GetX() const
{
	return _x;
}

int32_t SkillInfo::GetY() const
{
	return _y;
}

void SkillInfo::SetX(const int32_t x)
{
	_x = x;
}

void SkillInfo::SetY(const int32_t y)
{
	_y = y;
}

int32_t SkillInfo::GetAttackCount() const
{
	return _attack_count;
}

int32_t SkillInfo::GetMobCount() const
{
	return _mob_count;
}

void SkillInfo::SetMobCount(int32_t mob_count)
{
	_mob_count = mob_count;
}

int32_t SkillInfo::GetMad() const
{
	return _mad;
}

int32_t SkillInfo::GetMpCon() const
{
	return _mp_con;
}

int32_t SkillInfo::GetTime() const
{
	return _time;
}

int32_t SkillInfo::GetPdd() const
{
	return _pdd;
}

void SkillInfo::SetPdd(int32_t pdd)
{
	_pdd = pdd;
}

void SkillInfo::SetTime(const int32_t time)
{
	_time = time;
}

RECT SkillInfo::GetRect() const
{
	return _rect;
}

void SkillInfo::SetAttackCount(const int32_t attack_count)
{
	_attack_count = attack_count;
}

void SkillInfo::SetMad(const int32_t mad)
{
	_mad = mad;
}

void SkillInfo::SetMpCon(const int32_t mp_con)
{
	_mp_con = mp_con;
}

void SkillInfo::SetRect(const RECT rect)
{
	_rect = rect;
}
