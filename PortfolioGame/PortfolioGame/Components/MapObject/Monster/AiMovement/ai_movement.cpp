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
		std::string useAttack;
		for (auto attack_info : _partner->GetAttackInfo())
		{
			RECT range = attack_info.second->GetRange();
			RECT rect{
				static_cast<int>(_partner->GetInfo().x) - std::abs(range.left),
			static_cast<int>(_partner->GetInfo().y) - std::abs(range.top),
			static_cast<int>(_partner->GetInfo().x) + std::abs(range.right),
			static_cast<int>(_partner->GetInfo().y) + std::abs(range.bottom) };
			if (_partner->GetAttackDelaySize() == 0)
			{
				if (MapManager::GetInstance()->MonsterHitBoxCollision(rect))
				{
					auto now = GetTickCount64();
					auto tick = _partner->FindAttackDelay(attack_info.first);
					if (GetTickCount64() > tick + 10000)
					{
						if (attack_info.first == "attack1")
						{
							_partner->ChangeState(Monster::MonsterState::kAttack1);
							useAttack = attack_info.first;
							_partner->SetStateString(attack_info.first);
						}
						else if (attack_info.first == "attack2")
						{
							_partner->ChangeState(Monster::MonsterState::kAttack2);
							useAttack = attack_info.first;
							_partner->SetStateString(attack_info.first);
						}
						else if (attack_info.first == "attack3")
						{
							_partner->ChangeState(Monster::MonsterState::kAttack3);
							useAttack = attack_info.first;
							_partner->SetStateString(attack_info.first);
						}
						_partner->InsertAttackDelay(attack_info.first, GetTickCount64());

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
						return 0;
					}
				}
			} else if ((!attack_info.second->IsEffectFinish() && attack_info.second->IsAttackFinish()) ||
				(attack_info.second->IsEffectFinish() && !attack_info.second->IsAttackFinish()))
			{
				is_check_attack = true;
				useAttack = attack_info.first;
			}
		}
		if (!is_check_attack)
		{
			AttackInfo* attack_info = nullptr;
			std::string key;
			char attkey[100];
			int value = rand() % _partner->GetAttackInfo().size() + 1;
			snprintf(attkey, 100, "attack%d", value);

			attack_info = _partner->GetAttackInfo().find(attkey)->second;
			auto tick = _partner->FindAttackDelay(attkey);
			if (GetTickCount64() > tick + 10000)
			{
				switch (value)
				{
				case 1:
					_partner->ChangeState(Monster::MonsterState::kAttack1);
					useAttack = attkey;
					_partner->SetStateString(attkey);
					break;
				case 2:
					_partner->ChangeState(Monster::MonsterState::kAttack2);
					useAttack = attkey;
					_partner->SetStateString(attkey);
					break;
				case 3:
					_partner->ChangeState(Monster::MonsterState::kAttack3);
					useAttack = attkey;
					_partner->SetStateString(attkey);
					break;
				}
				_partner->InsertAttackDelay(attkey, GetTickCount64());

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
			if (!_partner->GetStateString().empty())
			{
				_partner->AttackApply(_partner->GetStateString());
			}
			return 0.f;
		}
	}
	
	if (is_check_attack)
	{
		return 0.f;
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
