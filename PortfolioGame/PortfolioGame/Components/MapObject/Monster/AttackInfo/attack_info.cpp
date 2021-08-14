#include "../../../../pch.h"
#include "attack_info.h"
#include "../MonsterUtility/monster_frame_manager.h"

class MonsterParts;

AttackInfo::AttackInfo(): _attack_after(0),
                          _attack_after_tick(0),
                          _is_attack_finish(false),
                          _effect_after(0),
                          _effect_after_tick(0),
                          _is_effect_finish(false),
                          _con_mp(0),
                          _is_deadly_attack(false),
                          _range(),
                          _type(0),
                          _area_warning_frame(nullptr),
                          _effect_frame(nullptr),
                          _hit_effect_frame(nullptr)
{
}

AttackInfo::~AttackInfo()
{
}

void AttackInfo::SetAttackAfter(const int32_t after)
{
	_attack_after = after;
}

void AttackInfo::SetEffectAfter(int32_t after)
{
	_effect_after = after;
}

void AttackInfo::SetAttackAfterTick(int64_t after)
{
	_attack_after_tick = after;
}

void AttackInfo::SetEffectAfterTick(int64_t after)
{
	_effect_after_tick = after;
}

int32_t AttackInfo::GetEffectAfter() const
{
	return _effect_after;
}

void AttackInfo::SetMpCon(const int32_t after)
{
	_con_mp = after;
}

void AttackInfo::SetDeadlyAttack(const bool is_deadly_attack)
{
	_is_deadly_attack = is_deadly_attack;
}

void AttackInfo::SetEffect(const std::vector<std::shared_ptr<MonsterParts>> effect)
{
	_effect = effect;
	if (_effect_frame == nullptr)
	{
		_effect_frame = new MonsterFrameManager();
	}
}

void AttackInfo::SetHitEffect(const std::vector<std::shared_ptr<MonsterParts>> effect)
{
	_hit_effect = effect;

	if (_hit_effect_frame == nullptr)
	{
		_hit_effect_frame = new MonsterFrameManager();
	}
}

void AttackInfo::SetAreaWarning(const std::vector<std::shared_ptr<MonsterParts>> effect)
{
	_area_warning = effect;
	if (_area_warning_frame == nullptr)
	{
		_area_warning_frame = new MonsterFrameManager();
	}
}

void AttackInfo::SetRange(RECT range)
{
	_range = range;
}

void AttackInfo::SetType(int32_t type)
{
	_type = type;
}

void AttackInfo::SetEffectFinish(bool check)
{
	_is_effect_finish = check;
}

void AttackInfo::SetAttackFinish(bool check)
{
	_is_attack_finish = check;
}

int32_t AttackInfo::GetAttackAfter() const
{
	return _attack_after;
}

int64_t AttackInfo::GetAttackAfterTick() const
{
	return _attack_after_tick;
}

int64_t AttackInfo::GetEffectAfterTick() const
{
	return _effect_after_tick;
}

int32_t AttackInfo::GetConMp() const
{
	return _con_mp;
}

int32_t AttackInfo::IsDeadlyAttack() const
{
	return _is_deadly_attack;
}

bool AttackInfo::IsAttackFinish() const
{
	return _is_attack_finish;
}

bool AttackInfo::IsEffectFinish() const
{
	return _is_effect_finish;
}

RECT AttackInfo::GetRange() const
{
	return _range;
}

std::vector<std::shared_ptr<MonsterParts>>& AttackInfo::GetAreaWarning()
{
	return _area_warning;
}

std::vector<std::shared_ptr<MonsterParts>>& AttackInfo::GetEffect()
{
	return _effect;
}

std::vector<std::shared_ptr<MonsterParts>>& AttackInfo::GetHitEffect()
{
	return _hit_effect;
}

int32_t AttackInfo::GetType() const
{
	return _type;
}

MonsterFrameManager* AttackInfo::GetAreaWarningFrame() const
{
	return _area_warning_frame;
}

MonsterFrameManager* AttackInfo::GetEffectFrame() const
{
	return _effect_frame;
}

MonsterFrameManager* AttackInfo::GetHitEffectFrame() const
{
	return _hit_effect_frame;
}

void AttackInfo::ResetEffectFrame()
{
	if (_effect_frame != nullptr)
	{
		_attack_after_tick = 0;
		_effect_after_tick = 0;
		_effect_frame->SetFrameNumber(0);
		_effect_frame->SetFrameRevers(false);
		_effect_frame->SetFrameTick(0);
		_is_effect_finish = false;
		_is_attack_finish = false;
	}
	//MonsterFrameManager* _effect_frame;
	//MonsterFrameManager* _hit_effect_frame;
}

void AttackInfo::ResetAreaFrame()
{
	if (_area_warning_frame != nullptr)
	{
		_area_warning_frame->SetFrameNumber(0);
		_area_warning_frame->SetFrameRevers(false);
		_area_warning_frame->SetFrameTick(0);
		_area_attack.clear();
	}
}

void AttackInfo::InsertAreaAttack(POINT pos)
{
	_area_attack.emplace_back(pos);
}

std::list<POINT>& AttackInfo::GetAreaAttack()
{
	return _area_attack;
}

bool AttackInfo::IsAttack()
{
	return !_effect.empty();
}
