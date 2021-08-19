#include "../../../../pch.h"
#include "ai_movement.h"

#include "../monster.h"
#include "../AttackInfo/attack_info.h"
#include "../MonsterUtility/monster_frame_manager.h"
#include "../../../../Managers/MapManager/map_manager.h"

AiMovement::AiMovement(float speed) :
	_move_range(static_cast<float>(rand() % 250)),
	_facing_direction(rand() % 2),
	_speed(speed),
	_is_stand(false)
{
	ResetMoveRange();
}

float AiMovement::Moveing()
{
	bool is_check_attack = false;
	if (_partner->GetPlayer() != nullptr && !_partner->GetAttackInfo().empty())
	{
		if (!_partner->IsAttacking())
		{
			AttackInfo* attack_info = nullptr;
			std::string key;
			char attkey[100];
			int value = rand() % _partner->GetAttackInfo().size() + 1;
			snprintf(attkey, 100, "attack%d", value);

			attack_info = _partner->GetAttackInfo().find(attkey)->second;
			auto tick = _partner->FindAttackDelay(attkey);
			if (GetTickCount64() > tick)
			{
				switch (value)
				{
				case 1:
					_partner->ChangeState(Monster::MonsterState::kAttack1);
					_partner->SetAttackTick();
					_partner->SetIsAttacking(true);
					_partner->SetStateString(attkey);
					_partner->InsertAttackDelay(attkey, GetTickCount64() + 10000);
					break;
				case 2:
					_partner->ChangeState(Monster::MonsterState::kAttack2);
					_partner->SetAttackTick();
					_partner->SetIsAttacking(true);
					_partner->SetStateString(attkey);
					_partner->InsertAttackDelay(attkey, GetTickCount64() + 15000);
					break;
				case 3:
					_partner->ChangeState(Monster::MonsterState::kAttack3);
					_partner->SetAttackTick();
					_partner->SetIsAttacking(true);
					_partner->SetStateString(attkey);
					_partner->InsertAttackDelay(attkey, GetTickCount64() + 20000);
					break;
				default:
					std::cout << "À¸¾Ó " << std::endl;
					break;
				}

				if (_partner->GetPlayer() != nullptr)
				{
					if (_partner->GetPlayer()->GetInfo().x > _partner->GetInfo().x)
					{
						SetFacingDirection(true);
					}
					else if (_partner->GetPlayer()->GetInfo().x < _partner->GetInfo().x)
					{
						SetFacingDirection(false);
					}
				}
			}
			return 0.f;
		}
	}

	if (_is_stand)
	{
		if (GetTickCount64() > _stand_tick + 3000)
		{
			ResetMoveRange();
		}
		return 0;
	} else if (!_facing_direction)
	{
		if (_move_range + _speed > 0)
		{
			ResetMoveRange();
		}
		_move_range += _speed;
		return -_speed;
	}
	if (_move_range - _speed < 0)
	{
		ResetMoveRange();
	}
	_move_range -= _speed;
	return _speed;
}

void AiMovement::SetFacingDirection(bool facing)
{
	_facing_direction = facing;
}

bool AiMovement::GetFacingDirection() const
{
	return _facing_direction;
}

void AiMovement::GainMoveRange(float value)
{
	_move_range += value;
}

float AiMovement::GetMoveRange() const
{
	return _move_range;
}

void AiMovement::SetPartner(Monster* partner)
{
	_partner = partner;
}

Monster* AiMovement::GetPartner()
{
	return _partner;
}

void AiMovement::ResetMoveRange()
{
	if (rand() % 10 == 1)
	{
		_is_stand = true;
		_stand_tick = GetTickCount64();
		return;
	}
	_facing_direction = rand() % 2;
	_move_range = static_cast<float>(rand() % 1000);
	if (!_facing_direction)
		_move_range *= -1;

	_is_stand = false;
	_stand_tick = 0;
}
